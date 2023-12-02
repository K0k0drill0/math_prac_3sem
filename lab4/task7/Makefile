CC = gcc
CFLAGS = -g -fdiagnostics-color=always

main: memory.o calculations.o main.o 
	$(CC) $(CFLAGS) -o main main.o memory.o calculations.o

main.o: main.c universal_header.h
	$(CC) $(CFLAGS) -c main.c

memory.o: memory.c universal_header.h
	$(CC) $(CFLAGS) -c memory.c
	
calculations.o: calculations.c universal_header.h
	$(CC) $(CFLAGS) -c calculations.c

.PHONY: clean

clean:
	rm -f *.o main
