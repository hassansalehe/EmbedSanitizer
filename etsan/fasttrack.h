//===-- fattrack.h implements race detection using Fasttrack algorithm   -===//
//
//
// This file is distributed under the BSD 3-clause "New" or "Revised" License
// License. See LICENSE.md for details.
//
//===----------------------------------------------------------------------===//
//
// (c) 2017, 2018 - Hassan Salehe Matar, Koc University
//            Email: hmatar@ku.edu.tr
//===----------------------------------------------------------------------===//

#ifndef FASTTRACK_HPP_
#define FASTTRACK_HPP_

#include "defs.h"

bool ft_read(VarState & x, ThreadState & t);
bool ft_write(VarState & x, ThreadState & t);

/**
 * Performs race detection at read event
 * @param x memory address state
 * @param t state of the thread which performed read operation
 * @return true if there is a race, false otherwise.
 */
bool ft_read(VarState & x, ThreadState & t) {

  bool reportIsRacy = false;
  VS.mGuard.lock(); // protect
// #ifdef STATS
  VS.reads++;
// #endif

  if (x.Racy) FastPathReturn;

  if (x.R == t.epoch) FastPathReturn;  // Same epoch 63.4%

  // write-read race?
  if ( TID(x.W) != t.tid && CLOCK(x.W) > CLOCK( t.C[TID(x.W)] ) ) {
#ifdef DEBUG
    printf("x.tid: %d, t.tid: %d\n", TID(x.W), t.tid);
#endif
    reportIsRacy = true;
  }

  // update read state
  if (x.R == READ_SHARED) {            // Shared     20.8%

    x.Rvc[t.tid] = t.epoch;

  } else {

    if (x.R <= t.C[TID(x.R)]) {        // Exclusive  15.7%

      x.R = t.epoch;

    } else {                          // Share       0.1%

      if(x.Rvc.size() == 0) {
        newVectorClock(x.Rvc, NumThreads);     // (SLOW PATH)
      }
      x.Rvc[TID(x.R)] = x.R;
      x.Rvc[t.tid] = t.epoch;
      x.R = READ_SHARED;
    }
  }

  VS.mGuard.unlock(); // release protection

  return reportIsRacy;
}

/**
 * Performs race detection on write event
 * @param x memory address state
 * @param t state of the thread which performed read operation
 * @return true if there is a race, false otherwise.
 */
bool ft_write(VarState & x, ThreadState & t) {

  bool reportIsRacy = false;
  VS.mGuard.lock(); // protection

// #ifdef STATS
  VS.writes++;
// #endif

  if (x.Racy) FastPathReturn; // should already have been reported

  if (x.W == t.epoch) FastPathReturn;     // Same epoch 71.0%

  // write-write race?
  if ( TID(x.W) != t.tid && CLOCK(x.W) > CLOCK( t.C[TID(x.W)] ) ) {
    reportIsRacy = true;
  }

  // read-write race?
  if (x.R != READ_SHARED) {   // Write Exclusive 28.9%
    if (TID(x.R) != t.tid && CLOCK(x.R) > CLOCK(t.C[TID(x.R)]) ) {
      reportIsRacy = true;
    }
  } else {                       // Write Shared       0.1%
    for (int u = 0; u < /*NumThreads*/std::min(x.Rvc.size(), t.C.size()); u++) {
      if (x.Rvc[u] > t.C[u]) {// (SLOW PATH)
        reportIsRacy = true; // RACE!
      }
    }
    // also have to set R = epoch
    x.R = (TID(t.epoch) << 24); // 0@tid
  } // a possible bug.

  x.W = t.epoch; // update write state
  VS.mGuard.unlock(); // release protection

  return reportIsRacy;
}


void ft_acquire(ThreadState& t, LockState& lock) {

  LS.mGuard.lock(); // protect

  ExtendVectorClocks(t.C, lock.L);

  // Join: Ct := Ct U Lm
  for (int i = 0; i < lock.L.size(); i++) {
    t.C[i] = std::max(t.C[i], lock.L[i]);
  }

  LS.mGuard.unlock(); // release protection

  t.updateEpoch(); // invariant
}

void ft_release(ThreadState& t, LockState& lock) {

  LS.mGuard.lock(); // protect

  ExtendVectorClocks(t.C, lock.L);

  // Copy: Lm := Ct
  for (int i = 0; i < lock.L.size(); i++) {
    lock.L[i] = t.C[i];
  }

  LS.mGuard.unlock(); // release protection

  t.increment();
}

void ft_fork(ThreadState & t, ThreadState & u){

  isConcurrent++;

  TS.mGuard.lock();

  ExtendVectorClocks(t.C, u.C);

  // Join: Cu := Cu U Ct
  for (int i = 0; i < u.C.size(); i++) {
    u.C[i] = std::max(u.C[i], t.C[i]);
  }

  u.updateEpoch(); // invariant

  t.increment(); // parent state

  TS.mGuard.unlock();
}


void ft_join(ThreadState & t, ThreadState & u){

  if ( isConcurrent ) isConcurrent--;

#ifdef DEBUG
  if (!isConcurrent) printf("No MULTITHREADS\n");
#endif

  TS.mGuard.lock();

  ExtendVectorClocks(t.C, u.C);

  // Join: Ct := Ct U Cu
  for (int i = 0; i < t.C.size(); i++) {
    t.C[i] = std::max(t.C[i], u.C[i]);
  }

  t.updateEpoch(); // invariant
  u.increment(); // child state

  TS.mGuard.unlock(); // release protection
}


void ft_atomic_load() {
  printf("EmbedSanitizer: Warning, atomic load not implemented!\n");
}

#endif // FASTTRACK_HPP_
