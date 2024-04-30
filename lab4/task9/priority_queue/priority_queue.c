#include <stdio.h>
#include <stdlib.h>

#include "priority_queue.h"

#include "Treap.h"
#include "Binary_heap.h"
#include "Binomial_heap.h"
#include "Fibonacci_heap.h"
#include "Leftist_heap.h"
#include "Skew_heap.h"

int Priority_queue_set_null(Priority_queue* pq) {
    if (pq == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    pq->ds = NULL;
    pq->set_null = NULL;
    pq->init = NULL;
    pq->copy = NULL;
    pq->free = NULL;
    pq->meld = NULL;
    pq->copy_meld = NULL;
    pq->get_max = NULL;
    pq->del_max = NULL;
    pq->insert = NULL;

    return ok;
}

int Priority_queue_init(Priority_queue* pq, Priority_queue_type t, int (*comparator) (const Application*, const Application*)) {
    if (pq == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    pq->type = t;
    pq->ds = NULL;

    switch (t) {
        case PQ_BINARY: 
        {
            pq->ds_size = sizeof(Binary_heap);
            pq->ds = malloc(sizeof(Binary_heap));
            
            pq->set_null 	= (int (*)(void*)) 											Binary_heap_set_null;
            pq->init 	= (int (*)(void*, int (*)(const Application*, const Application*))) Binary_heap_init;
            pq->copy 		= (int (*)(void*, void*)) 							Binary_heap_copy;
            pq->free 	= (int (*)(void*)) 											    Binary_heap_free;
            pq->meld 		= (int (*)(void*, void*, void*)) 							Binary_heap_meld;
            pq->copy_meld 	= (int (*)(void*, void*, void*))                Binary_heap_copy_meld;
            pq->size 		= (int (*)(void*, unsigned int*))                                 Binary_heap_size;
            pq->get_max 		= (int (*)(void*, Application**))                       Binary_heap_get_max;
            pq->del_max 		= (int (*)(void*, Application**))                       Binary_heap_del_max;
            pq->insert 		= (int (*)(void*, Application*))                      Binary_heap_insert;
            break;
        }
        case PQ_LEFTIST:
        {
            pq->ds_size = sizeof(Leftist_heap);
            pq->ds = malloc(sizeof(Leftist_heap));
            
            pq->set_null 	= (int (*)(void*)) 											Lft_heap_set_null;
            pq->init 	= (int (*)(void*, int (*)(const Application*, const Application*))) Lft_heap_init;
            pq->copy 		= (int (*)(void*, void*)) 							Lft_heap_copy;
            pq->free 	= (int (*)(void*)) 											Lft_heap_free;
            pq->meld 		= (int (*)(void*, void*, void*)) 							Lft_heap_meld;
            pq->copy_meld 	= (int (*)(void*, void*, void*))                Lft_heap_copy_meld;
            pq->size 		= (int (*)(void*, unsigned int*))                                 Lft_heap_size;
            pq->get_max 		= (int (*)(void*, Application**))                               Lft_heap_get_max;
            pq->del_max 		= (int (*)(void*, Application**))                               Lft_heap_del_max;
            pq->insert 		= (int (*)(void*, Application*))                          Lft_heap_insert;
            break;
        }
        case PQ_SKEW:
        {
            pq->ds_size = sizeof(Skew_heap);
            pq->ds = malloc(sizeof(Skew_heap));
            
            pq->set_null 	= (int (*)(void*)) 											Skw_heap_set_null;
            pq->init 	= (int (*)(void*, int (*)(const Application*, const Application*))) Skw_heap_init;
            pq->copy 		= (int (*)(void*, void*)) 							Skw_heap_copy;
            pq->free 	= (int (*)(void*)) 											Skw_heap_free;
            pq->meld 		= (int (*)(void*, void*, void*)) 							Skw_heap_meld;
            pq->copy_meld 	= (int (*)(void*, void*, void*))                Skw_heap_copy_meld;
            pq->size 		= (int (*)(void*, unsigned int*))                                 Skw_heap_size;
            pq->get_max 		= (int (*)(void*, Application**))                               Skw_heap_get_max;
            pq->del_max 		= (int (*)(void*, Application**))                               Skw_heap_del_max;
            pq->insert 		= (int (*)(void*, Application*))                          Skw_heap_insert;
            break;
        }
        case PQ_BINOMIAL:
        {
            pq->ds_size = sizeof(Binomial_heap);
            pq->ds = malloc(sizeof(Binomial_heap));
            
            pq->set_null    = (int (*)(void*))                                          Bm_heap_set_null;
            pq->init   = (int (*)(void*, int (*)(const Application*, const Application*))) Bm_heap_init;
            pq->copy        = (int (*)(void*, void*))                             Bm_heap_copy;
            pq->free    = (int (*)(void*))                                          Bm_heap_free;
            pq->meld        = (int (*)(void*, void*, void*))                            Bm_heap_meld;
            pq->copy_meld   = (int (*)(void*, void*, void*))                Bm_heap_copy_meld;
            pq->size        = (int (*)(void*, unsigned int*))                                 Bm_heap_size;
            pq->get_max         = (int (*)(void*, Application**))                               Bm_heap_get_max;
            pq->del_max         = (int (*)(void*, Application**))                               Bm_heap_del_max;
            pq->insert      = (int (*)(void*, Application*))                          Bm_heap_insert;
            break;
        }
        case PQ_FIBONACCI:
        {
            pq->ds_size = sizeof(Fibonacci_heap);
            pq->ds = malloc(sizeof(Fibonacci_heap));
            
            pq->set_null    = (int (*)(void*))                                          Fib_heap_set_null;
            pq->init   = (int (*)(void*, int (*)(const Application*, const Application*))) Fib_heap_init;
            pq->copy        = (int (*)(void*, void*))                             Fib_heap_copy;
            pq->free    = (int (*)(void*))                                          Fib_heap_free;
            pq->meld        = (int (*)(void*, void*, void*))                            Fib_heap_meld;
            pq->copy_meld   = (int (*)(void*, void*, void*))                Fib_heap_copy_meld;
            pq->size        = (int (*)(void*, unsigned int*))                                 Fib_heap_size;
            pq->get_max         = (int (*)(void*, Application**))                               Fib_heap_get_max;
            pq->del_max         = (int (*)(void*, Application**))                               Fib_heap_del_max;
            pq->insert      = (int (*)(void*, Application*))                          Fib_heap_insert;
            break;
        }
        case PQ_TREAP:
        {
            pq->ds_size = sizeof(Treap);
            pq->ds = malloc(sizeof(Treap));
            
            pq->set_null    = (int (*)(void*))                                          Treap_set_null;
            pq->init   = (int (*)(void*, int (*)(const Application*, const Application*))) Treap_init;
            pq->copy        = (int (*)(void*, void*))                             Treap_copy;
            pq->free    = (int (*)(void*))                                          Treap_destroy;
            pq->meld        = (int (*)(void*, void*, void*))                            Treap_meld;
            pq->copy_meld   = (int (*)(void*, void*, void*))                Treap_copy_meld;
            pq->size        = (int (*)(void*, unsigned int*))                                 Treap_size;
            pq->get_max         = (int (*)(void*, Application**))                               Treap_get_max;
            pq->del_max         = (int (*)(void*, Application**))                               Treap_del_max;
            pq->insert      = (int (*)(void*, Application*))                          Treap_insert;
            break;
        }
    }


    if (pq->ds == NULL) {
        Priority_queue_set_null(pq);
        return MEMORY_ISSUES;
    }

    pq->set_null(pq->ds);
    int st = pq->init(pq->ds, comparator);

    if (st != ok) {
        free(pq->ds);
        Priority_queue_set_null(pq);
        return st;
    }

    return ok;
}

int Priority_queue_copy(Priority_queue* dest, Priority_queue* source)
{
    if (dest == NULL || source == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    int st = ok;
    
    *dest = *source;
    
    dest->ds = malloc(source->ds_size);
    if (dest-> ds == NULL) {
        st = MEMORY_ISSUES;
    }
    
    st = source->copy(dest->ds, source->ds);
    
    if (st != ok) {
        Priority_queue_free(dest);
        return st;
    }
    
    return ok;
}

int Priority_queue_free(Priority_queue* pq)
{
    if (pq == NULL) {
        return ok;
    }
    if (pq->ds != NULL) {
        pq->free(pq->ds);
        free(pq->ds);
    }
    Priority_queue_set_null(pq);
    return ok;
}


int Priority_queue_meld(Priority_queue* res, Priority_queue* l, Priority_queue* r)
{
    if (res == NULL || l == NULL || r == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    if (l->type != r->type) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    int st = ok;
    Priority_queue tmp;
    
    if (res == l || res == r) {
        tmp = res == l ? *l : *r;
    }
    else {
        tmp = *l;
        tmp.ds = malloc(l->ds_size);
        if (tmp.ds == NULL) {
            return MEMORY_ISSUES;
        }
    }
    
    st = l->meld(tmp.ds, l->ds, r->ds);
    
    if (st != ok) {
        if (res != l && res != r) {
            Priority_queue_free(&tmp);
        }
        return st;
    }
    
    *res = tmp;
    // free(l->ds);
    // free(r->ds);
    // l->ds = NULL;
    // r->ds = NULL;
    return ok;
}

int Priority_queue_copy_meld(Priority_queue* res, Priority_queue* l, Priority_queue* r)
{
    if (res == NULL || l == NULL || r == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    if (l->type != r->type) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    int st = ok;
    Priority_queue tmp = *l;
    
    tmp.ds = malloc(l->ds_size);
    if (tmp.ds == NULL) {
        return MEMORY_ISSUES;
    }	
    
    st = l->copy_meld(tmp.ds, l->ds, r->ds);
    if (st != ok) {
        Priority_queue_free(&tmp);
        return st;
    }
    
    *res = tmp;
    return ok;
}


int Priority_queue_size(Priority_queue* pq, unsigned int* size)
{
    if (pq == NULL || size == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    return pq->size(pq->ds, size);
}

int Priority_queue_get_max(Priority_queue* pq, Application** a)
{
    if (pq == NULL || a == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    return pq->get_max(pq->ds, a);
}

int Priority_queue_del_max(Priority_queue* pq, Application** a)
{
    if (pq == NULL || a == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    return pq->del_max(pq->ds, a);
}

int Priority_queue_insert(Priority_queue* pq, Application* a)
{
    if (pq == NULL || a == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    return pq->insert(pq->ds, a);
}
