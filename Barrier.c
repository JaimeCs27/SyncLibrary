#include "Barrier.h"

void barrierInit(struct barrier *barrier, int threads) {
  barrier->waitingThreads = threads;
  barrier->threadsCompleted = 0;
  pthread_mutex_init(&(barrier->lock), NULL);
  pthread_cond_init(&(barrier->cond), NULL);
}

void barrierWait(struct barrier *barrier) {
  pthread_mutex_lock(&(barrier->lock));

  barrier->threadsCompleted += 1;
  if (barrier->threadsCompleted < barrier->waitingThreads) {
    pthread_cond_wait(&(barrier->cond), &(barrier->lock));
  } else {
    barrier->threadsCompleted = 0;
    pthread_cond_broadcast(&(barrier->cond));
  }

  pthread_mutex_unlock(&(barrier->lock));
}

void barrierDestroy(struct barrier *barrier) {
  pthread_mutex_destroy(&(barrier->lock));
}
