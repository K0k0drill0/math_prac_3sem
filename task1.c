#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

#define ull unsigned long long int

static char *Error_Names[] = {
    "Error: wrong input data.",
    "Error: overflow",
};

enum func_statuses {
    OK = -1,
    WRONG_INP_PARAMETR,
    OWERFLOW,
};

int func_h(int num) {
    if (num <= 0 || num > 100) {
        return WRONG_INP_PARAMETR;
    }

    for (int i = num; i <= 100; i += num) {
        printf("%d\n", i);
    }
    return OK;
}

int func_p(int num) {
    num = abs(num);
    if (num == 0 || num == 1) {
        return -1;
    }
    for (int i = 2; i <= (int)sqrt(num); i+=2) {
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

int func_e(int num) {
    if (num < 1) {
        return WRONG_INP_PARAMETR;
    }
    printf("|\t\tbase\t\t|\t\tpower\t\t|\t\tvalue\t\t|\n");
    ull outp = 1;
    for (int i = 1; i <= 10; i++) {
        for (int j = 1; j <= num; j++) {
            outp *= i;
            printf("|\t\t%d\t\t|\t\t%d\t\t|\t\t%llu\t\t\n", i, j, outp);
        }
        outp = 1;
    }
    return OK;
}

int func_a(int num, ull* ans) {
    if (num < 1) {
        return WRONG_INP_PARAMETR;
    }
    for (ull i = 1; i <= num; i++) {
        if (ULLONG_MAX - i < *ans) {
            return OWERFLOW;
        }
        *ans += i;
    }
    return OK;
}



int func_f(int num, ull* ans) {
    if (num < 0) {
        return WRONG_INP_PARAMETR;
    }
    if (num == 0 || num == 1) {
        *ans = 1;
        return OK;
    }
    for (int i = 2; i <= num; i++) {
        if (ULLONG_MAX / i < *ans) {
            return OWERFLOW;
        }
        *ans *= i;
    }
    return OK;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("Input error: the amount of arguments is not correct.\n");
        return 1;
    }
    if (strlen(argv[2]) != 2 || (argv[2][0] != '-' && argv[2][0] != '/') || 
    (argv[2][1] != 'h' && argv[2][1] != 'p' && argv[2][1] != 's' &&
    argv[2][1] != 'e' && argv[2][1] != 'a' && argv[2][1] != 'f')) {
        printf("Input error: wrong flag.\n");
        return 1;
    }
    for (ull i = 0; i < strlen(argv[1]); i++) {
        if (!isdigit(argv[1][i])) {
            if (i == 0 && argv[1][i] == '-') {
                continue;
            }
            printf("Input error: second argument should be a number.\n");
            return 1;
        }
    }
    if (argv[2][1] == 'e' && atoi(argv[1]) > 10) {
        printf("Input error: the number is too big.\n");
        return 1;
    }

    if (argv[2][1] == 'h') {
        switch (func_h(atoi(argv[1])))
        {
        case WRONG_INP_PARAMETR:
            printf("%s\n", Error_Names[WRONG_INP_PARAMETR]);
            break;
        default:
            break;
        }
    }
    else if (argv[2][1] == 'p') {
        switch (func_p(atoi(argv[1])))
        {
        case 1:
            printf("The number is prime\n");
            break;
        case 0:
            printf("The number is not prime\n");
        case -1:
            printf("The number is neither prime nor composite.\n");
        default:
            break;
        }
    }
    else if (argv[2][1] == 's') {
        func_s(argv[1]);
    }
    else if (argv[2][1] == 'e') {
        switch (func_e(atoi(argv[1])))
        {
        case WRONG_INP_PARAMETR:
            printf("%s\n", Error_Names[WRONG_INP_PARAMETR]);
            break;
        default:
            break;
        }
    }
    else if (argv[2][1] == 'a') {
        ull ans = 0;
        switch (func_a(atoi(argv[1]), &ans))
        {
        case OK:
            printf("%llu\n", ans);
            break;
        case OWERFLOW:
            printf("%s\n", Error_Names[OWERFLOW]);
            break;
        case WRONG_INP_PARAMETR:
            printf("%s\n", Error_Names[WRONG_INP_PARAMETR]);
            break;
        default:
            break;
        }
    }
    else if (argv[2][1] == 'f') {
        ull ans = 1;
        switch (func_a(atoi(argv[1]), &ans))
        {
        case OK:
            printf("%llu\n", ans);
            break;
        case OWERFLOW:
            printf("%s\n", Error_Names[OWERFLOW]);
            break;
        case WRONG_INP_PARAMETR:
            printf("%s\n", Error_Names[WRONG_INP_PARAMETR]);
            break;
        default:
            break;
        }
    }

}
