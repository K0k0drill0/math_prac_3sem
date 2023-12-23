#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>
#include "vector.h"

void print_error(const int st) {
    switch (st)
    {
    case MEMORY_ISSUES:
        printf("Memory issues!\n");
        break;
    case INVALID_PARAMETER:
        printf("Invalid parameter!\n");
    default:
        break;
    }
}

int max_norm(const Vector vec, double* ans) {
    *ans = 0.0;
    for (int i = 0; i < vec.n; i++) {
        if (fabs(vec.coords[i]) > *ans) {
            *ans = fabs(vec.coords[i]);
        }
    }
    return ok;
}

int p_norm(const Vector vec, const int p, double* ans) {
    if (p < 1) {
        return INVALID_PARAMETER;
    }
    *ans = 0.0;
    for (int i = 0; i < vec.n; i++) {
        *ans += pow(fabs(vec.coords[i]), p);
    }
    *ans = pow(*ans, 1.0/p);
    return ok;
}

int matrix_norm(const Vector vec, const Matrix matrix, const double eps, double* ans) {
    if (!is_positive_definite(matrix, eps)) {
        return INVALID_PARAMETER;
    }

    Vector mult;
    int st = matrix_multiply_vector(matrix, vec, &mult);
    if (st != ok) {
        return st;
    }

    double dot_pr = 0.0;
    st = dot(mult, vec, &dot_pr);
    if (st != ok) {
        free_vector(&mult);
        return st;
    }
    free_vector(&mult);
    *ans = sqrt(dot_pr);
    return ok;
}

int is_positive_definite(const Matrix mat, const double eps) {
    if (mat.m != mat.n) {
        return 0; // матрица не квадратная, поэтому не является положительно определенной
    }

    for (int i = 0; i < mat.m; i++) {
        for (int j = 0; j < i; j++) {
            if (mat.data[i][j] != mat.data[j][i]) {
                return 0; // матрица не симметрична, поэтому не является положительно определенной
            }
        }
    }

    for (int i = 0; i < mat.m; i++) {
        double dt = det(mat, i+1);
        if (dt < eps) {
            return 0;
        }
    }

    return 1; // матрица является положительно определенной
}

int matrix_multiply_vector(const Matrix matrix, const Vector vec, Vector* ans)  {
    if (matrix.n != vec.n) {
        return INVALID_PARAMETER;
    }
    int st = create_vector(ans, matrix.m);
    if (st != ok) {
        return st;
    }
    for (int i = 0; i < matrix.m; i++) {
        ans->coords[i] = 0.0;
        for (int j = 0; j < matrix.n; j++) {
            ans->coords[i] += matrix.data[i][j] * vec.coords[j];
        }
    }
    return ok;
}

int dot(const Vector v1, const Vector v2, double* ans) {
    if (v1.n != v2.n) {
        return INVALID_PARAMETER;
    }

    int n = v1.n;
    *ans = 0.0;
    for (int i = 0; i < n; i++) {
        *ans += v1.coords[i] * v2.coords[i];
    }
    return ok;
}

int create_vector(Vector* vec, const int n, ...) {
    vec->n = n;
    vec->coords = (double*)malloc(sizeof(double) * n);
    if (vec->coords == NULL) {
        return MEMORY_ISSUES;
    }

    va_list args;
    va_start(args, n);
    for (int i = 0; i < n; i++) {
        double el = va_arg(args, double);
        vec->coords[i] = el;
    }
    va_end(args);
    return ok;
}

int create_matrix(Matrix* matrix, const int m, const int n, ...) {
    matrix->m = m;
    matrix->n = n;
    matrix->data = (double**)malloc(sizeof(double*) * m);
    if (matrix->data == NULL) {
        return MEMORY_ISSUES;
    }

    va_list args;
    va_start(args, n);

    for (int i = 0; i < m; i++) {
        matrix->data[i] = (double*)malloc(sizeof(double) * n);
        if (matrix->data[i] == NULL) {
            matrix->m = i;
            //free_matrix(matrix);
            return MEMORY_ISSUES;
        }
        for (int j = 0; j < n; j++) {
            double el = va_arg(args, double);
            matrix->data[i][j] = el;
        }
    }
    va_end(args);
    return ok;
}

void free_vector(Vector* vec) {
    vec->n = 0;
    if (vec->coords != NULL) free(vec->coords);
}

void free_matrix(Matrix* matrix) {
    for (int i = 0; i < matrix->m; i++) {
        free(matrix->data[i]);
    }
    matrix->n = 0;
    matrix->m = 0;
    free(matrix->data);
}

void free_vector_arr(Vector** arr, int size) {
    for (int i = 0; i < size; i++) {
        //free(arr[i]->coords);
        free_vector(&(*arr)[i]);
        //(*arr)[i] = NULL;
    }
    free(*arr);
}

void print_vector(const Vector vec) {
    for (int i = 0; i < vec.n; i++) {
        printf("%lf ", vec.coords[i]);
    }
    printf("\n");
}

void print_vector_array(const Vector* arr, const int size) {
    for (int i = 0; i < size; i++) {
        print_vector(arr[i]);
    }
    printf("\n");
}

void print_matrix(const Matrix matrix) {
    for (int i = 0; i < matrix.m; i++) {
        for (int j = 0; j < matrix.n; j++) {
            printf("%lf ", matrix.data[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

double det(const Matrix matrix, const int N) {
    if (matrix.n != matrix.m) {
        return INVALID_PARAMETER;
    }
    //int N = matrix.n;
    double** matr = matrix.data;
    double sum,mul;
    sum =  0;
    if(2 == N)
    {
        sum= matr[0][0] * matr[1][1] - matr[0][1] * matr[1][0];
        return sum;
    }
    else if (N == 1) {
        return matr[0][0];
    }
    for(int i = 0; i< N; i++)
    {
        mul= 1;
        int k = i;
        for(int j = 0; j< N; j++)
        {
            mul *= matr[j][k];
            k = (k == N-1) ? 0 : k + 1;
        }
        sum+= mul;
    }
    for(int i = 0; i< N; i++)
    {
        mul = 1;
        int k = i;
        for(int j =0; j< N; j++)
        {
            mul *= matr[j][k];
            k = (k == 0) ? N - 1 : k - 1;
        }
        sum -= mul;
    }
    return sum;
}

int arr_append(Vector** arr, Vector el, int* arr_tmp_size, int* arr_max_size) {
    if (*arr_tmp_size == *arr_max_size) {
        *arr_max_size *= 2;
        Vector* tmp = (Vector*)realloc(*arr, sizeof(Vector) * *arr_max_size);
        if (tmp == NULL) {
            return MEMORY_ISSUES;
        }
        *arr = tmp;
    }
    (*arr)[*arr_tmp_size] = el;
    (*arr_tmp_size)++;
    return ok;
}
 
int solve(
    const unsigned int n, const double eps,
    int (*norm1)(const Vector, double*),
    int (*norm2)(const Vector, const int, double*), const int p,
    int (*norm3)(const Vector, const Matrix, const double, double*), const Matrix matrix,
    int* res1_size, Vector** res1,
    int* res2_size, Vector** res2,
    int* res3_size, Vector** res3,
    const unsigned int args_amount, ...
    ) {
    *res1_size = 0;
    *res2_size = 0;
    *res3_size = 0;
    *res1 = NULL;
    *res2 = NULL;
    *res3 = NULL;
    int res1_max_size = 16;
    int res3_max_size = 16;
    int res2_max_size = 16;
    *res1 = (Vector*)malloc(sizeof(Vector) * res1_max_size);
    if (*res1 == NULL) {
        return MEMORY_ISSUES;
    }
    *res2 = (Vector*)malloc(sizeof(Vector) * res2_max_size);
    if (*res2 == NULL) {
        return MEMORY_ISSUES;
    }
    *res3 = (Vector*)malloc(sizeof(Vector) * res3_max_size);
    if (*res3 == NULL) {
        return MEMORY_ISSUES;
    }

    va_list args;
    va_start(args, args_amount);

    Vector tmp;

    int st;
    double n_1 = 0.0;
    double n_2 = 0.0;
    double n_3 = 0.0;
    for (int i = 0; i < args_amount; i++) {
        tmp = va_arg(args, Vector);
        if (tmp.n != n) {
            return INVALID_PARAMETER;
        }

        double n1_tmp = 0.0;
        double n2_tmp = 0.0;
        double n3_tmp = 0.0;
        st = max_norm(tmp, &n1_tmp);
        if (st != ok) {
            return st;
        }

        st = p_norm(tmp, p, &n2_tmp);
        if (st != ok) {
            return st;
        }

        st = matrix_norm(tmp, matrix, eps, &n3_tmp);
        if (st != ok) {
            return st;
        }

        if (fabs(n1_tmp - n_1) < eps) {
            st = arr_append(res1, tmp, res1_size, &res1_max_size);
            if (st != ok) {
                return st;
            }
        }
        else if (n1_tmp > n_1) {
            *res1_size = 0;
            st = arr_append(res1, tmp, res1_size, &res1_max_size);
            if (st != ok) {
                return st;
            }
            n_1 = n1_tmp;
        }

        if (fabs(n2_tmp - n_2) < eps) {
            st = arr_append(res2, tmp, res2_size, &res2_max_size);
            if (st != ok) {
                return st;
            }
        }
        else if (n2_tmp > n_2) {
            *res2_size = 0;
            st = arr_append(res2, tmp, res2_size, &res2_max_size);
            if (st != ok) {
                return st;
            }
            n_2 = n2_tmp;
        }

        if (fabs(n3_tmp - n_3) < eps) {
            st = arr_append(res3, tmp, res3_size, &res3_max_size);
            if (st != ok) {
                return st;
            }
        }
        else if (n3_tmp > n_3) {
            *res3_size = 0;
            st = arr_append(res3, tmp, res3_size, &res3_max_size);
            if (st != ok) {
                return st;
            }
            n_3 = n3_tmp;
        }
    }
    return ok;
}

int main() {
    Vector vec;
    Vector vec2;
    //Matrix matrix;
    Matrix mat_positive_defined;
    int st = create_vector(&vec, 2, 1.0, 4.0);
    //st = create_matrix(&mat_positive_defined, 3, 3, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0);
    //st = create_matrix(&mat_positive_defined, 2, 2, 1.0, 1.0, 1.0, 3.0);
    st = create_vector(&vec2, 2, 2.0, 3.0);
    st = create_matrix(&mat_positive_defined, 3, 3, 5.0, 2.0, -1.0, 2.0, 1.0, -1.0, -1.0, -1.0, 3.0);
// 3 7

    Vector* ans1;
    Vector* ans2;
    Vector* ans3;
    int size1 = 0;
    int size2 = 0;
    int size3 = 0;

    st = solve(2, 0.00001, max_norm, p_norm, 1, matrix_norm, mat_positive_defined, &size1, &ans1, &size2, &ans2, &size3, &ans3, 2, vec, vec2);
    if (st != ok) {
        print_error(st);
        free_vector(&vec);
        free_vector(&vec2);
        free_matrix(&mat_positive_defined);
        free(ans1);
        free(ans2);
        free(ans3);
        return 1;
    }
    
    print_vector_array(ans1, size1);
    print_vector_array(ans2, size2);
    print_vector_array(ans3, size3);
    //print_matrix(matrix);
    free_vector(&vec);
    free_vector(&vec2);
    free_matrix(&mat_positive_defined);
    free(ans1);
    free(ans2);
    free(ans3);

    return 0;
}
