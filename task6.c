#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef unsigned long long ull;

static char *Error_names[] = {
    "Error: wrong amount of arguments.",
    "Error: wrong epsilon.",
    "Error: wrong argument(s)."
};

enum Errors{
    ok = -1,
    WRONG_AMOUNT_OF_ARGUMENTS,
    WRONG_EPS,
    WRONG_ARGUMENTS
};

double e(double eps) {
    double res1 = 2;
    double res2 = res1;
    int n = 1;
    do {
        n++;
        res1 = res2;
        res2 = pow((1 + 1.0/n), n);
        //printf("%f\n", ans2);
    } while (fabs(res2 - res1) > eps);
    return res2;
}

bool is_float(char* str) {
    int n = strlen(str);
    if (!isdigit(str[0]) && str[0] != '-') return false;
    int dot_cnt = 0;
    for (int i = 1; i < n; i++) {
        if (!isdigit(str[i])) {
            if (str[i] == '.') dot_cnt++;
            else return false;
            if (dot_cnt > 1) return false;
        }
    }
    return true;
}

double function_a(double x) {
    return log(1.0 + x) / x;
}

double integral_a(double eps) {
    double delta_x = 1;
    double step = 1;
    double res2 = log(2);
    double res1;
    do {
        res1 = res2;
        step /= 2;
        delta_x /= 2;

        res2 = 0;
        for (double x = 1; x > 0; x -= step) {
            res2 += function_a(x) * delta_x;
        }
    } while (fabs(res2 - res1) > eps);
    return res2;
}

double function_b(double x) {
    return exp(- x * x / 2);
}

double integral_b(double eps) {
    double delta_x = 1;
    double step = 1;
    double res1 = function_b(1);
    double res2 = res1;
    do {
        res1 = res2;
        step /= 2;
        delta_x /= 2;

        res2 = 0;
        for (double x = 1; x > 0; x -= step) {
            res2 += function_b(x) * delta_x;
        }
    } while (fabs(res2 - res1) > eps);
    return res2;
}

double function_c(double x) {
    return log(1 / (1 - x));
} 

double integral_c(double eps) {
    double delta_x = 1;
    double step = 1;
    double res1 = function_c(0);
    double res2 = res1;
    do {
        res1 = res2;
        step /= 2;
        delta_x /= 2;

        res2 = 0;
        for (double x = 0; x < 1; x += step) {
            res2 += function_c(x) * delta_x;
        }
    } while (fabs(res2 - res1) > eps);
    return res2;
}

double function_d(double x) {
    return pow(x, x);
}

double integral_d(double eps) {
    double delta_x = 1;
    double step = 1;
    double res1 = function_d(0);
    double res2 = res1;
    do {
        res1 = res2;
        step /= 2;
        delta_x /= 2;

        res2 = 0;
        for (double x = 0.0; x <= 1.0; x += step) {
            res2 += function_d(x) * delta_x;
        }
    } while (fabs(res2 - res1) > eps);
    return res2;
} 

int main(int argc, char** argv) {
    // double eps = 0.;
    // printf("%lf\n", integral_a(0, 1, eps));
    if (argc != 2) {
        printf("%s\n", Error_names[WRONG_AMOUNT_OF_ARGUMENTS]);
        return 1;
    }

    if (!is_float(argv[1])) {
        printf("%s\n", Error_names[WRONG_ARGUMENTS]);
        return 1;
    }

    double eps = atof(argv[1]);
    if (eps <= 0.0) {
        printf("%s\n", Error_names[WRONG_EPS]);
        return 1;
    }

    printf("Integral a = %lf\n", integral_a(eps));
    printf("Integral b = %lf\n", integral_b(eps));
    printf("Integral c = %lf\n", integral_c(eps));
    printf("Integral d = %lf\n", integral_d(eps));

    return 0;
}