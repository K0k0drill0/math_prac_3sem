#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

enum solve_statuses {
    ONE_ROOT,
    TWO_ROOTS,
    NO_ROOTS
};

int solve_equation(double eps, double a, double b, double c, double* ans1, double* ans2) {
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
        default:
            break;
    }
}

void func_q(double eps, double a, double b, double c) {
    //double ans1, ans2;
    if (fabs(a-b) < eps && fabs(b-c) < eps && fabs(a-c) < eps) {
        pr_solve(eps, a, b, c);
    }
    else if (fabs(a-b) < eps) {
        pr_solve(eps, a, b, c);
        pr_solve(eps, a, c, b);
        pr_solve(eps, c, a, b);
    }
    else if (fabs(a-c) < eps) {
        pr_solve(eps, a, b, c);
        pr_solve(eps, b, a, c);
        pr_solve(eps, a, c, b);
    }
    else if (fabs(b-c) < eps) {
        pr_solve(eps, a, b, c);
        pr_solve(eps, b, a, c);
        pr_solve(eps, c, b, a);
    }
    else {
        pr_solve(eps, a, b, c);
        pr_solve(eps, a, c, b);
        pr_solve(eps, b, a, c);
        pr_solve(eps, b, c, a);
        pr_solve(eps, c, a, b);
        pr_solve(eps, c, b, a);
    }

}

bool func_m(double x, double y) {
    double eps = 0.000000001;
    return (fmod(x, y) < eps);
}

bool func_t(double eps, double a, double b, double c) {
    if (a < 0.0 || b < 0.0 || c < 0.0) {
        return 0;
    }
    return (fabs(a*a - b*b - c*c) < eps || fabs(b*b - a*a - c*c) < eps || (c*c - b*b - a*a) < eps);
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
        if (func_t(atof(argv[2]), atof(argv[3]), atof(argv[4]), atof(argv[5]))) {
            printf("Yes, these numbers can be the lengths of a right triangle\n");
        }
        else {
            printf("No, these numbers can not be the lengths of a right triangle\n");
        }
    }

}
