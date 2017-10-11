//===-- Runtime race detection module of EmbedSanitizer - for Embeded ARM--===//
//
//
// This file is distributed under the BSD 3-clause "New" or "Revised" License
// License. See LICENSE.md for details.
//
//===----------------------------------------------------------------------===//
//
// (c) 2017 - Hassan Salehe Matar, Koc University
//            Email: hmatar@ku.edu.tr
//===----------------------------------------------------------------------===//

// Holds various definitions

#ifndef FT_DEFS_H
#define FT_DEFS_H

#include <unordered_map>
#include <vector>
#include <pthread.h>
#include <stdio.h>
#include <assert.h>
#include <thread>
#include <mutex>
#include <atomic>

using namespace std;

typedef void * Address;
typedef unsigned int ThreadID;
typedef vector<int> VectorClock;

#define TID(x) ((x & 0xFF000000) >> 24)
#define CLOCK(x) (x & (0x00FFFFFF))

#define READ_SHARED 0XEFFFFFFF

#define REPORT_RACES 1

#ifdef REPORT_RACES
#define race_message(msg) { printf("Race :%s\n", msg); }
#else
#define race_message(msg) { }
#endif

#define error(x,msg) {	\
  if(!x.Racy) {		\
   race_message(msg);	\
   x.Racy = true;	\
 } 			\
  VS.mGuard.unlock(); 	\
  return;		\
 }

#define FastPathReturn { VS.mGuard.unlock(); return reportIsRacy;}

/**
 * Maybe unnecessary but keeps track of
 * number of parallel threads in the program.
 * Invariant: NumThreads == C.size() */
static unsigned int NumThreads = 0;

/**
 * This variable tracks availability
 * of multiple threads in the program. No race is
 * detected if there are no multithreads in the program */
atomic_int isConcurrent{0};

/********************************************/
/**   Thread state related metadata        **/
/********************************************/

class ThreadState {
  public:
    int tid;
    VectorClock C;
    int epoch; // invariant: epoch == C[tid]

    void updateEpoch() { epoch = C[tid]; }
    void increment() {
      epoch++;
      C[tid] = epoch;
      assert(C[tid] == epoch);
    }
};

/*
 * This class holds states of all concurrent threads
 * in the system. I has a single lock for each thread
 * to acquire to access its thread states.
 */
class TStates {

public:

  /* A lock to acquire before accesing C */
  mutex mGuard; // lock

  // Threads states
  unordered_map<ThreadID, ThreadState> C;
//#ifdef STATS
  ~TStates() {
    printf("Threads: %d\n", C.size());
  }
//#endif
};

TStates TS; // instance for threads states

/**
 * Updates vector clocks to accomodate vectors
 * of all threads.
 * NOTE: This is a utility function and thus
 * not protected. Use inside a critical section
 * with the TS lock.
 */
void UpdateThreadClocks() {

  int nThreads =  TS.C.size();

  for(auto tv = TS.C.begin(); tv != TS.C.end(); tv++) {

    ThreadState& t = tv->second;

    int idx = t.C.size();
    for(; idx < nThreads; idx++) {
      int epoch = idx << 24;
      if(t.tid == idx)
        epoch = epoch + 1;

      t.C.push_back( epoch );
    }
  }
}

/**
 * Returns the State of a thread whose id is tid
 */
ThreadState & getState(ThreadID tid) {

  ThreadState* st;

  TS.mGuard.lock(); // protect

  if(TS.C.find(tid) == TS.C.end()) {

    TS.C[tid] = ThreadState();
    st = &TS.C[tid];
    NumThreads = TS.C.size(); //
    st->tid = NumThreads;
    st->epoch = ((st->tid << 24) + 1);

    UpdateThreadClocks();
    (st->C)[st->tid] = st->epoch;

    NumThreads = TS.C.size() + 1; // track # of threads
  }
  else
    st = &TS.C[tid];

  TS.mGuard.unlock(); // release protection

  return *st;
}

/**
 * Returns the State of the current thread
 */
ThreadState & getThreadState() {

  ThreadID tid = (ThreadID)pthread_self();
  return getState(tid);
}


/********************************************/
/** Variables states related metadata      **/
/********************************************/

class VarState {
  public:
    int W, R;
    VectorClock Rvc; // used iff R == READ_SHARED

    bool Racy = false;
};

class VStates {

public:

  /* A lock to acquire before accesing Vstates */
  mutex mGuard;

  // Variables states
  unordered_map<Address, VarState> Vstates;

//#ifdef STATS
  unsigned int reads{0};
  unsigned int writes{0};

  ~VStates() {
    printf("Addresses: %u\n", Vstates.size());
    printf("Reads: %u\n", (unsigned int)reads);
    printf("Writes: %u\n", (unsigned int)writes);
    int races = 0;
    for(auto addr = Vstates.begin(); addr != Vstates.end(); addr++)
       if((addr->second).Racy)
         races++;
    printf("Races: %d\n", races);

  }
//#endif
};

VStates VS; // instance for variables states

/**
 * Returns VarState instance for a memory
 * address "addr". If none exists already,
 * it creates one and stores in Vstates.
 */
VarState & getVarState(Address addr, bool isWrite) {

  VarState* vstt;

  VS.mGuard.lock(); // protect

  if(VS.Vstates.find(addr) == VS.Vstates.end()) {
    ThreadState & t = getThreadState();
    VarState vs;
    vs.W = (t.tid << 24);
    vs.R = (t.tid << 24);
    if(isWrite)
      vs.W = t.epoch;
    else
      vs.R = t.epoch;
    VS.Vstates[addr] = vs;
    vstt = &VS.Vstates[addr];
  }
  else
    vstt = &VS.Vstates[addr];

  VS.mGuard.unlock(); // release protection

  return *vstt;
}

/********************************************/
/**   Locks state related metadata         **/
/********************************************/
class LockState {
  public:
    VectorClock L;
};

class LStates {

public:

  /* A lock to acquire before accesing L */
  mutex mGuard;

  // Locks states
  unordered_map<Address, LockState> L;

//#ifdef STATS
  ~LStates() {
    printf("Locks: %u\n", L.size());
  }
//#endif
};

LStates LS; // instance for locks states metadata

/**
 * Initializes clocks in the vector clock to 0
 * for all threads 0 ... size-1 for a vector clock VC
 */
void newVectorClock(VectorClock& VC, int size) {
  VC.resize( size );
  for(int t = 0; t < size; t++)
    VC[t] = (t << 24); // =0?
}

/**
 * Updates vector clock to accomodate epochs
 * of new dynamically created threads.
 */
void ExtendVectorClock(VectorClock& C, int totalThreads) {

  int tid = C.size();

  for(; tid < totalThreads; tid++) {
    int epoch = tid << 24;
    C.push_back(epoch);
  }
}


/**
 * Makes sure to extend two Vector clocks C1 and C2
 * to be of same length by appending zeros.
 */
void ExtendVectorClocks(VectorClock& C1, VectorClock& C2) {
  int t_size = C1.size();
  int l_size = C2.size();

  int size = max(t_size, l_size);

  ExtendVectorClock(C1, size);
  ExtendVectorClock(C2, size);
}

/**
 * Returns vector clock state of a lock whose
 * address is "lock"
 */
LockState& getLockState(Address lock) {

  LockState* lockS;

  LS.mGuard.lock(); // protect access

  if(LS.L.find(lock) == LS.L.end()) {
    LS.L[lock] = LockState();
    newVectorClock(LS.L[lock].L, NumThreads);
  }

  lockS = &LS.L[lock];

  LS.mGuard.unlock(); // release lock

  return *lockS;
}

#endif // FT_DEFS_H
