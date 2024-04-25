CC = gcc
CFLAGS = -g -fdiagnostics-color=always

PQ_H = priority_queue/priority_queue.h priority_queue/Binary_heap.h priority_queue/Treap.h priority_queue/Binomial_heap.h priority_queue/Fibonacci_heap.h priority_queue/Leftist_heap.h priority_queue/Skew_heap.h
PQ_C = priority_queue/priority_queue.c priority_queue/Binary_heap.c priority_queue/Treap.c priority_queue/Binomial_heap.c priority_queue/Fibonacci_heap.c priority_queue/Leftist_heap.c priority_queue/Skew_heap.c
PQ_O = priority_queue.o Binary_heap.o Treap.o Binomial_heap.o Fibonacci_heap.o Leftist_heap.o Skew_heap.o

MAP_H = map/map.h map/Dynamic_array.h map/Trie.h map/Hash_table.h map/Binary_search_tree.h
MAP_C = map/map.c map/Dynamic_array.c map/Trie.c map/Hash_table.c map/Binary_search_tree.c
MAP_O = map.o Dynamic_array.o Trie.o Hash_table.o Binary_search_tree.o

UT_H = data_structs.h errors.h 
UT_C = data_structs.c errors.c
UT_O = data_structs.o errors.o

main: main.o $(UT_O) $(PQ_O) $(MAP_O) Department.o readers.o validators.o logger.o
	$(CC) $(CFLAGS) -o main main.o $(UT_O) $(PQ_O) $(MAP_O) Department.o readers.o validators.o logger.o -lm

main.o: main.c $(UT_H) $(MAP_H) priority_queue/priority_queue.h map/map.h Department.h readers.h validators.h logger.h
	$(CC) $(CFLAGS) -c main.c

data_structs.o: data_structs.c data_structs.h
	$(CC) $(CFLAGS) -c data_structs.c

errors.o: errors.c errors.h
	$(CC) $(CFLAGS) -c errors.c

validators.o: validators.c validators.h $(UT_H) priority_queue/priority_queue.h map/map.h 
	$(CC) $(CFLAGS) -c validators.c

readers.o: readers.c readers.h validators.h $(UT_H) priority_queue/priority_queue.h map/map.h 
	$(CC) $(CFLAGS) -c readers.c

#___________________PRIORITY_QUEUE_______________________

Treap.o: priority_queue/Treap.c priority_queue/Treap.h $(UT_H)
	$(CC) $(CFLAGS) -c priority_queue/Treap.c

Binary_heap.o: priority_queue/Binary_heap.c priority_queue/Binary_heap.h $(UT_H)
	$(CC) $(CFLAGS) -c priority_queue/Binary_heap.c

Binomial_heap.o: priority_queue/Binomial_heap.c priority_queue/Binomial_heap.h $(UT_H)
	$(CC) $(CFLAGS) -c priority_queue/Binomial_heap.c	

Fibonacci_heap.o: priority_queue/Fibonacci_heap.c priority_queue/Fibonacci_heap.h $(UT_H)
	$(CC) $(CFLAGS) -c priority_queue/Fibonacci_heap.c -lm

Leftist_heap.o: priority_queue/Leftist_heap.c priority_queue/Leftist_heap.h $(UT_H)
	$(CC) $(CFLAGS) -c priority_queue/Leftist_heap.c

Skew_heap.o: priority_queue/Skew_heap.c priority_queue/Skew_heap.h $(UT_H)
	$(CC) $(CFLAGS) -c priority_queue/Skew_heap.c

priority_queue.o: priority_queue/priority_queue.c $(UT_H) $(PQ_H)
	$(CC) $(CFLAGS) -c priority_queue/priority_queue.c -lm

#_____________________________MAP_______________________

map.o: map/map.c $(UT_H) $(MAP_H) 
	$(CC) $(CFLAGS) -c map/map.c

Department.o: Department.c Department.h $(PQ_H) $(UT_H) 
	$(CC) $(CFLAGS) -c Department.c

Dynamic_array.o: map/Dynamic_array.c map/Dynamic_array.h $(UT_H) Department.h
	$(CC) $(CFLAGS) -c map/Dynamic_array.c

Trie.o: map/Trie.c map/Trie.h $(UT_H) Department.h
	$(CC) $(CFLAGS) -c map/Trie.c

Hash_table.o: map/Hash_table.c map/Hash_table.h $(UT_H) Department.h 
	$(CC) $(CFLAGS) -c map/Hash_table.c

Binary_search_tree.o: map/Binary_search_tree.c map/Binary_search_tree.h $(UT_H) Department.h
	$(CC) $(CFLAGS) -c map/Binary_search_tree.c


#____________logger___________

logger.o: logger.c errors.h data_structs.h
	$(CC) $(CFLAGS) -c logger.c

.PHONY: clean

clean:
	rm -f *.o main