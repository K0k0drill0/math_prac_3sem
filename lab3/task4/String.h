#ifndef STRING_H
#define STRING_H

#include <stdbool.h>

typedef struct {
    char* data;
    int size;
} String;

enum status_code {
    ok,
    MEMORY_ISSUES, 
    INVALID_FUNCTION_ARGUMENT, 
    INVALID_INPUT
};

void print_error(int st);

int string_create(String* s, const char* data);

int string_delete(String* s);

int string_compare(const String s1, const String s2);

int string_equal(const String s1, const String s2);

int string_copy(String* dest, const String source);

int string_copy_new(String* dest, const String s);

int string_concatenation(String* s1, const String s2);

#endif