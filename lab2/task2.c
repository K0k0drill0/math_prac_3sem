#include <stdio.h>
#include <stdarg.h>
#include <limits.h>
#include <math.h>
#include <errno.h>

enum status_codes{
    ok, 
    OVERFLOWED, 
    INVALID
};

void print_error(int st) {
    switch (st)
    {
    case OVERFLOWED:
        printf("OverflowED!\n");
        break;
    case INVALID:
        printf("Invalid input.\n");
        break;
    default:
        break;
    }
}

int geometric_mean(double* ans, int amount, ...) {
    *ans = 1.0;
    if (amount == 0) {
        return 0.0;
    }
    if (amount < 0) {
        return INVALID;
    }
    va_list param;
    va_start(param, amount);
    for (int i = 0; i < amount; i++) {
        double umn = va_arg(param, double);
        *ans *= umn;
    }
    *ans = pow(*ans, 1.0/amount);
    va_end(param);
    return ok;
}

double power(double x, int n) {
    if (n == 0) return 1.0;
    if (n == 1) return x;
    if (n == -1) return 1.0 / x;
    double half = power(x, n / 2);
    if (!(n & 1)) {
        return half * half;
    } else if (n > 0) {
        return half * half * x;
    } else {
        return half * half * (1.0 / x);
    }
}
 
int main() {
    double ans;
    int st = geometric_mean(&ans, 3, 1.0, 2.0, 3.0);
    if (st != ok) {
        print_error(st);
        return 1;
    }

    printf("%lf\n", ans);

    ans = power(-1.0, 110133);
    printf("%lf\n", ans);

    if (errno == ERANGE){
        printf("Bruh\n");
    }

    return 0;
}