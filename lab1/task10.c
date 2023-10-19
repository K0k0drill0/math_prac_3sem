#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>

enum status_code{
    ok = -1,
    WRONG_BASE,
    MEMORY_ISSUES,
    OVERFLOWED
};

const char all_symbols[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z'
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

int remove_zeroes(char *str) {
    int cnt = 0;
    bool is_start = true;
    for (int i = 0; i < strlen(str); i++) {
        char c = str[i];
        if (c == '-') continue;

        if (is_start && c == '0') cnt++;

        if ((isdigit(c) || isalpha(c)) && c != '0') is_start = false;
    }
    if (is_start) {
        str[0] = '0';
        str[1] = '\0';
        return ok;
    }

    int i = 0;
    if (str[i] == '-') i++;
    while (i + cnt < strlen(str)) {
        str[i] = str[i + cnt];
        i++;
    }
    str[strlen(str) - cnt] = '\0';
    return ok;
}

int to_decimal(char *str_old, int *num_new, int notation) {
    *num_new = 0;
    int base_power = 1;
    int sign = 1;
    if (str_old[0] == '-') sign = -1;
    for (int i = strlen(str_old) - 1; i >= 0; i--) {
        if (str_old[i] == '-') continue;
        int dig = which_number(str_old[i]);

        if (INT_MAX - base_power * dig < *num_new) {
            return OVERFLOWED;
        }
        *num_new += base_power * dig;

        if (INT_MAX / notation < base_power) {
            return OVERFLOWED;
        }
        base_power *= notation;
    }
    *num_new *= sign;
    return ok;
}

int from_decimal(int decimal, int new_sn, char** ans) {  
    int arr_size = 0;
    int sign = 1;

    if (new_sn < 2 || new_sn > 36) {
        return WRONG_BASE;
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

    *ans = (char*)malloc(sizeof(char) * arr_size);
    if (*ans == NULL) {
        return MEMORY_ISSUES;
    }

    if (decimal == 0) {
        (*ans)[0] = '0';
        (*ans)[1] = '\0';
        return ok;
    }
    //char *ptr = ans;

    int i = 0;
    while (decimal > 0) {
        char remainder = all_symbols[decimal % new_sn];
        (*ans)[i] = remainder;
        i++;
        decimal /= new_sn;
    }
    if (sign == -1) {
        (*ans)[i] = '-';
        i++;
    } 
    (*ans)[i] = '\0';
    for (int j = 0; j < (i)/2; j++) {
        char tmp = (*ans)[j];
        (*ans)[j] = (*ans)[i-j-1];
        (*ans)[i-j-1] = tmp;
    }
    printf("%s\n", *ans);
    return ok;
}

int is_num_valid(char* str, int base) {
    int n = strlen(str);
    if (n == 1 && str[0] == '-') {
        return 0;
    }
    for (int i = 0; i < n; i++) {
        if (i == 0 && str[i] == '-') {
            continue;
        }

        int tmp = tolower(str[i]);
        if (isalpha(str[i]) && tmp == str[i]) {
            return 0;
        }

        if (!isdigit(str[i]) && !isalpha(str[i])) {
            return 0;
        }

        tmp = which_number(str[i]);
        if (tmp >= base) {
            return 0;
        }
    }
    return 1;
}

int read_line(char** str_inp) {
    int arr_max_size = 16;
    *str_inp = (char*)malloc(sizeof(char) * arr_max_size);
    if (*str_inp == NULL) {
        return MEMORY_ISSUES;
    }

    char c;
    c = getchar();
    int ind = 0;
    while (is_separator(c)) {
        c = getchar();
    }

    while (!is_separator(c)) {
        if (ind >= arr_max_size - 1) {
            arr_max_size *= 2;
            char *tmp = (char*)realloc(*str_inp, arr_max_size * sizeof(char));
            if (tmp == NULL) {
                free(str_inp);
                return MEMORY_ISSUES;
            }
            *str_inp = tmp;
        }
        (*str_inp)[ind] = c;
        ind++;
        c = getchar();
    }
    (*str_inp)[ind] = '\0';
    return ok;
}

int read_and_count(int base, char** max_abs_str, char** ninth, char** eighteenth, char** tw_sev, char** thir_six) {
    int max_abs = 0;

    char* str_inp = NULL;
    int st = read_line(&str_inp);
    while (!(strlen(str_inp) == 4 && str_inp[0] == 'S'
         && str_inp[1] == 't' && str_inp[2] == 'o' && str_inp[3] == 'p')) {

        if (st != ok) {
            free(str_inp);
            return st;
        }

        if (!is_num_valid(str_inp, base)) {
            free(str_inp);
            st = read_line(&str_inp);
            continue;
        }

        remove_zeroes(str_inp);

        int num = 0;
        st = to_decimal(str_inp, &num, base);
        if (st != ok) {
            free(str_inp);
            return st;
        }
        if (abs(num) >= max_abs) {
            max_abs = abs(num);
            char* tmp = (char*)realloc(*max_abs_str, sizeof(char) * (strlen(str_inp) + 1));
            if (tmp == NULL) {
                free(str_inp);
                return MEMORY_ISSUES;
            }
            *max_abs_str = tmp;
            strcpy(*max_abs_str, str_inp);
        }

        free(str_inp);
        st = read_line(&str_inp);
    }
    
    int n_st = from_decimal(max_abs, 9, ninth);
    int e_st = from_decimal(max_abs, 18, eighteenth);
    int t_st = from_decimal(max_abs, 27, tw_sev);
    int thir_st = from_decimal(max_abs, 36, thir_six);

    if (n_st != ok || e_st != ok || t_st != ok || thir_st != ok) {
        return MEMORY_ISSUES;
    }

    free(str_inp);
    return ok;
}

void print_error(int st) {
    switch (st)
    {
    case WRONG_BASE:
        printf("Wrong base!\n");
        break;
    case MEMORY_ISSUES:
        printf("Can not allocate the memory.\n");
        break;
    case OVERFLOWED:
        printf("Overflow!\n");
        break;
    default:
        break;
    }
}
 
int main(void)
{
    printf("Input base.\n");
    int base;
    scanf("%d", &base);
    if (base < 2 || base > 36) {
        print_error(WRONG_BASE);
        return 1;
    }

    char* max_abs_str = NULL;
    char* ninth = NULL;
    char* eighteenth = NULL;
    char* tw_sev = NULL;
    char* thir_six = NULL;

    int st = read_and_count(base, &max_abs_str, &ninth, &eighteenth, &tw_sev, &thir_six);
    if (st != ok) {
        free(max_abs_str);
        free(ninth);
        free(eighteenth);
        free(tw_sev);
        free(thir_six);
        print_error(st);
        return 1;
    }

    if (max_abs_str == NULL) {
        max_abs_str = (char*)malloc(sizeof(char) * 2);
        if (max_abs_str == NULL) {
            print_error(MEMORY_ISSUES);
            return 1;
        }
        max_abs_str[0] = '0';
        max_abs_str[1] = '\0';
    }

    printf("%s, %s, %s, %s, %s\n", max_abs_str, ninth, eighteenth, tw_sev, thir_six);
    free(max_abs_str);
    free(ninth);
    free(eighteenth);
    free(tw_sev);
    free(thir_six);
    return 0;
}