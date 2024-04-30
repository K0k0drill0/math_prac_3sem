#ifndef _BINOMIAL_HEAP_H_
#define _BINOMIAL_HEAP_H_

#include "../data_structs.h"

typedef struct Bmh_node
{
    Application* req;
    unsigned rank;
    struct Bmh_node* son;
    struct Bmh_node* brother;
} Bmh_node;

typedef struct Binomial_heap
{
    Bmh_node* head;
    size_t size;
    int (*compare)(const Application*, const Application*);
} Binomial_heap, Bm_heap;


int Bm_heap_set_null(Bm_heap* Bmh);
int Bm_heap_init(Bm_heap* Bmh, int (*compare)(const Application*, const Application*));
int Bm_heap_copy(Bm_heap* Bmh_dest, Bm_heap* Bmh_src);
int Bm_heap_free(Bm_heap* Bmh);

int Bm_heap_meld(Bm_heap* Bmh_res, Bm_heap* Bmh_l, Bm_heap* Bmh_r);
int Bm_heap_copy_meld(Bm_heap* Bmh_res, Bm_heap* Bmh_l, Bm_heap* Bmh_r);

int Bm_heap_size(Bm_heap* Bmh, unsigned int* size);
int Bm_heap_get_max(Bm_heap* Bmh, Application** req);
int Bm_heap_del_max(Bm_heap* Bmh, Application** req);
int Bm_heap_insert(Bm_heap* Bmh, Application* req);

#endif // _BINOMIAL_HEAP_H_