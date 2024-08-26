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

## Créditos

- Jaime Cabezas Segura
- Sebastián Lopez Villavicencio
- Natasha Calderón Rojas
