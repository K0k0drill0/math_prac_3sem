#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

typedef unsigned long long ull;

const char *Error_names[] = {
    "Error: wrong amount of arguments.",
    "Error: wrong epsilon.",
    "Error: wrong argument(s).",
    "Error: wrong x."
};

enum Errors{
    ok = -1,
    WRONG_AMOUNT_OF_ARGUMENTS,
    WRONG_EPS,
    WRONG_ARGUMENTS,
    WRONG_X
};

double fsum_a(double eps, double x) {
    double sum = 1.0;
    double res2 = 0;
    int n = 0;
    do {
        res2 += sum;
        n++;
        sum *= (x / n);
    } while (fabs(sum) >= eps);
    return res2;
} 

double fsum_b(double eps, double x) {
    double ans = 0;
    int n = 0;
    double sum = 1;
    do {
        ans += sum;
        n++;
        sum *= (- x * x) / ((2.0 * n) * (2.0 * n - 1.0));
    } while (fabs(sum) >= eps);
    return ans;
}

double fsum_c(double eps, double x) {
    int n = 0;
    double sum = 1;
    double ans = 0;
    double three_2 = 9.0;
    double x_2 = x * x;
    do {
        ans += sum;
        sum *= (three_2 * x_2 * (n + 1.0) * (n + 1.0)) / ((3.0 * n + 1.0) * (3.0 * n + 2.0));
        n++; 
    } while (fabs(sum) >= eps);
    return ans;
}

double fsum_d(double eps, double x) {
    int n = 1;
	double ans = 0, sum = -x*x / 2;
	do
	{
		ans += sum;
		++n;
		sum *= -(2.0*n - 1) * x*x / (2.0*n);
		
	} while (fabs(sum) >= eps);
	
	return ans;
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

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("%s\n", Error_names[WRONG_AMOUNT_OF_ARGUMENTS]);
        return 1;
    }

    if (!is_float(argv[1]) || !is_float(argv[2])) {
        printf("%s\n", Error_names[WRONG_ARGUMENTS]);
        return 1;
    }

    double x = atof(argv[2]);
    double eps = atof(argv[1]);
    if (eps <= 0.0) {
        printf("%s\n", Error_names[WRONG_EPS]);
        return 1;
    }

    printf("sum_a = %.10f\n", fsum_a(eps, x));
    printf("sum_b = %.10f\n", fsum_b(eps, x));
    if (fabs(x) < 1.0) {
        printf("sum_c = %.10f\n", fsum_c(eps, x));
    }
    else {
        printf("The series c converges only at |x| < 1\n");
    }
    if (fabs(x) <= 1.0) {
        printf("sum_d = %.10f\n", fsum_d(eps, x));
    }
    else {
        printf("The series d converges only at |x| <= 1\n");
    }
}