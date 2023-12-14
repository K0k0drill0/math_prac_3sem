CC = gcc
CFLAGS = -g -fdiagnostics-color=always

main: Liver.o validators.o Undo.o main.o 
	$(CC) $(CFLAGS) -o main main.o Liver.o validators.o Undo.o

main.o: main.c Liver.h validators.h
	$(CC) $(CFLAGS) -c main.c

Liver.o: Liver.c Liver.h validators.h
	$(CC) $(CFLAGS) -c Liver.c

validators.o: validators.c validators.h
	$(CC) $(CFLAGS) -c validators.c

Undo.o: Undo.c Liver.h validators.h Undo.h
	$(CC) $(CFLAGS) -c Undo.c


.PHONY: clean

clean:
	rm -f *.o main