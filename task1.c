#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

#define ull unsigned long long int

void func_h(int num) {
    if (num <= 0 || num > 100) {
        printf("No such numbers.\n");
        return;
    }

    for (int i = num; i <= 100; i += num) {
        printf("%d\n", i);
    }
}

bool func_p(int num) {
    for (int i = 2; i <= (int)sqrt(num); i++) {
        if (num % i == 0) {
            return 0;
        }
    }
    return 1;
}

void func_s(char* str) {
    for (ull i = 0; i < strlen(str); i++) {
        printf("%c ", str[i]);
    }
    printf("\n");
}

ull my_pow(int x, int y) {
    ull ans = 1;
    for (int i = 0; i < y; i++) {
        ans *= x;
    }
    return ans;
}

void func_e(int num) {
    if (num < 1) {
        printf("No such numbers.\n");
        return;
    }
    printf("|\t\tbase\t\t|\t\tpower\t\t|\t\tvalue\t\t|\n");
    for (int i = 1; i <= 10; i++) {
        for (int j = 1; j <= num; j++) {
            printf("|\t\t%d\t\t|\t\t%d\t\t|\t\t%llu\t\t\n", i, j, my_pow(i, j));
        }
    }
}

ull func_a(int num) {
    if (num < 1) {
        printf("No such number.\n");
        return 0;
    }
    ull ans = 0;
    for (ull i = 1; i <= num; i++) {
        if (ULLONG_MAX - i < ans) {
            printf("Error: owerflow.\n");
            return 0;
        }
        ans += i;
    }
    return ans;
}

ull func_f(int num) {
    if (num == 0 || num == 1) {
        return 1;
    }
    ull ans = 1;
    for (int i = 2; i <= num; i++) {
        if (ULLONG_MAX / i < ans) {
            printf("Error: owerflow.\n");
            return 0;
        }
        ans *= i;
    }
    return ans;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("Input error: the amount of arguments is not correct.\n");
        return 0;
    }
    if (strlen(argv[2]) != 2 || (argv[2][0] != '-' && argv[2][0] != '/') || 
    (argv[2][1] != 'h' && argv[2][1] != 'p' && argv[2][1] != 's' &&
    argv[2][1] != 'e' && argv[2][1] != 'a' && argv[2][1] != 'f')) {
        printf("Input error: wrong flag.\n");
        return 0;
    }
    for (ull i = 0; i < strlen(argv[1]); i++) {
        if (!isdigit(argv[1][i])) {
            if (i == 0 && argv[1][i] == '-') {
                continue;
            }
            printf("Input error: second argument should be a number.\n");
            return 0;
        }
    }
    if (argv[2][1] == 'e' && atoi(argv[1]) > 10) {
        printf("Input error: the number is too big.\n");
        return 0;
    }

    if (argv[2][1] == 'h') {
        func_h(atoi(argv[1]));
    }
    else if (argv[2][1] == 'p') {
        if (func_p(atoi(argv[1]))) {
            printf("The number is prime\n");
        }
        else {
            printf("The number is not prime\n");
        }
    }
    else if (argv[2][1] == 's') {
        func_s(argv[1]);
    }
    else if (argv[2][1] == 'e') {
        func_e(atoi(argv[1]));
    }
    else if (argv[2][1] == 'a') {
        if (func_a(atoi(argv[1]))) {
            printf("%llu\n", func_a(atoi(argv[1])));
        }
    }
    else if (argv[2][1] == 'f') {
        if (func_f(atoi(argv[1]))) {
            printf("%llu", func_f(atoi(argv[1])));
        }
    }

}
