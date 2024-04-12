#include <stdio.h>
#include <stdlib.h>

#include "map.h"
#include "../errors.h"

#include "Dynamic_array.h"
#include "Trie.h"
#include "Hash_table.h"
#include "Binary_search_tree.h"

int Map_init(Map* map, map_base_type t) {
    if (map == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    map->ds = NULL;

    switch (t)
    {
    case MAP_ARR:
        map->ds = malloc(sizeof(Dynamic_array));

        map->set_null = (int (*)(void*)) Dynamic_array_set_null;
        map->init = (int (*)(void*)) Dynamic_array_init;
        map->free = (int (*)(void*)) Dynamic_array_free;
        map->find = (int (*)(void*, char*, Department**)) Dynamic_array_find;
        map->insert = (int (*)(void*, char*, Department*)) Dynamic_array_insert;
        break;
    case MAP_TRIE:
        map->ds = malloc(sizeof(Dynamic_array));

        map->set_null = (int (*)(void*)) Trie_set_null;
        map->init = (int (*)(void*)) Trie_init;
        map->free = (int (*)(void*)) Trie_free;
        map->find = (int (*)(void*, char*, Department**)) Trie_find;
        map->insert = (int (*)(void*, char*, Department*)) Trie_insert;
        break;
    case MAP_HASHSET:
        map->ds = malloc(sizeof(Hash_set));

        map->set_null = (int (*)(void*)) Hash_set_set_null;
        map->init = (int (*)(void*)) Hash_set_init;
        map->free = (int (*)(void*)) Hash_set_free;
        map->find = (int (*)(void*, char*, Department**)) Hash_set_find;
        map->insert = (int (*)(void*, char*, Department*)) Hash_set_insert;
        break;
    case MAP_BST:
        map->ds = malloc(sizeof(Bst));

        map->set_null = (int (*)(void*)) Bst_set_null;
        map->init = (int (*)(void*)) Bst_init;
        map->free = (int (*)(void*)) Bst_free;
        map->find = (int (*)(void*, char*, Department**)) Bst_find;
        map->insert = (int (*)(void*, char*, Department*)) Bst_insert;
        break;
    default:
        break;
    }

    map->set_null(map->ds);
    int st = map->init(map->ds);

    if (st != ok) {
        free(map->ds);
        Map_set_null(map);
        return st;
    }

    return ok;
}

int Map_set_null(Map* map) {
    if (map == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    map->ds = NULL;
    map->init = NULL;
    map->free = NULL;
    map->set_null = NULL;
    map->find = NULL;
    map->insert = NULL;

    return ok;
}

int Map_free(Map* map) {
    if (map == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    int st = ok;

    st = map->free(map->ds);

    free(map->ds);

    return st;
}

int Map_find(Map* map, char* key, Department** res) {
    if (map == NULL || key == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    return map->find(map->ds, key, res);
}

int Map_insert(Map* map, char* key, Department* dep) {
    if (map == NULL || key == NULL || dep == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    return map->insert(map->ds, key, dep);
}