laba: String.o Validations.o main.o
	gcc String.o Validations.o main.o -o main
String.o : String.h String.c
	gcc -c String.c -g
Validations.o: Validations.h Validations.c
	gcc -c Validations.c -g
main.o : String.h Validations.h main.c
	gcc -c main.c -g