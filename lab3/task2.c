#include <stdio.h>
#include <stdarg.h>

typedef struct {
    double *coords;
} Vector;

double euclidean_norm(Vector v, int n) {
    double norm = 0;
    for (int i = 0; i < n; i++) {
        norm += v.coords[i] * v.coords[i];
    }
    return sqrt(norm);
}

double max_norm(Vector v, int n) {
    double norm = 0;
    for (int i = 0; i < n; i++) {
        double abs_coord = fabs(v.coords[i]);
        if (abs_coord > norm) {
            norm = abs_coord;
        }
    }
    return norm;
}

void find_longest_vectors(int num_norms, int n, Vector vectors[], double (*norm_functions[])(Vector, int)) {
    Vector longest_vectors[num_norms];
    double max_norms[num_norms];
    for (int i = 0; i < num_norms; i++) {
        max_norms[i] = 0;
    }
    for (int i = 0; i < n; i++) {
        Vector v = vectors[i];
        for (int j = 0; j < num_norms; j++) {
            double norm = norm_functions[j](v, n);
            if (norm > max_norms[j]) {
                max_norms[j] = norm;
                longest_vectors[j] = v;
            } else if (norm == max_norms[j]) {
                // If there is a tie, add the vector to the list of longest vectors
                longest_vectors[j + 1] = v;
            }
        }
    }
    for (int i = 0; i < num_norms; i++) {
        printf("Longest vector for norm %d:\n", i);
        Vector v = longest_vectors[i];
        for (int j = 0; j < n; j++) {
            printf("%f ", v.coords[j]);
        }
        printf("\n");
    }
}

int main() {
    int n = 3;
    Vector vectors[] = {
        {.coords = (double[]){1, 2, 3}},
        {.coords = (double[]){-4, 5, -6}},
        {.coords = (double[]){7, -8, 9}}
    };
    double (*norm_functions[])(Vector, int) = {euclidean_norm, max_norm};
    find_longest_vectors(2, n, vectors, norm_functions);
    return 0;
}