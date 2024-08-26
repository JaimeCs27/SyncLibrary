#include "pthread.h"

struct semaphore {
  int value;
  pthread_mutex_t lock;
  pthread_cond_t cond;
};

void semaphoreInit(struct semaphore *sem, int value);
void semaphoreDestroy(struct semaphore *sem);
void semaphoreWait(struct semaphore *sem);
void semaphoreSignal(struct semaphore *sem);
