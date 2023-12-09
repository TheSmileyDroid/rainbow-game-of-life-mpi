CC = mpicc
CFLAGS = -g -Wall
TARGET = rainbow

all: $(TARGET).out

$(TARGET).out: $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET).out $(TARGET).c

clean:
	$(RM) $(TARGET).out
