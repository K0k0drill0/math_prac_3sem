#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../errors.h"
#include "Treap.h"

int Treap_destroy_for_meld(Treap* t);
/*
1. Добавление данных по ключу
2. Удаление данных с максимальным приоритетом по ключу
3. Получение данных с максимальным приоритетом по ключу
4. Слияние двух очередей без разрушения исходных
5. Слияние с разрушением
*/

int Treap_set_null(Treap* t) {
    if (t == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    t->root = NULL;
    t->comparator = NULL;
    
    return ok;
}

int Treap_init(Treap* t, int (*comparator) (const Application*, const Application*)) {
    if (t == NULL || comparator == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    t->root = NULL;
    t->comparator = comparator;

    return ok;
}

int Treap_node_init(Treap_node** nd, Application* a) {
    if (a == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    *nd = (Treap_node*)malloc(sizeof(Treap_node));
    if (*nd == NULL) {
        return MEMORY_ISSUES;
    }

    (*nd)->key = a;
    (*nd)->left = NULL;
    (*nd)->right = NULL;

    (*nd)->priority = rand();

    return ok;
}

void Treap_node_free(Treap_node* nd) {
    free_application(nd->key);
    free(nd);
}

int Treap_node_copy(Treap_node** dest, Treap_node* source) {
    if (source == NULL) {
        *dest = NULL;
        return ok;
    }

    int st = ok;

    Application* application_copy = NULL;
    st = copy_application(&application_copy, source->key);
    if (st != ok) {
        return st;
    }

    st = Treap_node_init(dest, application_copy);
    if (st != ok) {
        free_application(application_copy);
        return st;
    }

    (*dest)->priority = source->priority;

    return ok;
}

int Treap_copy_from_root(Treap_node** dest, Treap_node* source) { // ВАЖНО!! Если не получилось создать копию, то нужно освободить dest
//     Treap_node* tmp_dest = NULL, *source_dest = NULL;
    if (source == NULL) {
        *dest = NULL;
        return ok;
    }
    
    int st = ok;

    st = Treap_node_copy(dest, source);
    if (st != ok) {
        return st;
    }

    st = Treap_copy_from_root(&((*dest)->left), source->left);
    if (st != ok) {
        (*dest)->left = NULL;
        (*dest)->right = NULL;
        return st;
    }

    st = Treap_copy_from_root(&((*dest)->right), source->right);
    if (st != ok) {
        (*dest)->left = NULL;
        (*dest)->right = NULL;
        return st;
    }

    return ok;
}

int Treap_copy(Treap* dest, Treap* source) {
    int st = ok;

    st = Treap_copy_from_root(&(dest->root), source->root);
    dest->size = source->size;
    dest->comparator = source->comparator;
    if (st != ok) {
        Treap_destroy_from_root(dest->root);
        dest->root = NULL;
    }

    return st;
}

Treap_node* Treap_merge_from_root(Treap_node* t1, Treap_node* t2) {
    if (!t1 || !t2) {
        return t1? t1 : t2;
    }

    if (t1->priority > t2->priority) {
        t1->right = Treap_merge_from_root(t1->right, t2);
        return t1;
    }
    else {
        t2->left = Treap_merge_from_root(t1, t2->left);
        return t2;
    }
}

int Treap_merge(Treap* res, Treap* t1, Treap* t2) {
    res->root = Treap_merge_from_root(t1->root, t2->root);
    //res->size = t1->size + t2->size;
}

int Treap_merge_from_root_no_destruction(Treap_node** res, Treap_node* t1, Treap_node* t2) {
    int st = ok;

    Treap_node* copy1 = NULL, *copy2  = NULL;
    st = Treap_copy_from_root(&copy1, t1);
    if (st != ok){
        Treap_destroy_from_root(copy1);
        return st;
    }

    st = Treap_copy_from_root(&copy2, t2);
    if (st != ok){
        Treap_destroy_from_root(copy1);
        Treap_destroy_from_root(copy2);
        return st;
    }

    *res = Treap_merge_from_root(copy1, copy2);

    return ok;
}

int Treap_merge_no_destruction(Treap* res, Treap* t1, Treap* t2) {
    return Treap_merge_from_root_no_destruction(&(res->root), t1->root, t2->root);
}

Treap_node* Treap_node_meld(Treap_node* t1, Treap_node* t2, int (*compare)(const Application*, const Application*)) {
    if ((t1 == NULL && t2 == NULL) || compare == NULL) {
        return NULL;
    }
    if (t1 == NULL) {
        return t2;
    }
    if (t2 == NULL) {
        return t1;
    }

    Treap_node* less = NULL;
    Treap_node* greater = NULL;

    t1 = Treap_node_meld(t1, t2->left, compare);
    t1 = Treap_node_meld(t1, t2->right, compare);
    t2->left = t2->right = NULL;
    
    Treap_split(t1, t2->key, &less, &greater);
    less = Treap_merge_from_root(less, t2);
    t1 = Treap_merge_from_root(less, greater);
    
    return t1;
}

int Treap_meld(Treap* res, Treap* t1, Treap* t2) {
    if (t1 == NULL || t2 == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    if (t1 == t2 || t1->comparator != t2->comparator) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    Treap tmp;

    tmp.root = Treap_node_meld(t1->root, t2->root, t1->comparator);
    tmp.size = t1->size + t2->size;
    tmp.comparator = t1->comparator;
    
    t1->root = t2->root = NULL;
    t1->size = t2->size = 0;
    *res = tmp;
    
    int st = Treap_destroy_for_meld(t1);
    if (st != ok) {
        Treap_destroy(res);
        return st;
    }

    st = Treap_destroy_for_meld(t2);
    if (st != ok) {
        Treap_destroy(res);
        return st;
    }

    return ok;
}

int Treap_copy_meld(Treap* res, Treap* t1, Treap* t2) {
    if (res == NULL || t1 == NULL || t2 == NULL)
    {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    if (res == t1 || res == t1)
    {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    int code = ok;
    Treap t1c, t2c;
        
    Treap_set_null(&t1c);
    Treap_set_null(&t2c);
    Treap_set_null(res);
    
    code = code ? code : Treap_copy(&t1c, t1);
    code = code ? code : Treap_copy(&t2c, t2);
    code = code ? code : Treap_meld(res, &t1c, &t2c);
    
    // Treap_destroy(&t1c);
    // Treap_destroy(&t2c);
    
    if (code)
    {
        Treap_destroy(res);
        return code;
    }
    
    return ok;
}

int Treap_split(Treap_node* t, Application* key, Treap_node** t1, Treap_node** t2) {
    if (key == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }   

    if (t == NULL) {
        *t1 = NULL;
        *t2 = NULL;
        return ok;
    }

    int st = ok;
    if (compare_applications(t->key, key) == -1) {
        st = Treap_split(t->right, key, &(t->right), t2);
        if (st != ok) {
            return st;
        }
        *t1 = t;
    }
    else {
        st = Treap_split(t->left, key, t1, &(t->left));
        if (st != ok) {
            return st;
        }
        *t2 = t;
    }
    return ok;
}

int Treap_insert(Treap* t, Application* key) {
    Treap_node* new_nd = NULL;
    int st = ok;

    st = Treap_node_init(&new_nd, key);
    if (st != ok) {
        free_application(key);
        return st;
    }

    Treap_node* less, *greater;
    st = Treap_split(t->root, key, &less, &greater);
    if (st != ok) {
        Treap_node_free(new_nd);
        return st;
    }

    t->root = Treap_merge_from_root(Treap_merge_from_root(less, new_nd), greater);

    (t->size)++; 
    return ok;
}

int Treap_erase(Treap* t, Application* key) {
    Treap_node *less, *equal, *greater;
    int st = ok;
    st = Treap_split(t->root, key, &less, &greater);
    if (st != ok) {
        return st;
    }

    key->id += 1;
    st = Treap_split(greater, key, &equal, &greater);
    if (st != ok) {
        return st;
    }
    
    t->root = Treap_merge_from_root(less, greater);
    key->id -= 1;

    Treap_node_free(equal);

    return ok;
}

void Treap_print_from_root(FILE* stream, Treap_node* root, int n) {
    if (root != NULL) {
        Treap_print_from_root(stream, root->right, n + 1);
        for (int i = 0; i < n; i++) fprintf(stream, "\t");
        //print_application(stream, root->key);
        printf("I:%d P:%d\n", root->key->id, root->priority);
        Treap_print_from_root(stream, root->left, n + 1);
    }
}

void Treap_print(FILE* stream, const Treap t) {
    if (t.root == NULL) {
        fprintf(stream, "The tree is empty!\n");
        return;
    }
    Treap_print_from_root(stream, t.root, 0);
}

void Treap_destroy_from_root_for_meld(Treap_node* root) {
    if (root == NULL) {
        return;
    }
    //free_application(root->key);
    Treap_destroy_from_root_for_meld(root->left);
    Treap_destroy_from_root_for_meld(root->right);

    free(root);
}

int Treap_destroy_for_meld(Treap* t) {
    if (t == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    Treap_destroy_from_root_for_meld(t->root);
    t->size = 0;
    return Treap_set_null(t);
}

int Treap_destroy(Treap* t) {
    if (t == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    Treap_destroy_from_root(t->root);
    t->size = 0;
    return Treap_set_null(t);
}

void Treap_destroy_from_root(Treap_node* root) {
    if (root == NULL) {
        return;
    }
    free_application(root->key);
    Treap_destroy_from_root(root->left);
    Treap_destroy_from_root(root->right);

    free(root);
}

// int Treap_get_max_from(Tre)

int Treap_get_max(Treap* t, Application** res) {
    if (t == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    Treap_node* root = t->root;

    Treap_node* prev = root;
    Treap_node* tmp_node = root;

    while (tmp_node != NULL) {
        prev = tmp_node;
        tmp_node = tmp_node->right;
    }

    *res = prev->key;

    return ok;
}

int Treap_del_max(Treap* t, Application** res) {
    if (t == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    if (t->root == NULL) {
        *res = NULL;
        return ok;
    }

    Treap_node* root = t->root;

    if (root->right == NULL) {
        *res = root->key;
        t->root = root->left;
        free(root);
        //Treap_node_free(root);
        return ok;
    }

    Treap_node* prev = root;
    Treap_node* tmp_node = root->right;

    while (tmp_node->right != NULL) {
        prev = tmp_node;
        tmp_node = tmp_node->right;
    }

    *res = tmp_node->key;
    prev->right = tmp_node->left;
    free(tmp_node);
    //Treap_node_free(tmp_node);
    (t->size)--;
    return ok;
}

int Treap_size(Treap* t, unsigned int* res) {
    if (t == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    *res = t->size;
    return ok;
}