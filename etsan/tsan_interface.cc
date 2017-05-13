//===-- tsan_interface.cc -------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "tsan_interface.h"
#include "fasttrack.h"
#include "defs.h"

void __tsan_init() {

  // create metadata for current thread
  //ThreadState& st = getThreadState();
  //printf("Parent thread: %u = %d\n", (unsigned)pthread_self(), st.tid);
}

void __tsan_read1(void * addr) {
  if(isConcurrent)
    ft_read(getVarState(addr, false), getThreadState());
    //  MemoryRead(cur_thread(), CALLERPC, (uptr)addr, kSizeLog1);
}

void __tsan_read2(void * addr) {
  if(isConcurrent)
    ft_read(getVarState(addr, false), getThreadState());
}


void __tsan_read4(void * addr) {
  if(isConcurrent)
    ft_read(getVarState(addr, false), getThreadState());
}

void __tsan_read8(void * addr) {
  if(isConcurrent)
    ft_read(getVarState(addr, false), getThreadState());
}

void __tsan_write1(void * addr) {
  if(isConcurrent)
    ft_write(getVarState(addr, true), getThreadState());
}

void __tsan_write2(void * addr) {
  if(isConcurrent)
    ft_write(getVarState(addr, true), getThreadState());
}

void __tsan_write4(void* addr) {
  if(isConcurrent)
    ft_write(getVarState(addr, true), getThreadState());
}

void __tsan_write8(void* addr) {
  if(isConcurrent)
    ft_write(getVarState(addr, true), getThreadState());
}

void __tsan_func_entry(void *pc) {
    //  FuncEntry(cur_thread(), (uptr)pc);
}

void __tsan_func_exit() {
    //  FuncExit(cur_thread());
}

void __tsan_thread_create(void * childIdAddr) {
  unsigned int child_id = *((unsigned int*)childIdAddr);
  unsigned int parent_id = (unsigned int)pthread_self();
  ft_fork(getThreadState(), getState(child_id));
}

void __tsan_thread_join(void * childIdAddr) {

  unsigned int child_id = ((unsigned int)childIdAddr);
  unsigned int parent_id = (unsigned int)pthread_self();

  ft_join(getThreadState(), getState(child_id));
}

void __tsan_thread_lock(void * lock) {
  ft_acquire(getThreadState(), getLockState(lock));
}

void __tsan_thread_unlock(void * lock) {
  ft_release(getThreadState(), getLockState(lock));
}

void __tsan_vptr_update(void **vptr_p, void *new_val) {
// treat as memory write
//
}

void __tsan_vptr_read(void **vptr_p) {
// tread as memory read
}
