#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

static char *Error_names[] = {
    "Wrong amount of arguments.",
    "Wrong flag.",
    "Can not open a file.",
    "Invalid file.",
    "Can not allocate memory."
};

enum Errors{
    ok = -1,
    WRONG_AMOUNT_OF_ARGUMENTS,
    WRONG_FLAG,
    UNABLE_TO_OPEN_FILE, 
    INVALID_FILE, 
    MEMORY_ISSUES
};

int is_separator(char c) {
    return (c == ' ' || c == '\t' || c == '\n');
}

int which_number(char c) {
    if (isdigit(c)) {
        return (int)c - 48;
    }
    if (isalpha(c)) {
        c = toupper(c);
        return (int)c - 55;
    }
    return -1;
}

bool is_float(char* str) {
    int n = strlen(str);
    if (which_number(str[0]) < 0 && str[0] != '-') return false;
    int dot_cnt = 0;
    for (int i = 1; i < n; i++) {
        if (which_number(str[i]) < 0) {
            if (str[i] == '.') dot_cnt++;
            else return false;
            if (dot_cnt > 1) return false;
        }
    }
    return true;
}

double to_decimal(char* num, int len, int base) {
    double decimal = 0;
    //int len = strlen(num);
    int is_negative = 0;
    int decimal_point_index = -1;
    
    // Check if number is negative
    if (num[0] == '-') {
        is_negative = 1;
        //memmove(num, num+1, len); // Remove negative sign from string
        //len--;
    }
    
    // Check if number has a decimal point
    for (int i = 0; i < len; i++) {
        if (num[i] == '-') continue;
        if (num[i] == '.') {
            decimal_point_index = i;
            break;
        }
    }
    
    // Convert integer part of number to decimal
	int base_power = 1;
    for (int i = (decimal_point_index == -1 ? len-1 : decimal_point_index-1); i >= 0; i--) {
        if (num[i] == '-') continue;
        int digit = which_number(num[i]);
        decimal += digit * base_power;
        base_power *= base;
    }
    
    // Convert fractional part of number to decimal
    if (decimal_point_index != -1) {
        double base_power = 1.0/(double)base;
        for (int i = decimal_point_index+1; i < len; i++) {
            //if (num[i] == '-') continue;
            int digit = which_number(num[i]);
            decimal += (double)digit * base_power;
            base_power /= base;
        }
    }
    
    // Add negative sign if necessary
    if (is_negative) {
        decimal *= -1;
    }
    
    return decimal;
}

// int to_decimal(char *str_old, int old_size, int notation) {
//     int num_new = 0;
//     int base_power = 1;
//     for (int i = old_size - 1; i >= 0; i--) {
//         int dig = which_number(str_old[i]);

//         num_new += base_power * dig;
//         base_power *= notation;
//     }
//     return num_new;
// }

int solve(FILE* inp, FILE* outp) {
    char c = fgetc(inp);
    while (c != EOF) {
        while (is_separator(c)) {
            c = fgetc(inp);
        }
        if (c == EOF) {
            continue;
        }

        char* str_old;
        int str_old_max_sz = 100;
        str_old = (char*)malloc(sizeof(char) * str_old_max_sz);
        if (str_old == NULL) {
            return MEMORY_ISSUES;
        }
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
                if (str_old == NULL) {
                    return MEMORY_ISSUES;
                }
                str_old[str_ind] = c;
                int tmp = which_number(c);
                if (tmp > notation) notation = tmp;
            }
            c = fgetc(inp);
        }
        str_ind++;
        str_old[str_ind] = '\0';
        notation++;

        if (!is_float(str_old)) return INVALID_FILE;

        double num_new = to_decimal(str_old, str_ind, notation);

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

        fprintf(outp, " %d %lf ", notation, num_new);
        free(str_old);
    }
    return ok;
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

    // inp = fopen("input.txt", "r");
    // outp = fopen("output.txt", "w+");

    if (inp == NULL || outp == NULL) {
        fclose(inp);
        fclose(outp);
        printf("%s\n", Error_names[UNABLE_TO_OPEN_FILE]);
        return -2;
    }

    switch (solve(inp, outp))
    {
    case INVALID_FILE:
        fclose(inp);
        fclose(outp);
        printf("%s\n", Error_names[INVALID_FILE]);
        return 1;
        break;
    case MEMORY_ISSUES:
        fclose(inp);
        fclose(outp);
        printf("%s\n", Error_names[MEMORY_ISSUES]);
        return 1;
        break;
    case ok:
        fclose(inp);
        fclose(outp);
        return 0;
        break;
    default:
        break;
    }
    
}