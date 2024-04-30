#ifndef _BINARY_HEAP_H_
#define _BINARY_HEAP_H_

#include <stdlib.h>

#include "../data_structs.h"

typedef struct Binary_heap
{
    unsigned int size;
    unsigned int capacity;
    Application** elems;
    int (*compare)(const Application*, const Application*);
} Binary_heap;

int Binary_heap_set_null(Binary_heap* bh);
int Binary_heap_init(Binary_heap* bh, int (*compare)(const Application*, const Application*));
int Binary_heap_copy(Binary_heap* bh_dest, Binary_heap* bh_src);
int Binary_heap_free(Binary_heap* bh);

int Binary_heap_meld(Binary_heap* bh, Binary_heap* bh_l, Binary_heap* bh_r);
int Binary_heap_copy_meld(Binary_heap* bh, Binary_heap* bh_l, Binary_heap* bh_r);

int Binary_heap_size(Binary_heap* bh, unsigned int* size);
int Binary_heap_get_max(Binary_heap* bh, Application** req);
int Binary_heap_del_max(Binary_heap* bh, Application** req);
int Binary_heap_insert(Binary_heap* bh, Application* req);

#endif //_BINARY_HEAP_H_