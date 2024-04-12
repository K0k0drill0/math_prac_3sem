#ifndef LEFTIST_HEAP_H
#define LEFTIST_HEAP_H

#include "../data_structs.h"

typedef struct leftist_node
{
    Application* req;
    int distance;
    struct leftist_node* left;
    struct leftist_node* right;
} leftist_node, Lft_node;

typedef struct Leftist_heap
{
    leftist_node* head;
    unsigned int size;
    int (*compare)(const Application*, const Application*);
} Leftist_heap, Lft_heap;

int Lft_heap_set_null(Lft_heap* Lft);
int Lft_heap_init(Lft_heap* Lft, int (*compare)(const Application*, const Application*));
int Lft_heap_copy(Lft_heap* Lft_dest, Lft_heap* Lft_src);
int Lft_heap_free(Lft_heap* Lft);

int Lft_heap_meld(Lft_heap* Lft_res, Lft_heap* Lft_l, Lft_heap* Lft_r);
int Lft_heap_copy_meld(Lft_heap* Lft_res, Lft_heap* Lft_l, Lft_heap* Lft_r);

int Lft_heap_size(Lft_heap* Lft, unsigned int* size);
int Lft_heap_get_max(Lft_heap* Lft, Application** req);
int Lft_heap_del_max(Lft_heap* Lft, Application** req);
int Lft_heap_insert(Lft_heap* Lft, Application* req);

#endif // _LEFTIST_HEAP_H_