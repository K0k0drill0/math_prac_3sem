#ifndef TRIE_H
#define TRIE_H

#include "../data_structs.h"
#include "../Department.h"

#define TRIE_ALPABET_SIZE 36

typedef struct Trie_node
{
    struct Trie_node* parent;
    struct Trie_node* children[TRIE_ALPABET_SIZE];
    Department* dep;
} Trie_node;

typedef struct Trie
{
    Trie_node* root;
} Trie;

typedef struct Trie_key_val {
    char* key; 
    Department* elem;
} Trie_key_val;

int Trie_set_null(Trie* trie);
int Trie_init(Trie* trie);
int Trie_free(Trie* trie);

//int Trie_contains(Trie* trie, char* key, int* is_contained);
int Trie_find(Trie* trie, char* key, Department** dep);
int Trie_insert(Trie* trie, char* key, Department* dep);

#endif 