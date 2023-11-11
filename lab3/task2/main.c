#include <stdio.h>
#include <stdarg.h>

typedef struct {
    int n;
    double* coords;
} Vector;

typedef struct {
    int m, n;
    double** data;
} Matrix;

int max_norm(const Vector vec, double* ans) {
    ///
}

int p_norm(const Vector vec, double* ans) {
    ///
}

int matrix_norm(const Vector vec, const Matrix matrix, double* ans) {
    
}

int solve(
    const unsigned int n, const double eps,
    int (*norm1)(const Vector, double*),
    int (*norm2)(const Vector, const int, double*), const int p,
    int (*norm3)(const Vector, const Matrix, double*), const Matrix matrix,
    int* res1_size, Vector** res1,
    int* res2_size, Vector** res2,
    int* res3_size, Vector** res3,
    const int args_amount, ...
    ) {
    
}
