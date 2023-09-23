#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#define ull unsigned long long

static char *Error_names[] = {
    "Wrong amount of arguments.",
    "Wrong epsilone.",
};

enum Errors{
    ok = -1,
    WRONG_AMOUNT_OF_ARGUMENTS,
    WRONG_EPS,
};

double fsum_a(double eps, double x) {
    int n = 0;
    int n_fact = 1;
    double x_pow = 1;
    double res1 = x_pow/n_fact;
    double res2 = res1;
    do {
        n++;
        n_fact *= n;
        x_pow *= x;
        res1 = res2;
        res2 += (x_pow / (double)n_fact);
        //printf("%f, %f, %f\n", res1, res2, pl_el);
    } while (fabs(res1-res2) > eps);
    return res2;
}

double fsum_b(double eps, double x) {
    //double xsq = x*x;
    int n = 0;
    int m_one_pow = 1;
    int tn_fact = 1;
    double x_pow = 1;
    double res1 = (double)m_one_pow * x_pow * x_pow / (double)tn_fact;
    double res2 = res1;
    double pl_res;
    do {
        n++;
        m_one_pow *= -1;
        x_pow *= x;
        tn_fact = tn_fact * (2 * n - 1) * (2 * n);
        res1 = res2;
        pl_res = (double)m_one_pow * x_pow * x_pow / (double)tn_fact;
        res2 += pl_res;
        // printf("%d %f %d\n", m_one_pow, x_pow * x_pow, tn_fact);
        // printf("%f, %f, %f\n", res1, res2, pl_res);
    } while (fabs(res1-res2) > eps);
    return res2;
}

double fsum_c(double eps, double x) {
    int n = 0;
    ull three_pow = 1;
    double n_fac = 1;
    double x_pow = 1;
    double tn_fac = 1;
    double res1 = pow(three_pow, 3) * pow(n_fac, 3) * pow(x_pow, 2) / tn_fac;
    double res2 = res1;
    double pl_res;
    do {
        n++;
        n_fac *= n;
        x_pow *= x;
        three_pow *= 3;
        tn_fac = tn_fac * (3 * n - 2) * (3 * n - 1) * (3 * n); 
        res1 = res2;
        pl_res = pow(three_pow, 3) * pow(n_fac, 3) * pow(x_pow, 2) / tn_fac;
        res2 += pl_res;
        // printf("%d, %llu, %f, %f, %f, %f\n", n, three_pow, n_fac, x_pow, tn_fac, pl_res);
        // printf("%f, %f\n", res1, res2);
    } while (fabs(res1-res2) > eps);
    return res2;
}

double fsum_d(double eps, double x) {
    int n = 1;
    int m_one_pow = -1;
    double odd_fac = 1;
    double even_fac = 2;
    double x_pow = x;
    double pl_res;
    double res1 = m_one_pow * odd_fac * x_pow * x_pow / even_fac;
    double res2 = res1;
    do {
        n++;
        m_one_pow *= -1;
        odd_fac *= 2 * n - 1;
        even_fac *= 2 * n;
        x_pow *= x;
        pl_res = m_one_pow * odd_fac * x_pow * x_pow / even_fac;
        res1 = res2;
        res2 += pl_res;
        // printf("%d, %d, %f, %f, %f, %f\n", n, m_one_pow, odd_fac, even_fac , x_pow, pl_res);
        // printf("%f, %f\n", res1, res2);
    } while (fabs(res1-res2) > eps);
    return res2;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("%s\n", Error_names[WRONG_AMOUNT_OF_ARGUMENTS]);
        return 1;
    }
    for (int i = 0; i < strlen(argv[1]); i++) {
        if (i == 0 && !isdigit(argv[1][i])) {
            printf("%s\n", Error_names[WRONG_EPS]);
            return 1;
        }
        int dot_c = 0;
        if (!isdigit(argv[1][i]) && argv[1][i] != '.') {
            printf("%s\n", Error_names[WRONG_EPS]);
            return 1;
        }
        if (argv[1][i] == '.') dot_c++;
        if (dot_c > 1) {
            printf("%s\n", Error_names[WRONG_EPS]);
            return 1;
        }
    }

    double eps = atof(argv[1]);
    double x;
    printf("Input the x.\n");
    scanf("%lf", &x);

    printf("sum_a = %f\n", fsum_a(eps, x));
    printf("sum_b = %f\n", fsum_b(eps, x));
    if (fabs(x) < 1.0) {
        printf("sum_c = %f\n", fsum_c(eps, x));
    }
    else {
        printf("The series c converges only at |x| < 1\n");
    }
    if (fabs(x) <= 1.0) {
        printf("sum_d = %f\n", fsum_d(eps, x));
    }
    else {
        printf("The series d converges only at |x| <= 1\n");
    }
}
