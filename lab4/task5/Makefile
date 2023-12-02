CC = gcc
CFLAGS = -g -fdiagnostics-color=always

main: stack.o arr.o main.o 
	$(CC) $(CFLAGS) -o main main.o stack.o arr.o

main.o: main.c stack.h arr.h
	$(CC) $(CFLAGS) -c main.c

stack.o: stack.c stack.h
	$(CC) $(CFLAGS) -c stack.c

arr.o: arr.c arr.h
	$(CC) $(CFLAGS) -c arr.c

.PHONY: clean

clean:
	rm -f *.o main