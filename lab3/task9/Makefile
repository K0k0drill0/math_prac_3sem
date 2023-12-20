CC = gcc
CFLAGS = -g -fdiagnostics-color=always

main: Tree.o  main.o 
	$(CC) $(CFLAGS) -o main main.o Tree.o  

main.o: main.c Tree.h 
	$(CC) $(CFLAGS) -c main.c

Tree.o: Tree.c Tree.h 
	$(CC) $(CFLAGS) -c Tree.c


.PHONY: clean

clean:
	rm -f *.o main