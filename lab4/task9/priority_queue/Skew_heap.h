#ifndef _SKEW_HEAP_H_
#define _SKEW_HEAP_H_

#include "../data_structs.h"

typedef struct skew_node
{
    Application* req;
    struct skew_node* left;
    struct skew_node* right;
} skew_node, Skw_node;

typedef struct Skew_heap
{
    skew_node* head;
    unsigned int size;
    int (*compare)(const Application*, const Application*);
} Skew_heap, Skw_heap;

int Skw_heap_set_null(Skw_heap* Skw);
int Skw_heap_init(Skw_heap* Skw, int (*compare)(const Application*, const Application*));
int Skw_heap_copy(Skw_heap* Skw_dest, Skw_heap* Skw_src);
int Skw_heap_free(Skw_heap* Skw);

int Skw_heap_meld(Skw_heap* Skw_res, Skw_heap* Skw_l, Skw_heap* Skw_r);
int Skw_heap_copy_meld(Skw_heap* Skw_res, Skw_heap* Skw_l, Skw_heap* Skw_r);

int Skw_heap_size(Skw_heap* Skw, unsigned int* size);
int Skw_heap_get_max(Skw_heap* Skw, Application** req);
int Skw_heap_del_max(Skw_heap* Skw, Application** req);
int Skw_heap_insert(Skw_heap* Skw, Application* req);

#endif // _SKEW_HEAP_H_