#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <limits.h>
#include "validators.h"

int is_separator(const char c) {
    return (c == ' ' || c == '\t' || c == '\n');
}

int is_valid_uint(const char* str) {
    int ind = 0;
    while (str[ind] != '\0') {
        if (!isdigit(str[ind])) {
            return 0;
        }
        ind++;
    }
    return (ind == 0) ? 0 : 1;
}

int is_valid_int(const char* num) {
    int ind = 0;
    while (num[ind] != '\0') {
        if (!isdigit(num[ind]) && (ind != 0 || num[ind] != '-')) {
            return 0;
        }
        ind++;
    }
    return (ind != 0);
}

int is_valid_base(const char* num, int base) {
    int ind = 0;
    while (num[ind] != '\0') {
        if ((which_number(num[ind]) > base || which_number(num[ind]) == -1)
         && (ind != 0 || num[ind] != '-')) {
            return 0;
        }
        ind++;
    }
    return (ind != 0);
}

int is_runtime_error(const int er) {
    return (er >= INVALID_COMMENT && er <= UNINITIALISED_VARIABLE);
}

int which_operation(const char c1, const char c2) {
    if (c1 == '+' && c2 != '>') return DISJUNCTION;
    if (c1 == '&') return CONJUNCTION;
    if (c1 == '-' && c2 == '>') return IMPLICATION;
    if (c1 == '<' && c2 == '-') return REVERSED_IMPLICATION;
    if (c1 == '~') return EQUIVALENCE;
    if (c1 == '<' && c2 == '>') return MOD2;
    if (c1 == '+' && c2 == '>') return COIMPLICATION;
    if (c1 == '?') return SHEFFER;
    if (c1 == '!') return PIERCE;
    if (c1 == '\\') return NEGATION;
}

int next_symbol(const char* str, int* ind) {
    if (str == NULL) {
        return INVALID_FUNCTION_PARAMETER;
    }
    (*ind)++;
    //char c = str[*ind];
    while (is_separator(str[*ind])) {
        (*ind)++;
    } 
    return ok;
}

int is_valid_binary(const char* command) {
    if (command == NULL) {
        return 0;
    }

    int i = 0;
    if (!isalpha(command[i])) {
        return 0;
    }
    
    next_symbol(command, &i);
    if (command[i] != ':' || command[i+1] != '=') {
        return 0;
    }
    i++;
    next_symbol(command, &i);

    if (!isalpha(command[i])) {
        return 0;
    }

    next_symbol(command, &i);

    if (command[i] != '+' && command[i] != '&' &&
     command[i] != '?' && command[i] != '!' && command[i] != '~' && 
     (command[i] != '-' || command[i+1] != '>') && 
     (command[i] != '<' || command[i+1] != '-') &&
     (command[i] != '<' || command[i+1] != '>') && 
     (command[i] != '+' || command[i+1] != '>')) {
        return 0;
    }

    if (command[i+1] == '>' || command[i+1] == '-') {
        i++;
    }

    next_symbol(command, &i);
    if (!isalpha(command[i])) {
        return 0;
    }
    i++;
    if (command[i] != '\0') {
        return 0;
    }

    return 1;
}

int is_valid_unary(const char* command) {
    if (command == NULL) {
        return 0;
    }

    int i = 0;
    if (!isalpha(command[i])) {
        return 0;
    }
    
    next_symbol(command, &i);
    if (command[i] != ':' || command[i+1] != '=') {
        return 0;
    }
    i++;
    next_symbol(command, &i);

    if (command[i] != '\\') {
        return 0;
    }
    next_symbol(command, &i);

    if (!isalpha(command[i])) {
        return 0;
    }

    if (command[i+1] != '\0') {
        return 0;
    }

    return 1;
}

int is_valid_read(const char* command) {
    if (command == NULL) {
        return 0;
    }

    int i = 0;
    if (command[i] != 'r' || command[i+1] != 'e' || 
    command[i+2] != 'a' || command[i+3] != 'd') {
        return 0;
    }
    i+=3;

    next_symbol(command, &i);
    if (command[i] != '(') {
        return 0;
    }

    next_symbol(command, &i);

    if (!isalpha(command[i])) {
        return 0;
    }

    next_symbol(command, &i);
    if (command[i] != ',') {
        return 0;
    }

    next_symbol(command, &i);
    while (!is_separator(command[i]) && command[i] != ')'
     && command[i] != '\0') {
        if (!isdigit(command[i])) {
            return 0;
        }
        i++;
    }

    if (command[i] == '\0') {
        return 0;
    }

    if (command[i] != ')') {
        next_symbol(command, &i);
    }

    if (command[i] != ')') {
        return 0;
    }
    i++;
    if (command[i] != '\0') {
        return 0;
    }

    return 1;
}

int is_valid_write(const char* command) {
    if (command == NULL) {
        return 0;
    }

    int i = 0;
    if (command[i] != 'w' || command[i+1] != 'r' || 
    command[i+2] != 'i' || command[i+3] != 't' || command[i+4] != 'e') {
        return 0;
    }
    i+=4;

    next_symbol(command, &i);
    if (command[i] != '(') {
        return 0;
    }

    next_symbol(command, &i);

    if (!isalpha(command[i])) {
        return 0;
    }

    next_symbol(command, &i);
    if (command[i] != ',') {
        return 0;
    }

    next_symbol(command, &i);
    while (!is_separator(command[i]) && command[i] != ')'
     && command[i] != '\0') {
        if (!isdigit(command[i])) {
            return 0;
        }
        i++;
    }

    if (command[i] == '\0') {
        return 0;
    }

    if (command[i] != ')') {
        next_symbol(command, &i);
    }

    if (command[i] != ')') {
        return 0;
    }
    i++;
    if (command[i] != '\0') {
        return 0;
    }

    return 1;
}

int which_number(char c) {
    c = toupper(c);

    if (isdigit(c)) {
        return c - '0';
    }
    if (isalpha(c)) {
        return c - 'A' + 10;
    }
    return -1;
}

int to_decimal(char* str, int base, int* ans) {
    //char alphabet = "0123456789ABCDEFGHIJKLNMOPQRSTUVWXYZ";

    int i = 0;
    int sign = (str[i] == '-') ? -1 : 1;
    if (str[i] == '-') i++;
    int res = which_number(str[i]);
    i++;
    while (str[i] != '\0') {
        if (INT_MAX / base <= res) {
            return OVERFLOWED;
        }
        res *= base;
        if (INT_MAX - which_number(str[i]) < res) {
            return OVERFLOWED;
        }
        res += which_number(str[i]);
        i++;
    }

    res *= sign;
    *ans = res;
    return ok;
}

char* my_itoa(int decimal, int new_sn) {  
    const char alphabet[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    char* ans;
    int arr_size = 0;
    int sign = 1;

    if (new_sn < 2 || new_sn > 36) {
        new_sn = 10;
    }

    if (decimal < 0) {
        sign = -1;
        arr_size++;
    }
    decimal = abs(decimal);
    if (decimal == 0) {
        arr_size = 2;
    }
    else {
        arr_size += log(decimal) / log(new_sn) + 1;
    }

    ans = (char*)malloc(sizeof(char) * (arr_size+1));
    if (ans == NULL) {
        return NULL;
    }

    if (decimal == 0) {
        ans[0] = '0';
        ans[1] = '\0';
        return ans;
    }

    int i = arr_size-1;
    while (decimal > 0) {
        char remainder = alphabet[decimal % new_sn];
        ans[i] = remainder;
        i--;
        decimal /= new_sn;
    }
    if (sign == -1) {
        ans[i] = '-';
    } 
    ans[arr_size] = '\0';

    return ans;
}
