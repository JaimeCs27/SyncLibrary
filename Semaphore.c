#include "Semaphore.h"

void semaphoreInit(struct semaphore *sem, int value) {
  sem->value = value;
  pthread_mutex_init(&(sem->lock), NULL);
  pthread_cond_init(&(sem->cond), NULL);
}

void semaphoreDestroy(struct semaphore *sem) {
  pthread_mutex_destroy(&(sem->lock));
}

void semaphoreWait(struct semaphore *sem) {
  pthread_mutex_lock(&(sem->lock));
  while (sem->value <= 0) {
    pthread_cond_wait(&(sem->cond), &(sem->lock));
  }
  sem->value--;
  pthread_mutex_unlock(&(sem->lock));
}

void semaphoreSignal(struct semaphore *sem) {
  pthread_mutex_lock(&(sem->lock));
  sem->value++;
  pthread_cond_signal(&(sem->cond));
  pthread_mutex_unlock(&(sem->lock));
}
