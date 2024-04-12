#include <stdlib.h>
#include <string.h>

#include "Skew_heap.h"
#include "../errors.h"

void Skw_node_free(Skw_node* node) {
    if (node == NULL) {
        return;
    }
    
    Skw_node_free(node->right);
    Skw_node_free(node->left);
    
    free_application(node->req);
    free(node);
    //free_all(2, node->req, node);
}

int Skw_node_copy(Skw_node** result, Skw_node* src) {
    if (result == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    if (src == NULL) {
        *result = NULL;
        return ok;
    }
    
    Skw_node* node = (Skw_node*) malloc(sizeof(Skw_node));
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
    
    node->right = NULL;
    node->left = NULL;
    code = code ? code : Skw_node_copy(&node->left, src->left);
    code = code ? code : Skw_node_copy(&node->right, src->right);
    if (code) {
        Skw_node_free(node);
        return code;
    }
    
    *result = node;
    
    return ok;
}

Skw_node* Skw_node_merge(Skw_node* a, Skw_node* b, int (*compare)(const Application*, const Application*)) {
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
        Skw_node* temp = a;
        a = b;
        b = temp;
    }
    
    a->left = Skw_node_merge(b, a->left, compare);
    
    Skw_node* temp = a->left;
    a->left = a->right;
    a->right = temp;
    
    return a;
}


int Skw_heap_set_null(Skw_heap* Skw) {
    if (Skw == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    Skw->head = NULL;
    Skw->size = 0;
    Skw->compare = NULL;
    
    return ok;
}

int Skw_heap_init(Skw_heap* Skw, int (*compare)(const Application*, const Application*)) {
    if (Skw == NULL || compare == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    Skw_heap_set_null(Skw);
    Skw->compare = compare;
    
    return ok;
}

int Skw_heap_copy(Skw_heap* Skw_dest, Skw_heap* Skw_src) {
    if (Skw_dest == NULL || Skw_src == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    int code = Skw_node_copy(&Skw_dest->head, Skw_src->head);
    if (code) {
        Skw_heap_free(Skw_dest);
        return code;
    }
    
    Skw_dest->size = Skw_src->size;
    Skw_dest->compare = Skw_src->compare;
    
    return ok;
}

int Skw_heap_free(Skw_heap* Skw) {
    if (Skw == NULL) {
        return ok;
    }
    
    Skw_node_free(Skw->head);
    Skw_heap_set_null(Skw);
    
    return ok;
}


int Skw_heap_meld(Skw_heap* Skw_res, Skw_heap* Skw_l, Skw_heap* Skw_r) {
    if (Skw_res == NULL || Skw_l == NULL || Skw_r == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    if (Skw_l == Skw_r || Skw_l->compare != Skw_r->compare) {
        return INVALID_INPUT;
    }
    
    Skw_heap Skw_tmp;
    
    Skw_tmp.head = Skw_node_merge(Skw_r->head, Skw_l->head, Skw_l->compare);
    Skw_tmp.size = Skw_l->size + Skw_r->size;
    Skw_tmp.compare = Skw_l->compare;
    
    Skw_l->head = Skw_r->head = NULL;
    Skw_l->size = Skw_r->size = 0;
    
    *Skw_res = Skw_tmp;
    
    return ok;
}

int Skw_heap_copy_meld(Skw_heap* Skw_res, Skw_heap* Skw_l, Skw_heap* Skw_r) {
    if (Skw_res == NULL || Skw_l == NULL || Skw_r == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    if (Skw_res == Skw_l || Skw_res == Skw_l) {
        return INVALID_INPUT;
    }
    
    int code = ok;
    Skw_heap Skw_lc, Skw_rc;
        
    Skw_heap_set_null(&Skw_lc);
    Skw_heap_set_null(&Skw_rc);
    Skw_heap_set_null(Skw_res);
    
    code = code ? code : Skw_heap_copy(&Skw_lc, Skw_l);
    code = code ? code : Skw_heap_copy(&Skw_rc, Skw_r);
    code = code ? code : Skw_heap_meld(Skw_res, &Skw_lc, &Skw_rc);
    
    Skw_heap_free(&Skw_lc);
    Skw_heap_free(&Skw_rc);
    
    if (code) {
        Skw_heap_free(Skw_res);
        return code;
    }
    
    return ok;
}

int Skw_heap_size(Skw_heap* Skw, unsigned int* size) {
    if (Skw == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    (*size) = Skw->size;
    return ok;
}

int Skw_heap_get_max(Skw_heap* Skw, Application** req) {
    if (Skw == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    if (Skw->head == NULL) {
        *req = NULL;
        return ok;
    }
    
    *req = Skw->head->req;
    
    return ok;
}

int Skw_heap_del_max(Skw_heap* Skw, Application** req) {
    if (Skw == NULL || req == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    if (Skw->head == NULL) {
        *req = NULL;
        return ok;
    }
    
    Skw_node* old_head= Skw->head;
    *req = Skw->head->req;
    Skw->head = Skw_node_merge(Skw->head->right, Skw->head->left, Skw->compare);
    
    free(old_head);
    Skw->size--;
    
    return ok;
}

int Skw_heap_insert(Skw_heap* Skw, Application* req) {
    if (Skw == NULL || req == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    
    
    Skw_node* new_node = (Skw_node*) malloc(sizeof(Skw_node));
    if (new_node == NULL) {
        return MEMORY_ISSUES;
    }

    new_node->req = req;
    new_node->right = NULL;
    new_node->left = NULL;
    
    Skw_heap Skw_add;
    Skw_add.head = new_node;
    Skw_add.size = 1;
    Skw_add.compare = Skw->compare;
    
    return Skw_heap_meld(Skw, Skw, &Skw_add);
}