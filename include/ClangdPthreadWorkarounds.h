#pragma once

struct pthread_once_t {
  int dummy;
  pthread_once_t() : dummy(0) {}
};
#define PTHREAD_ONCE_INIT pthread_once_t()
typedef int pthread_t;
typedef void* (*pthread_start_routine_t)(void*);
inline int pthread_create(pthread_t*, void*, pthread_start_routine_t, void*) { return 0; }
inline int pthread_join(pthread_t, void**) { return 0; }
inline int pthread_detach(pthread_t) { return 0; }
inline int pthread_equal(pthread_t, pthread_t) { return 0; }
inline pthread_t pthread_self() { return 0; }
inline int pthread_once(pthread_once_t*, void (*)(void)) { return 0; }
typedef int pthread_key_t;
inline int pthread_key_create(pthread_key_t*, void (*)(void*)) { return 0; }
inline int pthread_key_delete(pthread_key_t) { return 0; }
inline void* pthread_getspecific(pthread_key_t) { return nullptr; }
inline int pthread_setspecific(pthread_key_t, const void*) { return 0; }
typedef int pthread_mutex_t;
inline int pthread_mutex_init(pthread_mutex_t*, void*) { return 0; }
inline int pthread_mutex_destroy(pthread_mutex_t*) { return 0; }
inline int pthread_mutex_lock(pthread_mutex_t*) { return 0; }
inline int pthread_mutex_trylock(pthread_mutex_t*) { return 0; }
inline int pthread_mutex_timedlock(pthread_mutex_t*, const void*) { return 0; }
inline int pthread_mutex_unlock(pthread_mutex_t*) { return 0; }
typedef int pthread_mutexattr_t;
inline int pthread_mutexattr_init(pthread_mutexattr_t*) { return 0; }
#define PTHREAD_MUTEX_RECURSIVE 0
inline int pthread_mutexattr_settype(pthread_mutexattr_t*, int) { return 0; }
inline int pthread_mutexattr_destroy(pthread_mutexattr_t*) { return 0; }
typedef int pthread_cond_t;
inline int pthread_cond_init(pthread_cond_t*, void*) { return 0; }
inline int pthread_cond_destroy(pthread_cond_t*) { return 0; }
inline int pthread_cond_wait(pthread_cond_t*, pthread_mutex_t*) { return 0; }
inline int pthread_cond_signal(pthread_cond_t*) { return 0; }
inline int pthread_cond_broadcast(pthread_cond_t*) { return 0; }
inline int pthread_cond_timedwait(pthread_cond_t*, pthread_mutex_t*, const void*) { return 0; }
