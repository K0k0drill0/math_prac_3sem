CC = gcc
CFLAGS = -g -fdiagnostics-color=always

main: validators.o memory.o main.o 
	$(CC) $(CFLAGS) -o main main.o validators.o memory.o  -lm

main.o: main.c validators.h memory.h
	$(CC) $(CFLAGS) -c main.c -lm

validators.o: validators.c validators.h 
	$(CC) $(CFLAGS) -c validators.c -lm

memory.o: memory.c validators.h memory.h 
	$(CC) $(CFLAGS) -c memory.c -lm

.PHONY: clean

clean:
	rm -f *.o main