#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Dynamic_array.h"
#include "../errors.h"

int Arr_elem_init(Arr_elem** el, char* key, Department* dep) {
    *el = (Arr_elem*)malloc(sizeof(Arr_elem));
    if (el == NULL) {
        return MEMORY_ISSUES;
    }

    (*el)->key = key;
    (*el)->elem = dep;

    return ok;
}

void Arr_elem_free(Arr_elem* el) {
    if (el == NULL) {
        return;
    }
    if (el->key != el->elem->name) {
        free(el->key);
    }

    Department_free(el->elem);
    //Department_set_null(el->elem);

    free(el);
} 

int Dynamic_array_init(Dynamic_array* arr) {
    if (arr == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    arr->max_size = 16;

    arr->arr = (Arr_elem**)malloc(sizeof(Arr_elem*) * arr->max_size);
    if (arr->arr == NULL) {
        return MEMORY_ISSUES;
    }

    arr->size = 0;

    return ok;
}

int Dynamic_array_set_null(Dynamic_array* arr) {
    if (arr == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    arr->arr = NULL;
    arr->size = 0;
    arr->max_size = 0;

    return ok;
}

int Dynamic_array_shift(Dynamic_array* arr, int ind) {
    if (arr == NULL || arr->arr == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    int tmp_ind = arr->size + 1;

    while (tmp_ind != ind) {
        arr->arr[tmp_ind] = arr->arr[tmp_ind - 1];

        tmp_ind--;
    }

    return ok;
}

int Dynamic_array_insert(Dynamic_array* arr, char* key, Department* dep) {
    if (arr == NULL || key == NULL || dep == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    if (arr->size == arr->max_size) {
        Arr_elem** tmp = (Arr_elem**)realloc(arr->arr, sizeof(Arr_elem*) * arr->max_size * 2);
        if (tmp == NULL) {
            return MEMORY_ISSUES;
        }

        arr->arr = tmp;
        arr->max_size *= 2;
    }

    int ind = -1;
    for (int i = 0; i < arr->size; i++) {
        if (strcmp(arr->arr[i]->key, key) > 0) {
            ind = i;
            break;
        }
    }

    Arr_elem* new_el = NULL;
    int st = Arr_elem_init(&new_el, key, dep);
    if (st != ok) {
        return st;
    }

    if (ind == -1) {
        arr->arr[arr->size] = new_el;
        arr->size++;
    }
    else {
        Dynamic_array_shift(arr, ind);
        arr->arr[ind] = new_el;
        arr->size++;
    }

    return ok;
}

int Dynamic_array_find(Dynamic_array* arr, char* key, Department** res) {
    if (arr == NULL || arr->arr == NULL || key == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    int l = 0;
    int r = arr->size - 1;
    int m = (l + r) / 2;

    int cmp = 1;
    while (l <= r) {
        m = l + (r - l) / 2;

       // printf("%s, %s\n", arr->arr[m]->key, key);
        cmp = strcmp(arr->arr[m]->key, key);
        if (cmp == 0) {
            *res = arr->arr[m]->elem;

            return ok;
        }
        else if (cmp > 0) {
            r = m - 1;
        }
        else {
            l = m + 1;
        }
    }

    if (!strcmp(key, arr->arr[1]->key)) {
        *res = arr->arr[1]->elem;
        return ok;
    }

    *res = NULL;
    return ok;
}

int Dynamic_array_free(Dynamic_array* arr) {
    if (arr == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    for (int i = 0; i < arr->size; i++) {
        Arr_elem_free(arr->arr[i]);
    }
    free(arr->arr);
    return ok;
}

void Dynamic_array_print(FILE* stream, Dynamic_array* arr) {
    if (arr == NULL) {
        return;
    }

    for (int i = 0; i < arr->size; i++) {
        fprintf(stream, "%s\n", arr->arr[i]->elem->name);
    }
    fprintf(stream, "\n");
}