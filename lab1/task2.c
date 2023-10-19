#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

static char *Error_names[] = {
    "Wrong amount of arguments.",
    "Wrong epsilon.",
    "Wrong argument(s)."
};

enum Errors{
    ok = -1,
    WRONG_AMOUNT_OF_ARGUMENTS,
    WRONG_EPS,
    WRONG_ARGUMENTS
};

int is_prime(int num) {
    //num = abs(num);
    if (num <= 0) {
        return -1;
    }
    if (num == 2) return 1;
    if (!(num & 1)) return 0;
    for (int i = 3; i <= (int)sqrt(num); i+=2) {
        if (num % i == 0) {
            return 0;
        }
    }
    return 1;
}

double e_1(double eps) {
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

double e_2(double eps) {
    double ans1 = 2;
    double ans2 = ans1;
    double fact = 1;
    int n = 1;
    do {
        n++;
        ans1 = ans2;
        fact *= n;
        ans2 += 1/fact;
        //printf("%f, %f\n", ans1, ans2);
    } while(fabs(ans1 - ans2) > eps);
    return ans2;
}

double e_3(double eps) {
    double x0 = 2;
    double x1 = x0;
    do {
        x0 = x1;
        x1 = x0 - (log(x0) - 1) * x0;
    } while (fabs(x1 - x0) > eps);
    return x1;
}

double pi_1(double eps) {
    int n = 1;
    double res1 = 4;
    double res2 = res1;
    do {
        res1 = res2;
        res2 = res2 * 4 * (n + 1) * n / ((2 * n + 1)*(2 * n + 1));
        n++;
        //printf("%lf, %lf\n", res1, res2);
    } while (fabs(res1 - res2) > eps);
    return res2;
}

double pi_2(double eps) {
    int n = 1;
    int pow_one = 1;
    double res1 = pow_one / (2 * n - 1);
    double res2 = res1;
    do {
        n++;
        pow_one *= -1;
        res1 = res2;
        double pl_el = (double)pow_one / (double)(2 * n - 1);
        res2 += pl_el; 
    } while (fabs(res2 - res1) > eps);
    return 4 * res2;
}

double pi_3(double eps) {
    double x0 = 3;
    double x1 = x0;
    do {
        x0 = x1;
        x1 = x0 + (cos(x0) + 1) / (sin(x0)); 
    } while (fabs(x0 - x1) > eps);
    return x1;
}

double ln_1(double eps) {
    int n = 1;
    double res1 = 1;
    double res2 = res1;
    do {
        n++;
        res1 = res2;
        res2 = n * (pow(2, 1.0/(double)n) - 1);
    } while (fabs(res2 - res1) > eps);
    return res2;
}

double ln_2(double eps) {
    int n = 1;
    double res1 = 1;
    double res2 = res1;
    double m_one_pow = 1;
    do {
        n++;
        m_one_pow *= -1;
        res1 = res2;
        res2 += (double)m_one_pow/(double)n;
    } while (fabs(res2 - res1) > eps);
    return res2;
}

double ln_3(double eps) {
    double e = e_3(eps);
    double x0 = 1.0;
    double x1 = x0 - ((pow(e, x0) - 2.0) / (pow(e, x0)));
    do {
        x0 = x1;
        x1 = x0 - ((pow(e, x0) - 2.0) / (pow(e, x0)));
    } while (fabs(x0 - x1) > eps);
    return x1;
}

double root_1(double eps) {
    double x0 = -0.5;
    double x1 = x0 - x0 * x0 / 2 + 1;

    do {
        x0 = x1;
        x1 = x0 - x0 * x0 / 2 + 1;
    } while (fabs(x1 - x0) > eps);
    return x1;
}

double root_2(double eps) {
    double res1 = 1;
    double res2 = pow(2, 0.25);
    double tmp = 1;
    do {
        res1 = tmp;
        tmp *= res2;
        res2 = pow(res2, 0.5);
    } while (fabsl(tmp - res1) > eps);
    return tmp;
}

double root_3(double eps) {
    double x0 = 2;
    double x1 = x0 - (x0 * x0 - 2) / (2 * x0);
    do {
        x0 = x1;
        x1 = x0 - (x0 * x0 - 2) / (2 * x0);
    } while (fabs(x1 - x0) > eps);
    return x1;
}

double gamma_1(double eps) {
    int m = 1;
    double res1 = 0;
    double res2 = 0;

    do {
        m++;
        res1 = res2;
        res2 = 0;
        double pl = 0;
        double combinations = m;
        double lg = 0;

        for (int k = 2; k <= m; k++) {
            combinations *= (m-k+1.0) / k;
            lg += log(k);

            pl = combinations * lg / k;
            if (k & 1) pl *= -1;
            res2 += pl;
        }
    } while (fabs(res1 - res2) > eps);
    return res2;
}

double gamma_2(double eps) {
    double pi = pi_3(eps);
    int k = 2;
    double res1 = (1.0 / pow(floor(sqrt(k)), 2)) - 1.0 / k;
    double res2 = res1;
    long long tmp;
    do {
        k++;
        res1 = res2;
        res2 += (1.0 / pow(floor(sqrt(k)), 2)) - 1.0 / k;
        //printf("%lf, %lf\n", res1, res2);
        tmp = (int)sqrt(k);
    } while (fabs(res1 - res2) > eps || (tmp * tmp == k));
    return res2 - pi * pi / 6;
}

double gamma_3(double eps) {
    int t = 2;
    double prime_product = 0.5;

    double res1 = 0.0;
    double res2 = -log(0.5 * log(2));
    do {
        t++;
        res1 = res2;
        if (is_prime(t)) prime_product *= (t-1.0)/t;
        res2 = -log(prime_product * log(t));
    } while(fabsl(res2 - res1) >= eps);
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

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("%s\n", Error_names[WRONG_AMOUNT_OF_ARGUMENTS]);
        return 1;
    }

    //char* epsilon = "-0;12";

    if (!is_float(argv[1])) {
        printf("%s\n", Error_names[WRONG_EPS]);
        return 1;
    }

    double eps = atof(argv[1]);

    if (eps <= 0.0) {
        printf("%s\n", Error_names[WRONG_EPS]);
        return 1;
    }

    printf("e = %.10lf (limit)\n", e_1(eps));
	printf("e = %.10lf (series)\n", e_2(eps));
	printf("e = %.10lf (equation)\n\n", e_3(eps));
	printf("pi = %.10lf (limit)\n", pi_1(eps));
	printf("pi = %.10lf (series)\n", pi_2(eps));
	printf("pi = %.10lf (equation)\n\n", pi_3(eps));
	printf("ln2 = %.10lf (limit)\n", ln_1(eps));
	printf("ln2 = %.10lf (series)\n", ln_2(eps));
	printf("ln2 = %.10lf (equation)\n\n", ln_3(eps));
	printf("sqrt2 = %.10lf (limit)\n", root_1(eps));
	printf("sqrt2 = %.10lf (series)\n", root_2(eps));
	printf("sqrt2 = %.10lf (equation)\n\n", root_3(eps));
	printf("gamma = %.10lf (limit)\n", gamma_1(eps));
	printf("gamma = %.10lf (series)\n", gamma_2(eps));
	printf("gamma = %.10lf (equation)\n", gamma_3(eps));
    
    return 0;
}