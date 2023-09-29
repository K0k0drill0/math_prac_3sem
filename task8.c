#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

static char *Error_names[] = {
    "Wrong amount of arguments.",
    "Wrong flag.",
    "Can not open a file."
};

enum Errors{
    ok = -1,
    WRONG_AMOUNT_OF_ARGUMENTS,
    WRONG_FLAG,
    INABLE_TO_OPEN_FILE
};

int is_separator(char c) {
    return (c == ' ' || c == '\t' || c == '\n');
}

int which_number(char c) {
    if (isdigit(c)) {
        return (int)c - 48;
    }
    c = toupper(c);
    return (int)c - 55;
}

int to_decimal(char *str_old, int old_size, int notation) {
    int num_new = 0;
    int base_power = 1;
    for (int i = old_size - 1; i >= 0; i--) {
        int dig = which_number(str_old[i]);

        num_new += base_power * dig;
        base_power *= notation;
    }
    return num_new;
}

void solve(FILE* inp, FILE* outp) {
    char c = fgetc(inp);
    while (c != EOF) {
        while (is_separator(c)) {
            c = fgetc(inp);
            continue;
        }

        char* str_old;
        int str_old_max_sz = 100;
        str_old = (char*)malloc(sizeof(char) * str_old_max_sz);
        str_old[0] = c;
        int str_ind = 0;

        int notation = 0;

        c = fgetc(inp);
        while (!is_separator(c) && c != EOF) {
            str_ind++;
            if (str_ind < str_old_max_sz - 1) {
                str_old[str_ind] = c;
                int tmp = which_number(c);
                if (tmp > notation) notation = tmp;
            }
            else {
                str_old_max_sz *= 2;
                str_old = (char*)realloc(str_old, str_old_max_sz);
            }
            c = fgetc(inp);
        }
        str_ind++;
        str_old[str_ind] = '\0';
        notation++;

        int num_new = to_decimal(str_old, str_ind, notation);

        bool flag = true;
        for (int i = 0; i < str_ind; i++) {
            if (flag && str_old[i] == '0') {
                continue;
            }
            if (flag && isdigit(str_old[i])) {
                flag = false;
            }
            fputc(str_old[i], outp);
        }

        fprintf(outp, " %d %d ", notation, num_new);
        free(str_old);
    }
}


int main(int argc, char** argv) {
    if (argc != 3) {
        printf("%s\n", Error_names[WRONG_AMOUNT_OF_ARGUMENTS]);
        return 1;
    }
    FILE* inp;
    FILE* outp;

    inp = fopen(argv[1], "r");
    outp = fopen(argv[2], "w+");

    if (inp == NULL || outp == NULL) {
        fclose(inp);
        fclose(outp);
        printf("%s\n", Error_names[INABLE_TO_OPEN_FILE]);
        return -2;
    }

    solve(inp, outp);
    return 0;
}