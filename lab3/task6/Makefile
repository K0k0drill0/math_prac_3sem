CC = gcc
CFLAGS = -g -fdiagnostics-color=always

main: lists.o main.o 
	$(CC) $(CFLAGS) -o main main.o lists.o

main.o: main.c lists.h
	$(CC) $(CFLAGS) -c main.c

lists.o: lists.c lists.h
	$(CC) $(CFLAGS) -c lists.c

.PHONY: clean

clean:
	rm -f *.o main