#include <stdlib.h>
#include <string.h>

#include "Leftist_heap.h"
#include "../errors.h"

void Lft_node_free(Lft_node* node) {
    if (node == NULL) {
        return;
    }
        
    Lft_node_free(node->left);
    Lft_node_free(node->right);
    
    free_application(node->req);
    free(node);
    //free_all(2, node->req, node);
}

int Lft_node_copy(Lft_node** result, Lft_node* src) {
    if (result == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    if (src == NULL) {
        *result = NULL;
        return ok;
    }
    
    Lft_node* node = (Lft_node*) malloc(sizeof(Lft_node));
    if (node == NULL) {
        return MEMORY_ISSUES;
    }
    
    // node->req = (Application*) malloc(sizeof(Application));
    // if (node->req == NULL) {
    //     free(node);
    //     return MEMORY_ISSUES;
    // }
    
    int code = copy_application(&(node->req), src->req);
    if (code) {
        free_all(2, node->req, node);
        return MEMORY_ISSUES;
    }
    
    node->distance = src->distance;
    node->left = node->right = NULL;
    
    code = code ? code : Lft_node_copy(&node->right, src->right);
    code = code ? code : Lft_node_copy(&node->left, src->left);
    if (code) {
        Lft_node_free(node);
        return code;
    }
    
    *result = node;
    
    return ok;
}

Lft_node* Lft_node_merge(Lft_node* a, Lft_node* b, int (*compare)(const Application*, const Application*)) {
    if (compare == NULL || (a == NULL && b == NULL)) {
        return NULL;
    }
    
    if (a == NULL) {
        return b;
    }
    if (b == NULL) {
        return a;
    }
    
    if (compare(a->req, b->req) > 0) {
        Lft_node* temp = a;
        a = b;
        b = temp;
    }
    
    a->right = Lft_node_merge(a->right, b, compare);
    
    if (a->right != NULL && (a->left == NULL || a->left->distance < a->right->distance)) {
        Lft_node* temp = a->left;
        a->left = a->right;
        a->right = temp;
    }
    
    a->distance = a->right == NULL ? 0 : a->right->distance + 1;

    return a;
}


int Lft_heap_set_null(Lft_heap* Lft) {
    if (Lft == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    Lft->head = NULL;
    Lft->size = 0;
    Lft->compare = NULL;
    
    return ok;
}

int Lft_heap_init(Lft_heap* Lft, int (*compare)(const Application*, const Application*)) {
    if (Lft == NULL || compare == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    Lft_heap_set_null(Lft);
    Lft->compare = compare;
    
    return ok;
}

int Lft_heap_copy(Lft_heap* Lft_dest, Lft_heap* Lft_src) {
    if (Lft_dest == NULL || Lft_src == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    int code = Lft_node_copy(&Lft_dest->head, Lft_src->head);
    if (code) {
        Lft_heap_free(Lft_dest);
        return code;
    }
    
    Lft_dest->size = Lft_src->size;
    Lft_dest->compare = Lft_src->compare;
    
    return ok;
}

int Lft_heap_free(Lft_heap* Lft) {
    if (Lft == NULL) {
        return ok;
    }
    
    Lft_node_free(Lft->head);
    Lft_heap_set_null(Lft);
    
    return ok;
}


int Lft_heap_meld(Lft_heap* Lft_res, Lft_heap* Lft_l, Lft_heap* Lft_r) {
    if (Lft_res == NULL || Lft_l == NULL || Lft_r == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    if (Lft_l == Lft_r || Lft_l->compare != Lft_r->compare) {
        return INVALID_INPUT;
    }
    
    Lft_heap Lft_tmp;
    
    Lft_tmp.head = Lft_node_merge(Lft_r->head, Lft_l->head, Lft_l->compare);
    Lft_tmp.size = Lft_l->size + Lft_r->size;
    Lft_tmp.compare = Lft_l->compare;
    
    Lft_l->head = Lft_r->head = NULL;
    Lft_l->size = Lft_r->size = 0;
    
    *Lft_res = Lft_tmp;
    
    return ok;
}

int Lft_heap_copy_meld(Lft_heap* Lft_res, Lft_heap* Lft_l, Lft_heap* Lft_r) {
    if (Lft_res == NULL || Lft_l == NULL || Lft_r == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    if (Lft_res == Lft_l || Lft_res == Lft_l) {
        return INVALID_INPUT;
    }
    
    int code = ok;
    Lft_heap Lft_lc, Lft_rc;
        
    Lft_heap_set_null(&Lft_lc);
    Lft_heap_set_null(&Lft_rc);
    Lft_heap_set_null(Lft_res);
    
    code = code ? code : Lft_heap_copy(&Lft_lc, Lft_l);
    code = code ? code : Lft_heap_copy(&Lft_rc, Lft_r);
    code = code ? code : Lft_heap_meld(Lft_res, &Lft_lc, &Lft_rc);
    
    Lft_heap_free(&Lft_lc);
    Lft_heap_free(&Lft_rc);
    
    if (code) {
        Lft_heap_free(Lft_res);
        return code;
    }
    
    return ok;
}


int Lft_heap_size(Lft_heap* Lft, unsigned int* size) {
    if (Lft == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    (*size) = Lft->size;
    return ok;
}

int Lft_heap_get_max(Lft_heap* Lft, Application** req) {
    if (Lft == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    if (Lft->head == NULL) {
        *req = NULL;
        return ok;
    }
    
    *req = Lft->head->req;
    
    return ok;
}

int Lft_heap_del_max(Lft_heap* Lft, Application** req) {
    if (Lft == NULL || req == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    if (Lft->head == NULL) {
        *req = NULL;
        return ok;
    }
    
    Lft_node* old_head= Lft->head;
    *req = Lft->head->req;
    Lft->head = Lft_node_merge(Lft->head->right, Lft->head->left, Lft->compare);
    
    free(old_head);
    Lft->size--;
    
    return ok;
}

int Lft_heap_insert(Lft_heap* Lft, Application* req) {
    if (Lft == NULL || req == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    Lft_node* new_node = (Lft_node*) malloc(sizeof(Lft_node));
    if (new_node == NULL) {
        return MEMORY_ISSUES;
    }

    new_node->distance = 0;
    new_node->req = req;
    new_node->right = NULL;
    new_node->left = NULL;
    
    Lft_heap Lft_add;
    Lft_add.head = new_node;
    Lft_add.size = 1;
    Lft_add.compare = Lft->compare;
    
    return Lft_heap_meld(Lft, Lft, &Lft_add);
}