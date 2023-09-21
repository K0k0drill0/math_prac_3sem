#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

enum solve_statuses {
    ONE_ROOT,
    TWO_ROOTS,
    NO_ROOTS,
    A_IS_ZERO
};

int solve_equation(double eps, double a, double b, double c, double* ans1, double* ans2) {
    if (fabs(a) < eps) {
        return A_IS_ZERO;
    }
    double discr = (b * b - 4 * a * c);
    if (discr < 0.0) {
        return NO_ROOTS;
    }
    else if (discr < eps) {
        *ans1 = - b / (2 * a);
        return ONE_ROOT;
    }
    else {
        *ans1 = (- b - sqrt(discr)) / (2 * a);
        *ans2 = (- b + sqrt(discr)) / (2 * a);
        return TWO_ROOTS;
    }
}

void pr_solve(double eps, double a, double b, double c) {
    double ans1, ans2;
    switch (solve_equation(eps, a, b, c, &ans1, &ans2)) {
        case ONE_ROOT:
            printf("For a = %f, b = %f, c = %f:\n", a, b, c);
            printf("Root = %f\n", ans1);
            break;
        case TWO_ROOTS:
            printf("For a = %f, b = %f, c = %f:\n", a, b, c);
            printf("Root1 = %f, root2 = %f\n", ans1, ans2);
            break;
        case NO_ROOTS:
            printf("For a = %f, b = %f, c = %f:\n", a, b, c);
            printf("There are no real roots\n");
            break;
        case A_IS_ZERO:
            printf("For a = %f, b = %f, c = %f:\n", a, b, c);
            printf("This is not a quadratic equation.\n");
            break;
        default:
            break;
    }
}

void swap(int* arr, int ind1, int ind2) {
    int tmp = arr[ind1];
    arr[ind1] = arr[ind2];
    arr[ind2] = tmp; 
}

void func_q(double eps, double a, double b, double c) {
    int coeff[3] = {a, b, c};
    for (int i = 0; i < 3; i++) {
        swap(coeff, 0, 1);
        pr_solve(eps, coeff[0], coeff[1], coeff[2]);
        swap(coeff, 1, 2);
        pr_solve(eps, coeff[0], coeff[1], coeff[2]);
    }
}

bool func_m(double x, double y) {
    double eps = 0.000000001;
    return (fmod(x, y) < eps);
}

bool func_t(double eps, double a, double b, double c) {
    if (fabs(a) < eps || fabs(b) < eps || fabs(c) < eps) {
        return 0;
    }
    double sqra = a*a;
    double sqrb = b*b;
    double sqrc = c*c;
    return (fabs(sqra - sqrb - sqrc) < eps || fabs(sqrb - sqra - sqrc) < eps || (sqrc - sqrb - sqra) < eps);
}

int main(int argc, char** argv) {
    if (strlen(argv[1]) != 2 || (argv[1][0] != '-' && argv[1][0] != '/') ||
    (argv[1][1] != 'q' && argv[1][1] != 'm' && argv[1][1] != 't')) {
        printf("Input error: wrong flag.\n");
        return 1;
    }
    if ((argv[1][1] == 'q' && argc != 6) || (argv[1][1] == 'm' && argc != 4) ||
     (argv[1][1] == 't' && argc != 6)) {
        printf("Input error: wrong amount of arguments.\n");
        return 1;
    }
    
    if (argv[1][1] == 'q') {
        if (atof(argv[2]) < 0.0) {
            printf("Eps can not be negative.\n");
            return 1;
        }
        func_q(atof(argv[2]), atof(argv[3]), atof(argv[4]), atof(argv[5]));
    }
    if (argv[1][1] == 'm') {
        if (func_m(atof(argv[2]), atof(argv[3]))) {
            printf("Yes, the first number is a multiple of the second.\n");
        }
        else {
            printf("No, the first number is not a multiple of the second.\n");
        }
    }
    if (argv[1][1] == 't') {
         if (atof(argv[2]) < 0.0) {
            printf("Eps can not be negative.\n");
            return 1;
        }
        if (func_t(atof(argv[2]), atof(argv[3]), atof(argv[4]), atof(argv[5]))) {
            printf("Yes, these numbers can be the lengths of a right triangle\n");
        }
        else {
            printf("No, these numbers can not be the lengths of a right triangle\n");
        }
    }

}
