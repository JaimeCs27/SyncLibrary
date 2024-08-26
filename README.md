# Biblioteca de Sincronización

Una biblioteca pequeña con herramientas para la sincronización de hilos, utilizando la biblioteca de pthread.

## Tabla de contenidos
- [Implementación](#implementación)
- [Compilacion](#compilacion)
- [Uso](#uso)
- [Créditos](#créditos)

## Implementación

Para la implementación de los tres métodos de sincronización se utilizó la biblioteca de pthread la cual tiene consigo la estructura MUTEX, esta nos permite blocquear ciertos procesos hasta que se cumpla una condición.

Semáforo:
  En el caso del semáforo, se creo un objeto struct que contiene:
  - int value: cantidad de espacios disponibles.
  - pthread_mutex_t lock: objeto mutex para bloquear procesos.
  - pthread_cond_t cond: objeto condition para dar la señal de soltar el proceso.

  Cuenta con 4 métodos
  - semaphoreInit(struct semaphore *sem, int value): Metodo que inicializa el semáforo, recibe como parámetros, un puntero a un objeto semáforo y un int value, el cual es la cantidad de espacios disponibles.
  - semaphoreDestroy(struct semaphore *sem): Método que se encarga de liberar la memoria del semáforo. Recibe un puntero a un objeto semáforo.
  - semaphoreWait(struct semaphore *sem): Método encargado de asignar uno de los espacios disponibles en caso de que haya al proceso entrante. Recibe un objeto semáforo como parámetro.
  - semaphoreSignal(struct semaphore *sem): Método encargado de dar la señal para terminar de usar el espacio del semáforo. Recibe un objeto semáforo como parámetro.

Barrera:
  En el caso de la barrera, se crea un objeto struct que contiene:
  - int waitingThreads: Cantidad de hilos que ya terminaron su proceso.
  - int threadsCompleted: Cantidad de hilos necesarios para terminar la barrera.
  - pthread_mutex_t lock: objeto mutex para bloquear procesos.
  - pthread_cond_t cond: objeto condition para dar la señal de soltar el proceso.

  Cuenta con 3 métodos:
  barrierInit(struct barrier *barrier, int threads): Método que inicializa la barrera, recibe como parámetro un puntero a un objeto barrier y un int threads, que son la cantidad de procesos necesarios para terminar.
  barrierWait(struct barrier *barrier): Método que pausa el hilo hasta que todos los demás hayan terminado su ejecución. Recibe un puntero a un objeto barrier.
  barrierDestroy(struct barrier *barrier): Método que se encarga de liberar la memoria de la barrera. Recibe un puntero a un objeto barrier.

Read/Write Lock:
  En el caso de R/W lock, se crea un objeto struct con los siguientes campos:
  - pthread_mutex_t lock: objeto mutex para bloquear procesos.
  - pthread_cond_t cond: objeto condition para dar la señal de soltar el proceso.
  - int readers: Cantidad de procesos leyendo.
  - int isWriter: Si un proceso está escribiendo en memoria.

  Cuenta con 5 métodos:
  RWLockInit(struct RWLock *rwlock): Método encargado de inicializar el R/W Lock. recibe como parámetro un puntero a un RWlock.
  RWLockReadLock(struct RWLock *rwlock): Método que adquiere el bloqueo en caso de lectura. recibe como parámetro un puntero a un RWlock.
  RWLockWriteLock(struct RWLock *rwlock): Método que adquiere el bloqueo en caso de escritura. recibe como parámetro un puntero a un RWlock.
  RWLockUnlock(struct RWLock *rwlock): Método que libera el bloqueo en ambos casos lectura o escritura. recibe como parámetro un puntero a un RWlock.
  RWLockDestroy(struct RWLock *rwlock): Método encargado de liberar la memoria del objeto RWLock.

## Compilacion

Para compilar el ejemplo que se encuentra en el archivo "main.c" se debe hacer lo siguiente en la linea de comando donde se encuentre copiado el repositorio:
```console
  foo@bar~:make
```
Esto creara un archivo objeto llamado sync_program.
Para correr el programa, en la carpeta donde se genero el archivo objeto en la linea de comando se usaria la siguiente linea:
```console
  foo@bar~:./sync_program
```

## Uso

Para hacer uso de alguno de los métodos de sincronización de la biblioteca, simplemente se debe hacer un include del archivo .h del método que quisieramos.
Ejemplo:
```C
  #include "Semaphore.h" //En caso de usar semáfor
  #include "Barrier.h" // En caso de usar barrera
  #include "ReadWriteLock.h" // En caso de usar R/W Lock
```
### Uso del Semáforo

Primero se debe crear una instancia del struct semaphore:
```C
struct semaphore [nombreDelSemáforo];
```
Luego en el main debe inicializarse con el método correspondiente:
```C
int cantidadDeEspaciosDisponibles = 5;
semaphoreInit(&[nombreDelSemáforo], cantidadDeEspaciosDisponibles);
```
Ahora en el método creado para correr en los hilos buscamos la zona crítica que queremos proteger, y llamamos el método wait y despues de la región crítica llamariamos al método signal.
```C
void *thread(void* argv){
  // Some stuff

  // Critical Region
  semaphoreWait(&[nombreDelSemáforo]);
  printf("Here is the critical region");

  // End Critical Region
  semaphoreSignal(&[nombreDelSemáforo]);

  return 0;
}
```
Por último despues de que se hiciera join de lo hilos y todo el proceso de sincronizar acabara se destruye el objeto
```C
semaphoreDestroy(&[nombreDelSemáforo]);
```


### Uso de la Barrera
Primero se debe crear una instancia del struct barrier:
```C
struct barrier [nombreDeLaBarrera];
```
Luego en el main debe inicializarse con el método correspondiente:
```C
int cantidadHilos = 3;
barrierInit(&[nombreDeLaBarrera], cantidadHilos);
```

Cuando un hilo llega a la llamada barrierWait(&barrier), se detiene y espera hasta que todos los demás hilos también lleguen a ese mismo punto y una vez que todos los hilos han llegado a la barrera, la barrera se "libera" y todos los hilos pueden continuar con la siguiente fase de ejecución.

```C
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
```
### Uso de Read/Write Lock
Crear una instancia del struct RWLock:
```C
struct RWLock nombreDelRWLock;
```
Antes de usar el RWLock, debe inicializarse con el método RWLockInit. Esto se hace en el main.
```C
RWLockInit(&nombreDelRWLock);
```
Para un lector, antes de acceder a la región crítica (donde se lee un recurso compartido), se debe llamar al método RWLockReadLock. Después de salir de la región crítica, se debe liberar el lock con RWLockUnlock.
```C
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
```
Para un escritor, antes de acceder a la región crítica (donde se modifica un recurso compartido), se debe llamar al método RWLockWriteLock. Después de la operación de escritura, se libera el lock con RWLockUnlock.
```C
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
```
Después de que todos los hilos hayan terminado de ejecutarse y ya no sea necesario el RWLock, debe ser destruido para liberar los recursos utilizados.
```C
RWLockDestroy(&nombreDelRWLock);
```

## Créditos

- Jaime Cabezas Segura
- Sebastián Lopez Villavicencio
- Natasha Calderón Rojas
