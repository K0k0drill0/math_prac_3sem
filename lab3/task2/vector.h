#ifndef VECTOR_H
#define VECTOR_H

enum status_code{
    ok, 
    MEMORY_ISSUES,
    INVALID_PARAMETER
};

typedef struct {
    int n;
    double* coords;
} Vector;

typedef struct {
    int m, n;
    double** data;
} Matrix;

void print_error(const int st);

int max_norm(const Vector vec, double* ans);
int p_norm(const Vector vec, const int p, double* ans);
int matrix_norm(const Vector vec, const Matrix matrix, const double eps, double* ans);

int is_positive_definite(const Matrix mat, const double eps);
int matrix_multiply_vector(const Matrix matrix, const Vector vec, Vector* ans);
int dot(const Vector v1, const Vector v2, double* ans);
double det(const Matrix matrix, const int N);

int create_vector(Vector* vec, const int n, ...);
int create_matrix(Matrix* matrix, const int m, const int n, ...);

void free_vector(Vector* vec);
void free_matrix(Matrix* matrix);

void print_vector(const Vector vec);
void print_vector_array(const Vector* arr, const int size);
void print_matrix(const Matrix matrix);


#endif