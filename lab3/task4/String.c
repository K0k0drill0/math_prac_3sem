#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "String.h"

void print_error(int st) {
    switch (st)
    {
    case MEMORY_ISSUES:
        printf("Can not allocate the memory!\n");
        break;
    case INVALID_FUNCTION_ARGUMENT:
        printf("Invalid function parameter!\n");
        break;
    case INVALID_INPUT:
        printf("Invalid input!\n");
        break;
    case OVERFLOWED:
        printf("Overflowed!\n");
        break;
    default:
        break;
    }
}

int string_create(String* s, const char* data) {
    if (data == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    s->size = strlen(data);
    s->data = (char*)malloc(sizeof(char) * (s->size + 1));
    if (s->data == NULL) {
        return MEMORY_ISSUES;
    }
    int ind = 0;
    while (data[ind] != '\0') {
        s->data[ind] = data[ind];
        ind++;
    }
    s->data[ind] = data[ind];
    return ok;
}

int string_delete(String* s) {
    free(s->data);
    s->size = 0;
}

int string_compare(const String s1, const String s2) {
    if (s1.size != s2.size) {
        return (s1.size > s2.size) ? 1 : -1;
    }

    int ind = 0;
    while (s1.data[ind] != '\0' && s2.data[ind] != '\0') {
        if (s1.data[ind] != s2.data[ind]) {
            return (s1.data[ind] > s2.data[ind]) ? 1 : -1;
        }
        ind++;
    }
    return 0;
}

int string_equal(const String s1, const String s2) {
    if (s1.size != s2.size) {
        return 0;
    }

    int ind = 0;
    while (s1.data[ind] != '\0' && s2.data[ind] != '\0') {
        if (s1.data[ind] != s2.data[ind]) {
            return 0;
        }
        ind++;
    }
    return 1;
}

int string_copy(String* dest, const String source) {
    if (dest->size != source.size) {
        char* tmp = (char*)realloc(dest->data, (source.size + 1));
        if (!tmp) {
            return MEMORY_ISSUES;
        } 
        dest->data = tmp;
    }

    dest->size = source.size;
    int ind = 0;
    while (source.data[ind] != '\0') {
        dest->data[ind] = source.data[ind];
        ind++;
    }
    dest->data[ind] = '\0';
    return ok;
}

int string_copy_new(String* dest, const String s) {
    dest->data = (char*)malloc(sizeof(char) * (s.size + 1));
    if (dest->data == NULL) {
        return MEMORY_ISSUES;
    }

    for (int i = 0; i <= s.size; i++) {
        dest->data[i] = s.data[i];
    }

    dest->size = s.size;
    return ok;
}

int string_concatenation(String* s1, const String s2) {
    char* tmp = (char*)realloc(s1->data, sizeof(char) * (s2.size + s1->size + 1));
    if (!tmp) {
        return MEMORY_ISSUES;
    }
    s1->data = tmp;

    int ind = s1->size;
    for (int i = 0; i <= s2.size; i++) {
        s1->data[ind] = s2.data[i];
        ind++;
    }
    s1->size = ind-1;
    return ok;
}



// int main() {
//     String s1, s2;
//     string_create(&s1, "1");
//     string_create(&s2, "123");
//     string_concatenation(&s1, s2);
//     printf("%s, %s, %d, %d\n", s1.data, s2.data, s1.size, s2.size);
//     return 0;
// }