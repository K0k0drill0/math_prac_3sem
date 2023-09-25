#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

static char *Error_names[] = {
    "Wrong amount of arguments.",
    "Wrong epsilone.",
};

enum Errors{
    ok = -1,
    WRONG_AMOUNT_OF_ARGUMENTS,
    WRONG_EPS,
};

double e_1(double eps) {
    double ans1 = 2;
    double ans2 = ans1;
    int n = 1;
    do {
        n++;
        ans1 = ans2;
        ans2 = pow((1 + 1.0/n), n);
        //printf("%f\n", ans2);
    } while (fabs(ans2 - ans1) > eps);
    return ans2;
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
        printf("%f, %f\n", ans1, ans2);
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

double pi_2(double eps) {
    
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

// double gamma_2(double eps) {
//     double pi = pi_3(eps);
//     double ans1 = - pi*pi / 6;
//     double ans2 = ans1;
//     int k = 1;
//     do {
//         k++;

//     } while (fabs(ans1 - ans2) > eps && )
// }

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

    printf(" используем версию языка %ld\n", __STDC_VERSION__);
    printf("%.20lf\n", e_3(eps));
}