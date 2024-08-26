
#include "Barrier.h"
#include "ReadWriteLock.h"
#include "Semaphore.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREADSNUMBER 10
#define READERS 10
#define WRITERS 2A

int shared_data = 0;
struct RWLock rwlock;
struct semaphore sem;
struct barrier barrier;

void *tarea(void *arg) {
  int thread_id = *(int *)arg;

  printf("Hilo %d: Ejecutando la primera fase.\n", thread_id);
  sleep(rand() % 2 + 1); // Simular trabajo en la primera fase

  // Llegar a la barrera y esperar a que todos los hilos lleguen
  printf("Hilo %d: Esperando en la barrera.\n", thread_id);
  barrierWait(&barrier);

  // Después de que todos los hilos hayan pasado la barrera
  printf("Hilo %d: Continuando con la segunda fase.\n", thread_id);
  sleep(rand() % 2 + 1); // Simular trabajo en la segunda fase

  return NULL;
}

// Función que representa a un lector
void *reader(void *arg) {
  int id = *((int *)arg);
  while (1) {
    RWLockReadLock(&rwlock); // Adquiere el lock en modo lectura
    printf("Lector %d: El valor de shared_data es %d\n", id, shared_data);
    RWLockUnlock(&rwlock); // Libera el lock
    sleep(1);              // Simula trabajo del lector
    RWLockReadLock(&rwlock);
    if (shared_data == 10) {
      RWLockUnlock(&rwlock);
      break;
    }
    RWLockUnlock(&rwlock);
  }
  return NULL;
}

// Función que representa a un escritor
void *writer(void *arg) {
  int id = *((int *)arg);
  while (1) {
    RWLockWriteLock(&rwlock); // Adquiere el lock en modo escritura
    shared_data += 1;
    printf("Escritor %d: Incrementó shared_data a %d\n", id, shared_data);
    RWLockUnlock(&rwlock); // Libera el lock
    sleep(2);              // Simula trabajo del escritor
    RWLockReadLock(&rwlock);
    if (shared_data == 10) {
      RWLockUnlock(&rwlock);
      break;
    }
    RWLockUnlock(&rwlock);
  }
  return NULL;
}

void *phylosophers(void *arg) {
  int id = *((int *)arg);
  printf("Soy el filosofo %d estoy pensando\n", id);
  sleep(rand() % 2 + 1);
  printf("Soy el filosofo %d tengo hambre\n", id);
  semaphoreWait(&sem);
  semaphoreWait(&sem);
  printf("Soy el filosofo %d estoy comiendo\n", id);
  sleep(rand() % 5 + 1);
  semaphoreSignal(&sem);
  semaphoreSignal(&sem);
  printf("Soy el filosofo %d termino de comer vuelvo a pensar\n", id);
  return 0;
}

int main() {
  printf("Para iniciar con el ejemplo de la 'Barrera' presione enter o "
         "cualquier tecla. \n\n");
  getchar();
  pthread_t threads[THREADSNUMBER];
  barrierInit(&barrier, THREADSNUMBER);

  for (int i = 0; i < THREADSNUMBER; i++) {
    int *val = malloc(sizeof(int));
    *val = i;
    pthread_create(&threads[i], NULL, tarea, (void *)val);
  }

  for (int i = 0; i < THREADSNUMBER; i++) {
    pthread_join(threads[i], NULL);
  }

  barrierDestroy(&barrier);
  printf("\n\n El ejemplo de la 'Barrera' ha terminado, para continuar con el "
         "ejemplo del 'Semáforo' presione enter o cualquier tecla. \n\n");
  getchar();
  int num_philosophers = 5;
  pthread_t philosophers_threads[num_philosophers];
  semaphoreInit(&sem, num_philosophers);
  for (int i = 0; i < num_philosophers; i++) {
    int *id = malloc(sizeof(int));
    *id = i + 1;
    pthread_create(&philosophers_threads[i], NULL, phylosophers, id);
  }
  for (int i = 0; i < num_philosophers; i++) {
    pthread_join(philosophers_threads[i], NULL);
  }
  semaphoreDestroy(&sem);
  printf(
      "\n\n El ejemplo del 'Semáforo' ha terminado, para continuar con el "
      "ejemplo del 'Read/Write Lock' presione enter o cualquier tecla. \n\n");
  getchar();

  pthread_t readers[READERS], writers[WRITERS];
  int reader_ids[READERS], writer_ids[WRITERS];

  // Inicializa el rwlock
  RWLockInit(&rwlock);

  // Crea los hilos lectores
  for (int i = 0; i < READERS; i++) {
    reader_ids[i] = i;
    pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
  }

  // Crea los hilos escritores
  for (int i = 0; i < WRITERS; i++) {
    writer_ids[i] = i;
    pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
  }

  // Espera a que los hilos terminen (en este caso, nunca terminarán)
  for (int i = 0; i < READERS; i++) {
    pthread_join(readers[i], NULL);
  }

  for (int i = 0; i < WRITERS; i++) {
    pthread_join(writers[i], NULL);
  }

  // Destruye el rwlock
  RWLockDestroy(&rwlock);

  printf("\n\n El ejemplo del 'Read/Write Lock' ha terminado, para terminar el "
         "programa presione enter o cualquier tecla. \n\n");
  getchar();

  return 0;
}
