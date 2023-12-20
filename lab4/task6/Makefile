CC = gcc
CFLAGS = -g -fdiagnostics-color=always

main: Tree.o arithmetic.o main.o 
	$(CC) $(CFLAGS) -o main main.o Tree.o arithmetic.o 

main.o: main.c Tree.h arithmetic.h
	$(CC) $(CFLAGS) -c main.c

Tree.o: Tree.c Tree.h arithmetic.h
	$(CC) $(CFLAGS) -c Tree.c

arithmetic.o: arithmetic.c arithmetic.h
	$(CC) $(CFLAGS) -c arithmetic.c


.PHONY: clean

clean:
	rm -f *.o main