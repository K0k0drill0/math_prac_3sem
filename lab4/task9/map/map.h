#ifndef MAP_H
#define MAP_H

#include "../Department.h"

typedef enum map_base {
    MAP_HASHSET,
    MAP_ARR,
    MAP_BST,
    MAP_TRIE,

    MAP_INVALID
} map_base_type;

typedef struct Map {
    void* ds;

    int (*set_null)(void* ds);
    int (*init)(void* ds); // probably calc_hash
    int (*free)(void* ds);

    int (*find)(void* ds, char* key, Department** dep);
    int (*insert) (void* ds, char* key, Department* dep);
    // erase???
} Map;

int Map_init(Map* map, map_base_type t);
int Map_set_null(Map* map);
int Map_free(Map* map);

int Map_find(Map* map, char* key, Department** res);
int Map_insert(Map* map, char* key, Department* dep);

#endif