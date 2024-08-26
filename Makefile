
# Compilador y banderas
CC = gcc
CFLAGS = -Wall -pthread -I.

# Archivos fuente y objetos
SRCS = main.c Barrier.c ReadWriteLock.c Semaphore.c
OBJS = $(SRCS:.c=.o)

# Nombre del ejecutable
TARGET = sync_program

# Regla principal para compilar todo
all: $(TARGET)

# Regla para crear el ejecutable
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

# Regla para compilar los archivos .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpiar los archivos generados
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
