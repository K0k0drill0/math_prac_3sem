#include <stdlib.h>
#include <string.h>

#include "Binomial_heap.h"
#include "../errors.h"

typedef long long ll;

void Bmh_node_free(Bmh_node* node) {
    if (node == NULL) {
        return;
    }
    
    Bmh_node_free(node->son);
    Bmh_node_free(node->brother);
    free_application(node->req);
    free(node);
    //free_all(2, node->req, node);
}

int Bmh_node_copy(Bmh_node** node_dest, Bmh_node* node_src) {
    if (node_dest == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    if (node_src == NULL) {
        *node_dest = NULL;
        return ok;
    }
    
    *node_dest = NULL;
    Bmh_node* tmp_node = (Bmh_node*) malloc(sizeof(Bmh_node));
    if (tmp_node == NULL) {
        return MEMORY_ISSUES;
    }
    
    // tmp_node->req = (Application*) malloc(sizeof(Application));
    // if (tmp_node->req == NULL) {
    //     free(tmp_node);
    //     return MEMORY_ISSUES;
    // }
    
    int code = copy_application(&(tmp_node->req), node_src->req);
    if (code) {
        free(tmp_node);
        return code;
    }
    
    tmp_node->son = NULL;
    tmp_node->brother = NULL;
    tmp_node->rank = node_src->rank;
    
    code = code ? code : Bmh_node_copy(&tmp_node->son, node_src->son);
    code = code ? code : Bmh_node_copy(&tmp_node->brother, node_src->brother);
    if (code) {
        Bmh_node_free(tmp_node);
        return code;
    }
    
    *node_dest = tmp_node;
    
    return ok;
}

int Bm_heap_set_null(Bm_heap* Bmh) {
    if (Bmh == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    Bmh->head = NULL;
    Bmh->size = 0;
    Bmh->compare = NULL;
    
    return ok;
}

int Bm_heap_init(Bm_heap* Bmh, int (*compare)(const Application*, const Application*)) {
    if (Bmh == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    Bmh->head = NULL;
    Bmh->size = 0;
    Bmh->compare = compare;
    
    return ok;
}

int Bm_heap_copy(Bm_heap* Bmh_dest, Bm_heap* Bmh_src) {
    if (Bmh_dest == NULL || Bmh_src == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    Bmh_dest->size = Bmh_src->size;
    Bmh_dest->compare = Bmh_src->compare;
    
    int code = Bmh_node_copy(&Bmh_dest->head, Bmh_src->head);
    if (code) {
        Bm_heap_free(Bmh_dest);
        return code;
    }
    
    return ok;
}

int Bm_heap_free(Bm_heap* Bmh) {
    if (Bmh == NULL) {
        return ok;
    }
    
    Bmh_node_free(Bmh->head);
    Bm_heap_set_null(Bmh);
    
    return ok;
}

int Bm_heap_meld(Bm_heap* Bmh_res, Bm_heap* Bmh_l, Bm_heap* Bmh_r) {
    if (Bmh_res == NULL || Bmh_l == NULL || Bmh_r == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    if (Bmh_l == Bmh_r || Bmh_l->compare != Bmh_r->compare) {
        return INVALID_INPUT;
    }
    
    // STEP 1: GATHERING UP;
    Bm_heap Bmh_tmp;
    Bmh_node* iter_l = Bmh_l->head;
    Bmh_node* iter_r = Bmh_r->head;
    
    if (iter_l == NULL && iter_r == NULL) {
        Bmh_res->head = NULL;
        Bmh_res->size = 0;
        Bmh_res->compare = Bmh_l->compare;
        return ok;
    }
    
    if (iter_l != NULL && (iter_r == NULL || iter_l->rank < iter_r->rank)) {
        Bmh_tmp.head = iter_l;
        iter_l = iter_l->brother;
    }
    else {
        Bmh_tmp.head = iter_r;
        iter_r = iter_r->brother;
    }
    
    Bmh_node* iter_res = Bmh_tmp.head;
    while (iter_l != NULL && iter_r != NULL) {
        if (iter_l->rank < iter_r->rank)
        {
            iter_res->brother = iter_l;
            iter_l = iter_l->brother;
        }
        else
        {
            iter_res->brother = iter_r;
            iter_r = iter_r->brother;
        }
        iter_res = iter_res->brother;
    }
    
    while (iter_l != NULL) {
        iter_res->brother = iter_l;
        iter_l = iter_l->brother;
        iter_res = iter_res->brother;
    }
    
    while (iter_r != NULL) {
        iter_res->brother = iter_r;
        iter_r = iter_r->brother;
        iter_res = iter_res->brother;
    }
    
    // STEP 2: COMBINING
    
    Bmh_node* prev_node = NULL;
    Bmh_node* cur_node = Bmh_tmp.head;
    Bmh_node* nxt_node = Bmh_tmp.head->brother;
    
    while (nxt_node != NULL) {
        Bmh_node* tmp_node = nxt_node->brother;
        if (cur_node->rank == nxt_node->rank && (tmp_node == NULL || tmp_node->rank != nxt_node->rank))
        {
            if (Bmh_l->compare(cur_node->req, nxt_node->req) <= 0)
            {
                // cur > nxt -> "no swap"
                nxt_node->brother = cur_node->son;
                cur_node->son = nxt_node;
            }
            else
            {
                // nxt > cur -> "swap"
                cur_node->brother = nxt_node->son;
                nxt_node->son = cur_node;
                
                if (cur_node == Bmh_tmp.head)
                {
                    Bmh_tmp.head = nxt_node;
                }
                cur_node = nxt_node;
            }
            if (prev_node != NULL)
            {
                prev_node->brother = cur_node;
            }
            cur_node->rank++;
            cur_node->brother = tmp_node;
            nxt_node = tmp_node;
        }
        else
        {
            prev_node = cur_node;
            cur_node = nxt_node;
            nxt_node = nxt_node->brother;
        }
    }
    
    size_t size = Bmh_l->size + Bmh_r->size;
    Bmh_l->head = Bmh_r->head = NULL;
    Bmh_l->size = Bmh_r->size = 0;
    Bmh_res->head = Bmh_tmp.head;
    Bmh_res->size = size;
    Bmh_res->compare = Bmh_l->compare;
    
    return ok;
}

int Bm_heap_copy_meld(Bm_heap* Bmh_res, Bm_heap* Bmh_l, Bm_heap* Bmh_r) {
    if (Bmh_res == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    if (Bmh_res == Bmh_l || Bmh_res == Bmh_l) {
        return INVALID_INPUT;
    }
    
    int code = ok;
    Bm_heap Bmh_lc, Bmh_rc;
        
    Bm_heap_set_null(&Bmh_lc);
    Bm_heap_set_null(&Bmh_rc);
    Bm_heap_set_null(Bmh_res);
    
    code = code ? code : Bm_heap_copy(&Bmh_lc, Bmh_l);
    code = code ? code : Bm_heap_copy(&Bmh_rc, Bmh_r);
    code = code ? code : Bm_heap_meld(Bmh_res, &Bmh_lc, &Bmh_rc);
    
    Bm_heap_free(&Bmh_lc);
    Bm_heap_free(&Bmh_rc);
    
    if (code) {
        Bm_heap_free(Bmh_res);
        return code;
    }
    
    return ok;
}

int Bm_heap_size(Bm_heap* Bmh, size_t* size) {
    if (Bmh == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    *size = Bmh->size;
    return ok;
}

int Bm_heap_find(Bm_heap* Bmh, Bmh_node** prev, Bmh_node** node) {
    // bad O(logn) alg 
    if (node == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    if (Bmh->head == NULL) {
        *node = NULL;
        return ok;
    }
    
    Bmh_node* prev_node = NULL;
    Bmh_node* target_node = Bmh->head;
    Bmh_node* cur_node = Bmh->head;
    
    while (cur_node->brother != NULL) {
        if (Bmh->compare(cur_node->brother->req, target_node->req) < 0)
        {
            prev_node = cur_node;
            target_node = cur_node->brother;
        }
        cur_node = cur_node->brother;
    }
    
    if (prev != NULL) {
        *prev = prev_node;
    }
    *node = target_node;
    
    return ok;
}

int Bm_heap_get_max(Bm_heap* Bmh, Application** req) {
    if (Bmh == NULL || req == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    Bmh_node* node = NULL;
    Bm_heap_find(Bmh, NULL, &node);
    if (node == NULL) {
        *req = NULL;
        return ok;
    }
    
    *req = node->req;
    
    return ok;
}

int Bm_heap_del_max(Bm_heap* Bmh, Application** req) {
    if (Bmh == NULL || req == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    Bmh_node* prev = NULL;
    Bmh_node* node = NULL;
    Bm_heap_find(Bmh, &prev, &node);
    if (node == NULL) {
        if (req != NULL)
        {
            *req = NULL;
        }
        return ok;
    }
    
    ll tmp_size = 0;
    int code = bpow_safely(2, node->rank, &tmp_size);
    size_t node_size = (size_t) tmp_size;
    if (code)  {
        return code;
    }
    
    if (req != NULL) {
        *req = node->req;
    }
    if (prev != NULL) {
        prev->brother = node->brother;
    }
    if (node == Bmh->head) {
        Bmh->head = node->brother;
    }
    
    Bm_heap Bmh_add;
    Bmh_add.head = node->son;
    Bmh_add.compare = Bmh->compare;
    //free_application(node->req);
    free(node);
    
    // reverse add Bmh
    prev = NULL;
    node = Bmh_add.head;
    while (node != NULL) {
        Bmh_node* nxt = node->brother;
        node->brother = prev;
        prev = node;
        node = nxt;
    }
    Bmh_add.head = prev;
    
    Bmh->size -= node_size;
    Bmh_add.size = node_size - 1;
    Bm_heap_meld(Bmh, Bmh, &Bmh_add);
    
    return ok;
}

int Bm_heap_insert(Bm_heap* Bmh, Application* req) {
    if (Bmh == NULL || req == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    Bmh_node* node = (Bmh_node*) malloc(sizeof(Bmh_node));
    if (node == NULL) {
        return MEMORY_ISSUES;
    }
    
    node->req = req;
    node->son = NULL;
    node->brother = NULL;
    node->rank = 0;
    
    Bm_heap Bmh_add;
    Bmh_add.head = node;
    Bmh_add.size = 1;
    Bmh_add.compare = Bmh->compare;
    Bm_heap_meld(Bmh, Bmh, &Bmh_add);
    
    return ok;
}