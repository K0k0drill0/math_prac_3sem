#include <stdio.h>
#include <stdlib.h>
#include "arr.h"

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

int fprint_error(FILE* outp, const int st) {
    if (outp == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    switch (st)
    {
    case UNABLE_TO_OPEN_FILE:
        fprintf(outp, "Unable to open a file!\n");
        break;
    case MEMORY_ISSUES:
        fprintf(outp, "Unable to allocate the memory!\n");
        break;
    case OVERFLOWED:
        fprintf(outp, "Overflowed!\n");
        break;
    case INVALID_FUNCTION_ARGUMENT:
        fprintf(outp, "Invalid function argument!\n");
        break;
    case INVALID_CHARACTER:
        fprintf(outp, "Invalid character met!\n");
        break;
    case UNBALANCED_BRACKETS:
        fprintf(outp, "Unbalanced brackets!\n");
        break;
    case INVALIND_OPERATOR_POSTURE:
        fprintf(outp, "Invalid operator position!\n");
        break;
    case NO_VALUE_IN_BRACKETS:
        fprintf(outp, "Empty brackets!\n");
        break;
    case EMPTY_EXPRESSION:
        fprintf(outp, "Empty expression!\n");
        break;
    case DIVISION_BY_ZERO:
        fprintf(outp, "Division by zero!\n");
        break;
    case ZERO_POW_ZERO:
        fprintf(outp, "Zero in power of zero!\n");
        break;
    case NEGATIVE_POWER:
        fprintf(outp, "Negative power!\n");
        break;
    default:
        break;
    }
}

int arr_init(Array* arr) {
    arr->max_size = 16;
    arr->vec = (Expression*)malloc(sizeof(Expression) * arr->max_size);
    if (arr->vec == NULL) {
        return MEMORY_ISSUES;
    }

    arr->tmp_size = 0;
    return ok;
}

int arr_append(Array* arr, const Expression expr) {
    if (arr == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    if (arr->tmp_size == arr->max_size) {
        arr->max_size *= 2;
        Expression* tmp = (Expression*)realloc(arr->vec, arr->max_size);
        if (tmp == NULL) {
            return MEMORY_ISSUES;
        }
        arr->vec = tmp;
    }
    arr->vec[arr->tmp_size] = expr;
    (arr->tmp_size)++;
    return ok;
}

int print_Expression(FILE* outp, Expression expr) {
    if (outp == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    if (expr.err != ok) {
        fprintf(outp, "Infix form: %s\nLine: %d\n", expr.infix, expr.line);
        fprint_error(outp, expr.err);
        fprintf(outp, "\n");
    }
    else {
        printf("Infix form: %s\nPostfix form:%s\nResult: %d\n\n", expr.infix, expr.postfix, expr.res);
    }
    return ok;
}

int arr_print(FILE* outp, Array arr) {
    if (outp == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    if (arr.tmp_size == 0) {
        printf("None");
    }
    for (int i = 0; i < arr.tmp_size; i++) {
        print_Expression(outp, arr.vec[i]);
    }
    return ok;
}

int expr_delete(Expression expr) {
    free(expr.infix);
    free(expr.postfix);
}

int arr_delete(Array* arr) {
    if (arr == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    for (int i = 0; i < arr->tmp_size; i++) {
        expr_delete(arr->vec[i]);
    }
    free(arr->vec);
    arr->max_size = 0;
    arr->tmp_size = 0;
    return ok;
}