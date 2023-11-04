#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

double machine_eps() {
    double eps = 1.0;
    while (1.0+eps > 1.0) {
        eps /= 2;
    }
    return eps;
}


int has_finite_representation(double num, int base) {
    if (base < 2) {
        return false;
    }

    double eps = machine_eps();

    int numerator, denominator = 1;
    while (num - floor(num) > eps) {
        num *= 10;
        denominator *= 10;
    }

    numerator = (int)num;
    int igcd = gcd(numerator, denominator);
    numerator /= igcd;
    denominator /= igcd;

    for (int i = 2; i <= denominator; i++) {
        if (denominator % i == 0) {
            if (base % i != 0) {
                return false;
            }
        }
        while (denominator % i == 0) {
            denominator /= i;
        }
    }
    return true; 
}

char* has_fin_more(int base, int amount, ...) {
    va_list args;
    va_start(args, amount);

    char* ans = (char*)malloc(sizeof(char) * (amount + 1));
    ans[amount] = '\0';

    for (int i = 0; i < amount; i++) {
        double num = va_arg(args, double);

        ans[i] = has_finite_representation(num, base) + '0';
    }
    va_end(args);

    return ans;
}

int main() {
    char* ans = has_fin_more(3, 3, 0.75, 0.2, 0.3);
    printf("is finite: %s\n", ans);
    free(ans);
    return 0;
}