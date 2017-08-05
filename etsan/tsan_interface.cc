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
  printf("ThreadSanitizer initializing\n");
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

void __tsan_read16(void * addr) {
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

void __tsan_write16(void* addr) {
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


__tsan_atomic8 __tsan_atomic8_load(const volatile __tsan_atomic8 *a, __tsan_memory_order mo) {
 ft_atomic_load();
}
__tsan_atomic16 __tsan_atomic16_load(const volatile __tsan_atomic16 *a, __tsan_memory_order mo) {
 ft_atomic_load();
}

__tsan_atomic32 __tsan_atomic32_load(const volatile __tsan_atomic32 *a, __tsan_memory_order mo) {
  ft_atomic_load();
}

a8 __tsan_atomic32_fetch_add(volatile a8 *a, a8 v, __tsan_memory_order mo) {
  printf("EmbedSanitizer: atomic_fetch_add not implemented\n");
}

void __tsan_unaligned_read2(const void *addr) {
  ft_unaligned_read();
}

void __tsan_unaligned_read4(const void *addr) {
  ft_unaligned_read();
}

void __tsan_unaligned_read8(const void *addr) {
  ft_unaligned_read();
}

void __tsan_unaligned_read16(const void *addr) {
  ft_unaligned_read();
}

void __tsan_unaligned_write2(void *addr) {
  ft_unaligned_write();
}

void __tsan_unaligned_write4(void *addr) {
  ft_unaligned_write();
}

void __tsan_unaligned_write8(void *addr) {
  ft_unaligned_write();
}
void __tsan_unaligned_write16(void *addr) {
  ft_unaligned_write();
}
