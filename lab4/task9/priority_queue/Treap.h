#ifndef TREAP_H
#define TREAP_H

#include "../data_structs.h"

typedef struct Treap_node {
    Application* key;
    int priority;
    struct Treap_node* left;
    struct Treap_node* right;
} Treap_node;

typedef struct Treap {
    Treap_node* root;
    unsigned int size;
    int (*comparator) (const Application*, const Application*);
} Treap;

int Treap_set_null(Treap* t);
int Treap_init(Treap* t, int (*comparator) (const Application*, const Application*));
// int Treap_node_init(Treap_node** nd, Application* a);
// void Treap_node_free(Treap_node* nd);

int Treap_get_max(Treap* t, Application** res);
int Treap_del_max(Treap* t, Application** res);

int Treap_copy(Treap* dest, Treap* source);

int Treap_merge(Treap* res, Treap* t1, Treap* t2);
int Treap_merge_no_destruction(Treap* res, Treap* t1, Treap* t2);
int Treap_split(Treap_node* t, Application* key, Treap_node** t1, Treap_node** t2);

int Treap_meld(Treap* res, Treap* t1, Treap* t2);
int Treap_copy_meld(Treap* res, Treap* t1, Treap* t2);

 int Treap_insert(Treap* t, Application* key);
 int Treap_erase(Treap* t, Application* key);

void Treap_print(FILE* stream, const Treap t);
//void Treap_print_from_root(FILE* stream, Treap_node* root, int n);

int Treap_destroy(Treap* t);
void Treap_destroy_from_root(Treap_node* root);

int Treap_size(Treap* t, unsigned int* res);

#endif