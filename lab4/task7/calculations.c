#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include "universal_header.h"

int is_valid_varname(const char* str) {
    int ind = 0;
    while (str[ind] != '\0') {
        if (!isalpha(str[ind])) {
            return 0;
        }
        ind++;
    }
    return 1;
}

int is_digit(const char* str) {
    int ind = 0;
    while (str[ind] != '\0') {
        if (!isdigit(str[ind])) {
            if (str[ind] != '-' || ind != 0) {
                return 0;
            }
        }
        ind++;
    }
    return 1;
}

int op_print(const Memory* mem, const char* str) {
    if (mem == NULL || str == NULL) {
        return INVALID_ARGUMENT_OF_FUNCTION;
    }

    int n = strlen(str);
    if (n < 6) {
        return INVALID_ARGUMENT_OF_FUNCTION;
    }

    int ind = 6;
    if (str[ind] == '\0') {
        print_memory(mem);
        return ok;
    }
    
    char* var = (char*)malloc(sizeof(char) * (n+1));
    if (var == NULL) {
        return MEMORY_ISSUES;
    }

    int var_ind = 0;
    while (str[ind] != ';') {
        var[var_ind] = str[ind];
        ind++;
        var_ind++;
    }
    var[var_ind] = '\0';

    MemoryCell* cell = NULL;

    int st = ok;
    st = find_memory_cell_by_name(mem, var, &cell);
    if (st != ok) {
        free(var);
        return st;
    }

    if (cell == NULL) {
        free(var);
        return NONE;
    }

    printf("%d\n", cell->val);
    free(var);
    return ok;
}

int op_assign(Memory** mem, const char* str) {
    if (*mem == NULL || str == NULL) {
        return INVALID_ARGUMENT_OF_FUNCTION;
    }
    int n = strlen(str);

    char* var_name = (char*)malloc(sizeof(char) * (n+1));
    if (var_name == NULL) {
        return MEMORY_ISSUES;
    }

    int str_ind = 0, var_name_ind = 0;
    while (str[str_ind] != '=') {
        var_name[var_name_ind] = str[str_ind];
        str_ind++;
        var_name_ind++;
    }
    var_name[var_name_ind] = '\0';

    char* val_str = (char*)malloc(sizeof(char) * (n+1));
    if (val_str == NULL) {
        free(var_name);
        return MEMORY_ISSUES;
    }

    str_ind++;
    int val_ind = 0;
    while(str[str_ind] != ';') {
        val_str[val_ind] = str[str_ind];
        val_ind++;
        str_ind++;
    }
    val_str[val_ind] = '\0';

    int value = 0;
    int st = calculate_expression(*mem, val_str, &value);
    free(val_str);
    if (st != ok) {
        free(var_name);
        return st;
    }

    MemoryCell* container = NULL;
    st = find_memory_cell_by_name(*mem, var_name, &container);
    if (st != ok) {
        free(var_name);
        return st;
    }

    if (container == NULL) {
        MemoryCell* new = NULL;
        st = init_memory_cell(&new, var_name, value);
        if (st != ok) {
            return st;
        }
        st = add_memory_cell(mem, new);
        if (st != ok) {
            free_memory_cell(new);
            return st;
        }
    }
    else {
        free(var_name);
        container->val = value;
    }

    return ok;
}

int find_value(Memory* mem, const char* str, int* ans) {
    *ans = 0;
    if (is_valid_varname(str)) {
        MemoryCell* cell = NULL;
        int st = find_memory_cell_by_name(mem, str, &cell);
        if (st != ok) {
            return st;
        }
        if (cell == NULL) {
            return INVALID_VALUE;
        }
        *ans += cell->val;
    }
    else if (is_digit(str)) {
        *ans += atoi(str);
        if (errno == ERANGE) {
            return OVERFLOWED;
        }
    }
    else {
        return INVALID_VALUE;
    }
    return ok;
}

int calculate_expression(Memory* mem, char* str, int* ans) {
    if (str == NULL || ans == NULL) {
        return INVALID_ARGUMENT_OF_FUNCTION;
    }
    *ans = 0;

    int n = strlen(str);
    char* fir = (char*)malloc(sizeof(char) * (n+1));
    if (fir == NULL) {
        return MEMORY_ISSUES;
    }

    char* sec = (char*)malloc(sizeof(char) * (n+1));
    if (sec == NULL) {
        free(fir);
        return MEMORY_ISSUES;
    }

    int str_ind = 0, fir_ind = 0, sec_ind = 0;

    while ((!is_arithmetic_operation(str[str_ind]) || (str[str_ind] == '-' && str_ind == 0)) && str[str_ind] != '\0') {
        fir[fir_ind] = str[str_ind];
        fir_ind++;
        str_ind++;
    }
    fir[fir_ind] = '\0';

    int num = 0;
    int st = find_value(mem, fir, &num);
    free(fir);
    if (st != ok) {
        free(sec);
        return st;
    }
    *ans += num;
    if (errno == ERANGE) {
        free(sec);
        return OVERFLOWED;
    }

    if (str[str_ind] == '\0') {
        free(sec);
        return ok;
    }

    char op = str[str_ind];
    str_ind++;

    while (str[str_ind] != '\0') {
        sec[sec_ind] = str[str_ind];
        sec_ind++;
        str_ind++;
    }
    sec[sec_ind] = '\0';

    num = 0;
    st = find_value(mem, sec, &num);
    free(sec);
    if (st != ok) {
        return st;
    }
    if (errno == ERANGE) {
        return OVERFLOWED;
    }

    switch (op)
    {
    case '+':
        *ans += num;
        break;
    case '-':
        *ans -= num;
        break;
    case '*':
        *ans *= num;
        break;
    case '/':
        if (num == 0) {
            return INVALID_VALUE;
        }
        *ans /= num;
        break;
    case '%':
        if (num == 0) {
            return INVALID_VALUE;
        }
        *ans %= num;
        break;
    default:
        break;
    }

    return ok;
}