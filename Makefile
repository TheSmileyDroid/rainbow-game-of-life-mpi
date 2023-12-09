CC = mpicc
CFLAGS = -g -Wall
TARGET = rainbow
ARGS = 50 50

run: $(TARGET).out
	mpirun -n 4 --oversubscribe ./$(TARGET).out $(ARGS)

all: $(TARGET).out

$(TARGET).out: $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET).out $(TARGET).c

clean:
	$(RM) $(TARGET).out
