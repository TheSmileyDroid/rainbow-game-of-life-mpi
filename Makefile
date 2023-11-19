CC = gcc
CFLAGS = -g -Wall -fopenmp -pthread
TARGET = rainbow

all: $(TARGET).out $(TARGET)_omp.out $(TARGET)_pthread.out

$(TARGET).out: $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET).out $(TARGET).c

$(TARGET)_omp.out: $(TARGET)_omp.c
	$(CC) $(CFLAGS) -o $(TARGET)_omp.out $(TARGET)_omp.c

$(TARGET)_pthread.out: $(TARGET)_pthread.c
	$(CC) $(CFLAGS) -o $(TARGET)_pthread.out $(TARGET)_pthread.c

clean:
	$(RM) $(TARGET).out
	$(RM) $(TARGET)_omp.out
	$(RM) $(TARGET)_pthread.out
