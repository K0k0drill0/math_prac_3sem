#ifndef _TRIE_H_
#define _TRIE_H_

#include "errors.h"
#include <string.h>
#include <ctype.h>

#define VARIABLE_CHARAS_COUNT (26*2 + 10 + 1) // letters, digits and "_"

typedef struct Trie_node
{
    struct Trie_node * childs[VARIABLE_CHARAS_COUNT];
    int is_final; // 1 - yes, 0 - no
    uint32_t value;
    
} Trie_node;

typedef struct Trie
{
    Trie_node* root;
} Trie;

Trie_node* Trie_node_create();
void Trie_node_free(Trie_node* node);

Trie* Trie_create();
void Trie_free(Trie* tree);

int find_digits_count_in_number(int number);

status_code Trie_find(Trie* tree, char * name, Trie_node* * result);
status_code Trie_insert(Trie* tree, char * name, uint32_t value);
status_code Trie_set_null(Trie* tree);

#endif