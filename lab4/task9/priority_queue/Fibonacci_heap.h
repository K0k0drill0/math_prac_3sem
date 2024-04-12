#ifndef FIBONACCI_HEAP_H
#define FIBONACCI_HEAP_H

#include "../data_structs.h"

typedef struct Fib_node {
	Application* req;
	unsigned int rank;

    struct Fib_node *child;
    struct Fib_node *left;
	struct Fib_node *right;
} Fib_node;

typedef struct Fibonacci_heap
{
	Fib_node* head;
	unsigned int size;
	int (*compare)(const Application*, const Application*);
} Fibonacci_heap, Fib_heap;


int Fib_heap_set_null(Fib_heap* heap);
int Fib_heap_init(Fib_heap* heap, int (*compare)(const Application*, const Application*));
int Fib_heap_copy(Fib_heap* dest, Fib_heap* src);
int Fib_heap_free(Fib_heap* heap);

int Fib_heap_meld(Fib_heap* res, Fib_heap* heap_l, Fib_heap* heap_r);
int Fib_heap_copy_meld(Fib_heap* res, Fib_heap* heap_l, Fib_heap* heap_r);

int Fib_heap_size(Fib_heap* heap, unsigned int* size);
int Fib_heap_get_max(Fib_heap* heap, Application** req);
int Fib_heap_del_max(Fib_heap* heap, Application** req);
int Fib_heap_insert(Fib_heap* heap, Application* req);

#endif // _FIBONACCI_HEAP_H_