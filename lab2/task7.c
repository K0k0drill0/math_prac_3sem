#include <stdio.h>
#include <math.h>

enum status_code {
    ok,
    NO_ROOTS,
    WRONG_EPS, 
    INVALID
};

void print_error(int st) {
    switch (st)
    {
    case NO_ROOTS:
        printf("There are no roots, or more than one root.\n");
        break;
    case WRONG_EPS:
        printf("Wrong epsilon!\n");
        break;
    case INVALID:
        printf("Invalid!!!\n");
        break;
    default:
        break;
    }
}

double parab(double x) {
    return x * x - 2;
}
  
int dichotomy(double a, double b, double eps, double (*f)(double), double* root) {
    if (eps <= 0.0) {
        return WRONG_EPS;
    }
    if (b - a <= eps) {
        return INVALID;
    }
    double f_a = f(a);
    double f_b = f(b);
    if (f_a * f_b > eps) {
        return NO_ROOTS;
    }

    double c;
    while ((b - a) / 2 > eps) {
        c = (a + b) / 2;
        if (fabs(f(c)) < eps) {
            *root = c;
            return ok;
        } else if (f(a) * f(c) < eps) {
            b = c;
        } else {
            a = c;
        }
    }
    *root = (a + b) / 2;
    return ok;
}

int main() {
    double a1 = 0, b1 = 2, eps1 = 0.0001;
    double root1;
    int st = dichotomy(a1, b1, eps1, parab, &root1);
    if (st == ok) {
        printf("Root of x^2 - 2 on [%lf,%lf] with eps=%lf: %lf\n", a1, b1, eps1, root1);
    }
    else {
        print_error(st);
    }

    a1 = -6.0, b1 = -5.0, eps1 = 0.001;
    st = dichotomy(a1, b1, eps1, parab, &root1);
    if (st == ok) {
        printf("Root of x^2 - 2 on [%lf,%lf] with eps=%lf: %lf\n", a1, b1, eps1, root1);
    }
    else {
        print_error(st);
    }

    double a2 = 3, b2 = 4, eps2 = 0.00001;
    double root2;
    st = dichotomy(a2, b2, eps2, sin, &root2);
    if (st == ok) {
        printf("Root of sin(x) on [%lf,%lf] with eps=%lf: %lf\n", a2, b2, eps2, root2);
    }
    else {
        print_error(st);
    }



    return 0; 
}