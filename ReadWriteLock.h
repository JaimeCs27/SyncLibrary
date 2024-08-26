#include "pthread.h"

struct RWLock {
  pthread_mutex_t lock;
  pthread_cond_t cond;
  int readers;
  int isWriter;
};

void RWLockInit(struct RWLock *rwlock);
void RWLockReadLock(struct RWLock *rwlock);
void RWLockWriteLock(struct RWLock *rwlock);
void RWLockUnlock(struct RWLock *rwlock);
void RWLockDestroy(struct RWLock *rwlock);
