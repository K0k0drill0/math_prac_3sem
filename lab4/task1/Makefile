CC = gcc
CFLAGS = -g -fdiagnostics-color=always

main: list.o cache.o hash_table.o main.o 
	$(CC) $(CFLAGS) -o main main.o list.o cache.o hash_table.o

main.o: main.c ultra_header.h
	$(CC) $(CFLAGS) -c main.c

list.o: list.c ultra_header.h
	$(CC) $(CFLAGS) -c list.c

cache.o: cache.c ultra_header.h
	$(CC) $(CFLAGS) -c cache.c

hash_table.o: hash_table.c ultra_header.h
	$(CC) $(CFLAGS) -c hash_table.c

.PHONY: clean

clean:
	rm -f *.o main
