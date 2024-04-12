#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include "../data_structs.h"
#include "../Department.h"

typedef struct Bst_node
{
    char* key;
    struct Bst_node* left;
    struct Bst_node* right;
    Department* dep;
} Bst_node;

typedef struct Binary_search_tree
{
    Bst_node* root;
} Binary_search_tree, Bst;

int Bst_set_null(Bst* tree);
int Bst_init(Bst* tree);
int Bst_free(Bst* tree);

//int Bst_contains(Bst* tree, char* key, int* is_contained);
int Bst_find(Bst* tree, char* key, Department** dep);
int Bst_insert(Bst* tree, char* key, Department* dep);
//int Bst_erase(Bst* tree, char* key);

void Bst_print(Bst_node *node, int tab);

#endif // _BINARY_SEARCH_TREE_H_