#include <stdio.h>
#include <stdlib.h>

#include "Binary_heap.h"
#include "../errors.h"

int Binary_heap_set_null(Binary_heap* bh) {
    if (bh == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    bh->comparator = NULL;
    bh->heap.data = NULL;
    bh->heap.size = 0;
    bh->heap.max_size = 0;

    return ok;
}

int Binary_heap_init(Binary_heap* bh,
 int (*comparator)(const Application*, const Application*)) {

    if (comparator == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    int st = ok;

    st = Application_arr_init(&(bh->heap));
    if (st != ok) {
        return st;
    }

    bh->comparator = comparator;

    return ok;
}

int Binary_heap_copy(Binary_heap* dest, Binary_heap* source) {
    if (source == NULL || dest == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    dest->comparator = source ->comparator;

    Application_arr arr_copy;

    int st = ok;

    st = Application_arr_copy(&arr_copy, dest->heap);
    if (st != ok) {
        Binary_heap_set_null(dest);
        return st;
    }

    return ok;
}

int Binary_heap_sift_down(Binary_heap bh, int i) {
    if (bh.heap.data == NULL || bh.comparator == NULL || i < 0) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    while(2 * i + 1 < bh.heap.size) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        int j = left;
        if (right < bh.heap.size && compare_applications(bh.heap.data[right], bh.heap.data[left]) == 1) {
            j = right;
        }

        if (compare_applications(bh.heap.data[i], bh.heap.data[j]) >= 0) {
            break;
        }

        swap_ptr((void**)&(bh.heap.data[i]), (void**)&(bh.heap.data[j]));
        i = j;
    }

    return ok;
}

int Binary_heap_sift_up(Binary_heap bh, int i) {
    if (bh.heap.data == NULL || bh.comparator == NULL || i < 0) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    while(compare_applications(bh.heap.data[i], bh.heap.data[(i - 1) / 2]) == 1) {
        swap_ptr((void**)&bh.heap.data[i], (void**)&bh.heap.data[(i - 1) / 2]);
        i = (i - 1) / 2;
    }

    return ok;
}

int Binary_heap_insert(Binary_heap* bh, Application* a) { 
    if (bh == NULL || a == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    if (bh->heap.data == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    int st = ok;

    st = Application_arr_push_back(&(bh->heap), a);
    if (st != ok) {
        Application_arr_free(&(bh->heap));
        free_application(a);
        return st;
    }

    st = Binary_heap_sift_up(*bh, bh->heap.size - 1);

    return st;
}

int Binary_heap_get_max(Binary_heap* bh, Application** res) {
    if (bh == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    if (bh->heap.data == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    *res = bh->heap.data[0];
    return ok;
}

int Binary_heap_del_max(Binary_heap* bh, Application** res) {
    if (bh == NULL || bh->heap.data == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    if (bh->heap.size == 0) {
        *res = NULL;
        return ok;
    }

    if (bh->heap.size == 1) {
        *res = bh->heap.data[0];
        //free_application(bh->heap.data[0]);
        bh->heap.size = 0;
        return ok;
    }

    *res = bh->heap.data[0];
    // free_application(bh->heap.data[0]);
    bh->heap.data[0] = bh->heap.data[bh->heap.size - 1];
    bh->heap.size--;
    return Binary_heap_sift_down(*bh, 0); 
}

int Binary_heap_build_from_arr(Binary_heap* res, int (*comparator)(const Application*, const Application*), Application_arr source) {
    if (source.data == NULL || comparator == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    res->heap = source;
    res->comparator = comparator;

    int st = ok;

    for (int i = res->heap.size / 2; i >= 0; i--) {
        st = Binary_heap_sift_down(*res, i);

        if (st != ok) {
            return st;
        }
    }

    return st;
}

int Binary_heap_merge_no_destruction(Binary_heap* res, Binary_heap* bh1, Binary_heap* bh2) {
    if (bh1->comparator == NULL || bh1->heap.data == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    if (bh2->comparator == NULL || bh2->heap.data == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    int st = ok;

    Application_arr res_arr;

    st = Application_arr_concatenation(bh1->heap, bh2->heap, &res_arr);
    if (st != ok) {
        return st;
    }

    st = Binary_heap_build_from_arr(res, bh1->comparator, res_arr);
    if (st != ok) {
        Application_arr_free(&res_arr);
        return st;
    }

    return ok;
}

int Binary_heap_merge_with_destruction(Binary_heap* res, Binary_heap* bh1, Binary_heap* bh2) {
    if (bh1->comparator == NULL || bh1->heap.data == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    if (bh2->comparator == NULL || bh2->heap.data == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    int st = ok;

    st = Binary_heap_merge_no_destruction(res, bh1, bh2);
    if (st != ok) {
        return st;
    }

    st = Binary_heap_free(bh1);
    if (st != ok) {
        return st;
    }

    st = Binary_heap_free(bh2);
    if (st != ok) {
        return st;
    }

    return ok;
}

void printSpaces(int n) {
    for (int i = 0; i < n; i++) {
        printf(" ");
    }
}

// Функция для печати бинарной кучи в виде дерева
void Binary_heap_print(Application_arr heap, int index, int level) {
    if (index >= heap.size) {
        return;
    }

    Binary_heap_print(heap, 2 * index + 2, level + 1);

    printSpaces(level * 4);
    printf("%d\n", heap.data[index]->id);

    Binary_heap_print(heap, 2 * index + 1, level + 1);
}

int Binary_heap_free(Binary_heap* bh) {
    Application_arr_free(&(bh->heap));
    return Binary_heap_set_null(bh);
}

int Binary_heap_size(Binary_heap* bh, unsigned int* res) {
    if (bh == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    *res = bh->heap.size;

    return ok;
}
