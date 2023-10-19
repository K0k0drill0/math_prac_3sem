#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

const char all_symbols[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z'
};

enum status_code{
    ok, 
    WRONG_BASE,
    MEMORY_ISSUES
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

int add(char* a, char* b, int base, char** res) {
    int lena = strlen(a), lenb = strlen(b);
    int len = lena > lenb ? lena : lenb;
    *res = (char*)malloc((len+2)*sizeof(char));
    if (*res == NULL) {
        return MEMORY_ISSUES;
    }
    int carry = 0, sum;
    for (int i = 0; i < len; i++) {
        int x = i < lena ? which_number(a[lena-i-1]) : 0;
        int y = i < lenb ? which_number(b[lenb-i-1]) : 0;
        sum = carry + x + y;
        (*res)[len-i] = all_symbols[sum%base];
        carry = sum/base;
    }
    if (carry > 0) {
        (*res)[0] = all_symbols[carry];
        (*res)[len+1] = '\0';
    } else {
        (*res)++;
        (*res)[len] = '\0';
    }
    return ok;
}

int sum(char** ans, int base, int n, ...) {
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
        char* tmp = (char*)malloc(sizeof(char) * (strlen(res) + 1));
        strcpy(tmp, res);
        int st = add(tmp, arg, base, &res);
        if (st != ok) {
            free(tmp);
            free(res);
            return MEMORY_ISSUES;
        }
        free(tmp);
    }
    va_end(args);
    *ans = res;
    return ok;
}

int main() {
    char* ans;
    int st = sum(&ans, 10, 2, "123", "321");
    if (st != ok ){
        print_error(st);
        free(ans);
        return 1;
    }
    printf("%s\n", ans);
    free(ans);
    return 0;
}