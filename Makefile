CC = mpicc
CFLAGS = -g -Wall
TARGET = rainbow
ARGS = 2000 2048
NP = 4


all: $(TARGET).out

run: $(TARGET).out
	mpirun -n $(NP) --oversubscribe ./$(TARGET).out $(ARGS)


$(TARGET).out: $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET).out $(TARGET).c

clean:
	$(RM) $(TARGET).out
