#ifndef ARR_H
#define ARR_H

typedef struct Expression {
    char* infix;
    char* postfix; 
    int res;

    int err;
    int line;
} Expression;

typedef struct Array {
    Expression* vec;
    int tmp_size;
    int max_size;
} Array;

int arr_init(Array* arr);
int arr_append(Array* arr, const Expression expr);
int arr_delete(Array* arr);
int arr_print(FILE* outp, Array arr);

#endif