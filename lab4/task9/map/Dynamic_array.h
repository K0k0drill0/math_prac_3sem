#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include "../priority_queue/priority_queue.h"
#include "../Department.h"

typedef struct Arr_elem {
    char* key;
    Department* elem;
} Arr_elem;

typedef struct Dynamic_array {
    Arr_elem** arr;
    unsigned int size;
    unsigned int max_size;
} Dynamic_array;

int Dynamic_array_init(Dynamic_array* arr);
int Dynamic_array_set_null(Dynamic_array* arr);
int Dynamic_array_free(Dynamic_array* arr);

int Dynamic_array_find(Dynamic_array* arr, char* key, Department** res);
int Dynamic_array_insert(Dynamic_array* arr, char* key, Department* dep);

void Dynamic_array_print(FILE* stream, Dynamic_array* arr);

#endif