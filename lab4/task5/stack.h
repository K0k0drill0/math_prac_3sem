#ifndef STACK_H
#define STACK_H

#include <stdlib.h>

typedef struct Stack {
    char* data;
    struct Stack* prev;
} Stack;

int my_strcpy(char** dest, const char* source);

int stack_init(Stack** st, const char* data);

int stack_push(Stack** top, const char* data);
int stack_pop(Stack** top);

int stack_top(const Stack* st, char** val);

int stack_reverse(Stack** stack);

int stack_is_empty(const Stack* st);

int stack_destroy(Stack** st);

#endif