#include "ReadWriteLock.h"

void RWLockInit(struct RWLock *rwlock) {
  pthread_mutex_init(&(rwlock->lock), NULL);
  pthread_cond_init(&(rwlock->cond), NULL);
  rwlock->readers = 0;
  rwlock->isWriter = 0;
}
void RWLockReadLock(struct RWLock *rwlock) {
  pthread_mutex_lock(&(rwlock->lock));
  while (rwlock->isWriter > 0)
    pthread_cond_wait(&(rwlock->cond), &(rwlock->lock));
  rwlock->readers += 1;
  pthread_mutex_unlock(&(rwlock->lock));
}
void RWLockWriteLock(struct RWLock *rwlock) {
  pthread_mutex_lock(&(rwlock->lock));
  while (rwlock->readers > 0)
    pthread_cond_wait(&(rwlock->cond), &(rwlock->lock));
  rwlock->isWriter = 1;
  pthread_mutex_unlock(&(rwlock->lock));
}
void RWLockUnlock(struct RWLock *rwlock) {
  pthread_mutex_lock(&(rwlock->lock));
  if (rwlock->isWriter == 1) {
    rwlock->isWriter = 0;
  } else {
    rwlock->readers -= 1;
    if (rwlock->readers == 0)
      pthread_cond_broadcast(&(rwlock->cond));
  }
  pthread_mutex_unlock(&(rwlock->lock));
}

void RWLockDestroy(struct RWLock *rwlock) {
  pthread_mutex_destroy(&(rwlock->lock));
}
