#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "stack.h"

enum status_code {
    ok, 
    UNABLE_TO_OPEN_FILE,
    MEMORY_ISSUES,
    OVERFLOWED,
    INVALID_FUNCTION_ARGUMENT,

    INVALID_CHARACTER,
    UNBALANCED_BRACKETS,
    INVALIND_OPERATOR_POSTURE,
    NO_VALUE_IN_BRACKETS, 
    EMPTY_EXPRESSION,

    DIVISION_BY_ZERO,
    ZERO_POW_ZERO,
    NEGATIVE_POWER
};

int my_strcpy(char** dest, const char* source) {
    if (source == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    int ind = 0;
    while (source[ind] != '\0') {
        ind++;
    }

    *dest = (char*)malloc(sizeof(char) * ind + 1);
    if (*dest == NULL) {
        return MEMORY_ISSUES;
    }
    while (ind >= 0) {
        (*dest)[ind] = source[ind];
        ind--;
    }
    return ok;
}

int stack_init(Stack** st, const char* data) {
    if (data == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    *st = (Stack*)malloc(sizeof(Stack));
    if (*st == NULL) {
        return MEMORY_ISSUES;
    }

    int state = my_strcpy(&((*st)->data), data);
    if (state != ok) {
        free(*st);
        return state;
    }

    (*st)->prev = NULL;
    return ok;
}

int stack_push(Stack** top, const char* data) {
    if (data == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    Stack* new_node = NULL;
    int st = stack_init(&new_node, data);
    if (st != ok) {
        return st;
    }

    new_node->prev = *top;
    *top = new_node;
    return ok;
}

int stack_pop(Stack** top) {
    if (*top == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    Stack* new_top = (*top)->prev;
    free((*top)->data);
    free(*top);

    *top = new_top;
    return ok;
}

int stack_top(const Stack* st, char** val) {
    if (st == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    *val = st->data;
    return ok;
}

int stack_reverse(Stack** stack) {
    if (*stack == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    Stack* tmp = NULL;
    int st = ok;
    while (!stack_is_empty(*stack)) {
        char* top = NULL;
        st = stack_top(*stack, &top);
        if (st != ok) {
            stack_destroy(&tmp);
            return st;
        }

        st = stack_push(&tmp, top);
        if (st != ok) {
            stack_destroy(&tmp);
            return st;
        }

        st = stack_pop(stack);
        if (st != ok) {
            stack_destroy(&tmp);
            return st;
        }
    }

    *stack = tmp;
    return ok;
}

int stack_is_empty(const Stack* st) {
    return (st == NULL);
}

int stack_destroy(Stack** st) {
    while (stack_pop(st) == ok);
}
