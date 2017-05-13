#ifndef TSAN_INTERFACE_H
#define TSAN_INTERFACE_H

// This header should NOT include any other headers.
// All functions in this header are extern "C" and start with __tsan_.

#ifdef __cplusplus
extern "C" {
#endif

// This function should be called at the very beginning of the process,
// before any instrumented code is executed and before any call to malloc.
void __tsan_init();

void __tsan_read1(void *addr);

void __tsan_read2(void *addr);

void __tsan_read4(void *addr);

void __tsan_read8(void *addr);

void __tsan_write1(void *addr);

void __tsan_write2(void *addr);

void __tsan_write4(void *addr);

void __tsan_write8(void *addr);

void __tsan_func_entry(void *call_pc);
void __tsan_func_exit();

void __tsan_thread_create(void * child_id);

void __tsan_thread_join(void * child_id);

void __tsan_thread_lock(void * lock);

void __tsan_thread_unlock(void * lock);

void __tsan_vptr_update(void **vptr_p, void *new_val);
void __tsan_vptr_read(void **vptr_p);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // TSAN_INTERFACE_H
