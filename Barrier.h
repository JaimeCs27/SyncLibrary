#include "pthread.h"

struct barrier {
  int waitingThreads;
  int threadsCompleted;
  pthread_mutex_t lock;
  pthread_cond_t cond;
};

void barrierInit(struct barrier *barrier, int threads);
void barrierWait(struct barrier *barrier);
void barrierDestroy(struct barrier *barrier);
