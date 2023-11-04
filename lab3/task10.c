#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>

enum status_code {
    ok,
    MEMORY_ISSUES,
    OVERFLOWED, 
    INVALID
};

void print_error(int st) {
    switch (st)
    {
    case MEMORY_ISSUES:
        printf("Unable to allocate the memory.\n");
        break;
    case OVERFLOWED:
        printf("Overflowed!\n");
        break;
    case INVALID:
        printf("INVALID!\n");
        break;
    default:
        break;
    }
}

double cut_sum(double* arr, int l, int r) {
    double ans = 0.0;
    for (int i = l; i <= r; i++) {
        ans += arr[i];
    }
    return ans;
}

void print_arr(double* arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%lf ", arr[i]);
    }
    printf("\n");
}

int polynomial_re_expansion(double epsilon, double a, double** coefficients_g, int degree, ...) {
    if (epsilon < 0.0 || degree < 0) {
        return INVALID;
    }
    va_list args;
    va_start(args, degree);

    double* coefficients_f = (double*)malloc(sizeof(double) * (degree+1));
    *coefficients_g = (double*)malloc(sizeof(double) * (degree+1));
    if (coefficients_f == NULL || *coefficients_g == NULL) {
        free(coefficients_f);
        return MEMORY_ISSUES;
    }

    double a_pow = 1;
    for (int i = 0; i <= degree; i++) {
        coefficients_f[i] = va_arg(args, double) * a_pow;
        a_pow *= a;
    }

    va_end(args);

    (*coefficients_g)[0] = cut_sum(coefficients_f, 0, degree);

    int k_fac = 1;

    for (int k = 1; k <= degree; k++) {
        k_fac *= k;
        if (errno == ERANGE) {
            free(coefficients_f);
            return OVERFLOWED;
        }
        (*coefficients_g)[k] = 0.0;
        for (int i = k; i <= degree; i++) {
            coefficients_f[i] = coefficients_f[i] * (i - k + 1) / a;
            (*coefficients_g)[k] += coefficients_f[i];
        }
        (*coefficients_g)[k] /= k_fac;
    }
    free(coefficients_f);
    return ok;
}

int main() {
    double* res;
    int st = polynomial_re_expansion(0.00001, 2.0, &res, 2, 1.0, 2.0, 3.0);
    if (st != ok) {
        print_error(st);
        free(res);
        return 1;
    }
    print_arr(res, 3);
    free(res);
    return 0;
}