#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>

const char all_symbols[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z'
};

enum status_code{
    ok, 
    WRONG_BASE,
    MEMORY_ISSUES,
    INVALID_PARAMETER
};

void print_error(int st) {
    switch (st)
    {
    case WRONG_BASE:
        printf("Wrong base!!11!\n");
        break;
    case MEMORY_ISSUES:
        printf("Can not allocate the memory.\n");
        break;
    case INVALID_PARAMETER:
        printf("Invalid parameter!\n");
        break;
    default:
        break;
    }
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

void sdvig(char* str) {
    int ind = 0;
    while (str[ind] != '\0') {
        str[ind] = str[ind+1];
        ind++;
    }
}

int add(char* a, char* b, int base, char** res) {
    int lena = strlen(a), lenb = strlen(b);
    if (lena == 0 || lenb == 0) {
        return INVALID_PARAMETER;
    }
    int len = lena > lenb ? lena : lenb;
    *res = (char*)malloc((len+2)*sizeof(char));
    if (*res == NULL) {
        return MEMORY_ISSUES;
    }
    int carry = 0, sum;
    for (int i = 0; i < len; i++) {
        int x = i < lena ? which_number(a[lena-i-1]) : 0;
        int y = i < lenb ? which_number(b[lenb-i-1]) : 0;
        if (x >= base || y >= base || x == -1 || y == -1) {
            free(*res);
            return INVALID_PARAMETER;
        }
        sum = carry + x + y;
        (*res)[len-i] = all_symbols[sum%base];
        carry = sum/base;
    }
    if (carry > 0) {
        (*res)[0] = all_symbols[carry];
        (*res)[len+1] = '\0';
    } else {
        sdvig(*res);
        (*res)[len] = '\0';
    }
    return ok;
}

void remove_zeroes(char* str) {
    //bool is_start = true;
    int ind = 0;
    while (str[ind] != '\0' && str[ind] == '0') {
        ind++;
    }
    if (str[ind] == '\0') {
        str[1] = '\0';
        return;
    }
    int ind1 = ind;
    while (str[ind1] != '\0') {
        str[ind1 - ind] = str[ind1];
        ind1++;
    }
    str[ind1 - ind] = '\0';
}

int sum(char** ans, int base, int n, ...) {
    if (base < 2 || base > 36) {
        return WRONG_BASE;
    }
    if (n < 0) {
        return INVALID_PARAMETER;
    }

    va_list args;
    va_start(args, n);
    char* res = (char*)malloc(2*sizeof(char));
    if (res == NULL) {
        return MEMORY_ISSUES;
    }
    res[0] = '0';
    res[1] = '\0';
    for (int i = 0; i < n; i++) {
        char* arg = va_arg(args, char*);
        if (strlen(arg) == 0) {
            continue;
        }
        char* tmp = (char*)malloc(sizeof(char) * (strlen(res) + 1));
        if (tmp == NULL) {
            free(res);
            return MEMORY_ISSUES;
        }
        strcpy(tmp, res);
        free(res);
        int st = add(tmp, arg, base, &res);
        if (st != ok) {
            free(tmp);
            free(res);
            return MEMORY_ISSUES;
        }
        free(tmp);
    }
    va_end(args);
    remove_zeroes(res);
    *ans = res;
    return ok;
}

int main() {
    char* ans = NULL;
    int st = sum(&ans, 2, 2, "00", "000000000000001");
    if (st != ok ){
        print_error(st);
        free(ans);
        return 1;
    }
    printf("%s\n", ans);
    free(ans);


    return 0;
}