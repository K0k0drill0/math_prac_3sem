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

    if (!is_float(argv[1])) {
        printf("%s\n", Error_names[WRONG_ARGUMENTS]);
        return 1;
    }

    double eps = atof(argv[1]);
    if (eps <= 0.0) {
        printf("%s\n", Error_names[WRONG_EPS]);
        return 1;
    }

    return 0;
}