#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

#include "../data_structs.h"

typedef struct Binary_heap {
    Application_arr heap;
    
    int (*comparator)(const Application*, const Application*);
} Binary_heap;

int Binary_heap_set_null(Binary_heap* bh);
int Binary_heap_init(Binary_heap* bh,
 int (*comparator)(const Application*, const Application*)) ;

int Binary_heap_copy(Binary_heap* dest, Binary_heap* source);

// int Binary_heap_sift_down(Binary_heap bh, int i);
// int Binary_heap_sift_up(Binary_heap bh, int i);

int Binary_heap_insert(Binary_heap* bh, Application* a);

int Binary_heap_get_max(Binary_heap* bh, Application** res) ;
int Binary_heap_del_max(Binary_heap* bh, Application** res);

int Binary_heap_merge_no_destruction(Binary_heap* res, Binary_heap* bh1, Binary_heap* bh2);
int Binary_heap_merge_with_destruction(Binary_heap* res, Binary_heap* bh1, Binary_heap *bh2);

void Binary_heap_print(Application_arr heap, int index, int level);

int Binary_heap_free(Binary_heap* bh);

int Binary_heap_size(Binary_heap* bh, unsigned int* res);

#endif