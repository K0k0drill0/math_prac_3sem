#include <string.h>

#include "Binary_heap.h"
#include "../errors.h"

int Binary_heap_sift_down(Binary_heap* bh, unsigned int i)
{
    if (bh == NULL)
    {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    while (2 * i + 1 < bh->size)
    {
        int left_idx = 2 * i + 1;
        int right_idx = 2 * i + 2;
        int j = left_idx;
        if (right_idx < bh->size && (bh->compare(bh->elems[right_idx], bh->elems[left_idx]) < 0))
        {
            j = right_idx;
        }
        if (bh->compare(bh->elems[i], bh->elems[j]) < 0)
        {
            break;
        }
        Application* tmp = bh->elems[i];
        bh->elems[i] = bh->elems[j];
        bh->elems[j] = tmp;
        i = j;
    }
    
    return ok;
}

int Binary_heap_sift_up(Binary_heap* bh)
{
    if (bh == NULL)
    {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    unsigned i = bh->size;
    while (i > 0 && (bh->compare(bh->elems[(i-1) / 2], bh->elems[i]) > 0))
    {
        Application* tmp = bh->elems[i];
        bh->elems[i] = bh->elems[(i - 1) / 2];
        bh->elems[(i - 1) / 2] = tmp;
        i = (i - 1) / 2;
    }
    return ok;
}

int Binary_heap_set_null(Binary_heap* bh)
{
    if (bh == NULL)
    {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    bh->capacity = 0;
    bh->size = 0;
    bh->elems = NULL;
    bh->compare = NULL;
    
    return ok;
}

int Binary_heap_init(Binary_heap* bh, int (*compare)(const Application*, const Application*))
{
    if (bh == NULL)
    {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    bh->elems = (Application**) calloc(16, sizeof(Application*));
    if (bh->elems == NULL)
    {
        Binary_heap_free(bh);
        return MEMORY_ISSUES;
    }
    
    bh->compare = compare;
    bh->capacity = 16;
    bh->size = 0;
    
    return ok;
}

int Binary_heap_copy(Binary_heap* bh_dest, Binary_heap* bh_src)
{
    if (bh_dest == NULL || bh_src == NULL)
    {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    bh_dest->elems = (Application**) calloc(bh_src->capacity, sizeof(Application*));
    if (bh_dest->elems == NULL)
    {
        Binary_heap_free(bh_dest);
        return MEMORY_ISSUES;
    }
    
    bh_dest->size = 0;
    for (unsigned int i = 0; i < bh_src->size; ++i)
    {
        // bh_dest->elems[i] = (Application*) malloc(sizeof(Application));
        // if (bh_dest->elems[i] == NULL)
        // {
        //     Binary_heap_free(bh_dest);
        //     return MEMORY_ISSUES;
        // }
        
        int st = copy_application(&(bh_dest->elems[i]), bh_src->elems[i]);
        if (st)
        {
            //free(bh_dest->elems[i]);
            Binary_heap_free(bh_dest);
            return st;
        }
        bh_dest->size++;
    }
    
    bh_dest->size = bh_src->size;
    bh_dest->capacity = bh_src->capacity;
    bh_dest->compare = bh_src->compare;
    
    return ok;
}

int Binary_heap_free(Binary_heap* bh)
{
    if (bh == NULL)
    {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    for (unsigned int i = 0; i < bh->size; ++i)
    {
        free_application(bh->elems[i]);
        // free(bh->elems[i]);
    }
    free(bh->elems);
    Binary_heap_set_null(bh);
    
    return ok;
}

int Binary_heap_meld(Binary_heap* bh, Binary_heap* bh_l, Binary_heap* bh_r)
{
    if (bh == NULL || bh_l == NULL || bh_r == NULL)
    {
        return INVALID_FUNCTION_ARGUMENT;
    }
    if (bh_l == bh_r || bh_l->compare != bh_r->compare)
    {
        return INVALID_INPUT;
    }
    
    Binary_heap bh_tmp;
    unsigned int max_capacity = bh_l->capacity > bh_r->capacity ? bh_l->capacity : bh_r->capacity;
    
    if (bh_l->size + bh_r->size > max_capacity)
    {
        max_capacity *= 2;
    }
    
    bh_tmp.size = bh_l->size + bh_r->size;
    bh_tmp.capacity = max_capacity;
    bh_tmp.elems = (Application**) malloc(sizeof(Application*) * max_capacity);
    bh_tmp.compare = bh_l->compare;
    
    if (bh_tmp.elems == NULL)
    {
        return MEMORY_ISSUES;
    }
    
    for (unsigned int i = 0; i < bh_l->size; ++i)
    {
        bh_tmp.elems[i] = bh_l->elems[i];
    }
    for (unsigned int i = 0; i < bh_r->size; ++i)
    {
        bh_tmp.elems[i + bh_l->size] = bh_r->elems[i];
    }
    
    for (unsigned int i = 0; i <= bh_tmp.size / 2; ++i)
    {
        Binary_heap_sift_down(&bh_tmp, bh_tmp.size / 2 - i);
    }
    
    
    
    if (bh == bh_l)
    {
        bh_r->size = 0;
        //free(bh_r->elems);
        //bh_r->elems = NULL;
    }
    if (bh == bh_r)
    {
        bh_l->size = 0;
        //free(bh_r->elems);
        //bh_r->elems = NULL;
    }
    else {
        bh_l->size = 0;
        bh_r->size = 0;
    }
    *bh = bh_tmp;
    
    return ok;
}

int Binary_heap_copy_meld(Binary_heap* bh, Binary_heap* bh_l, Binary_heap* bh_r)
{
    if (bh == NULL || bh_l == NULL || bh_r == NULL)
    {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    Binary_heap bh_lc, bh_rc, bh_res;
    Binary_heap_set_null(&bh_lc);
    Binary_heap_set_null(&bh_rc);
    Binary_heap_set_null(&bh_res);
    
    int st = Binary_heap_copy(&bh_lc, bh_l);
    st = st ? st : Binary_heap_copy(&bh_rc, bh_r);
    st = st ? st : Binary_heap_meld(&bh_res, &bh_lc, &bh_rc);
    
    Binary_heap_free(&bh_rc);
    Binary_heap_free(&bh_lc);
    
    if (st)
    {
        Binary_heap_free(&bh_res);
        return st;
    }
    
    *bh = bh_res;
    
    return ok;
}

int Binary_heap_size(Binary_heap* bh, unsigned int* size)
{
    if (size == NULL)
    {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    *size = bh->size;
    
    return ok;
}

int Binary_heap_get_max(Binary_heap* bh, Application** req)
{
    if (bh == NULL || req == NULL)
    {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    if (bh->size == 0)
    {
        *req = NULL;
        return ok;
    }
    
    *req = bh->elems[0];
    
    return ok;
}

int Binary_heap_del_max(Binary_heap* bh, Application** req)
{
    if (bh == NULL || req == NULL)
    {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    if (bh->size == 0)
    {
        *req = NULL;
        return ok;
    }
    
    *req = bh->elems[0];
    
    bh->elems[0] = bh->elems[bh->size-1];
    bh->size--;
    Binary_heap_sift_down(bh, 0);
    
    return ok;
}

int Binary_heap_insert(Binary_heap* bh, Application* req)
{
    if (bh == NULL || req == NULL)
    {
        return INVALID_FUNCTION_ARGUMENT;
    }
    if (bh->elems == NULL)
    {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    if (bh->size == bh->capacity)
    {
        Application** tmp = (Application**) (bh->elems, sizeof(Application*) * (bh->capacity * 2));
        if (tmp == NULL)
        {
            return MEMORY_ISSUES;
        }
        bh->capacity *= 2;
        bh->elems = tmp;
    }
    
    bh->elems[bh->size] = req;
    Binary_heap_sift_up(bh);
    bh->size++;
    
    return ok;
}