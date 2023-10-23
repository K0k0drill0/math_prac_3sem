#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <ctype.h>

enum status_code{
    ok,
    WRONG_AMOUNT_OF_ARGS,
    INVALID_ARGS,
    OVERFLOW,
    MEMORY_ISSUES
};

void print_error(int st) {
    switch (st)
    {
    case INVALID_ARGS:
        printf("Invalid args. Try again.\n");
        break;
    case WRONG_AMOUNT_OF_ARGS:
        printf("Wrong amount of arguments. Try again.\n");
        break;
    case OVERFLOW:
        printf("Overflow.\n");
        break;
    case MEMORY_ISSUES:
        printf("Can not allocate the memory.\n");
        break;
    default:
        break;
    }
}

int m_strlen(char* str) {
    int ans = 0;
    while (str[ans] != '\0') {
        ans++;
        if (errno == ERANGE) {
            return INT_MAX;
        }
    }

    return ans;
}

int m_reverse(char* str, char** ans) {
    int n = m_strlen(str);
    if (errno == OVERFLOW) {
        return OVERFLOW;
    }

    *ans = (char*)malloc(sizeof(char) * (n + 1));
    if (*ans == NULL) {
        return MEMORY_ISSUES;
    }
    (*ans)[n] = '\0';

    n--;
    int ind = 0;
    while (n >= 0) {
        (*ans)[ind] = str[n];
        ind++;
        n--;
    }

    return ok;
}

int odd_upper_case(char* str, char** ans) {
    *ans = (char*)malloc(sizeof(str));
    if (*ans == NULL) {
        return MEMORY_ISSUES;
    }

    int ind = 0;
    while (str[ind] != '\0') {
        if ((ind & 1)) {
            (*ans)[ind] = toupper(str[ind]);
        }
        else {
            (*ans)[ind] = str[ind];
        }
        ind++;
    }
    (*ans)[ind] = '\0';

    if (errno == ERANGE) {
        return OVERFLOW;
    }

    return ok;
}

int sort_str(char* str, char** ans) {
    *ans = (char*)malloc(sizeof(str));
    if (*ans == NULL) {
        return MEMORY_ISSUES;
    }

    int ind = 0;
    int ans_ind = 0;
    while (str[ind] != '\0') {
        if (isdigit(str[ind])) {
            (*ans)[ans_ind] = str[ind];
            ans_ind++;
        }
        ind++;
    }

    ind = 0;
    while (str[ind] != '\0') {
        if (isalpha(str[ind])) {
            (*ans)[ans_ind] = str[ind];
            ans_ind++;
        }
        ind++;
    }

    ind = 0;
    while (str[ind] != '\0') {
        if (!isalpha(str[ind]) && !isdigit(str[ind])) {
            (*ans)[ans_ind] = str[ind];
            ans_ind++;
        }
        ind++;
    }

    (*ans)[ind] = '\0';

    if (errno == ERANGE) {
        return OVERFLOW;
    }

    return ok;
}

int get_strings(int argc, char** argv, char*** ans) {
    *ans = (char**)malloc(sizeof(char*) * (argc - 3));
    if (*ans == NULL) {
        return MEMORY_ISSUES;
    }

    (*ans)[0] = argv[2];
    for (int i = 4; i < argc; i++) {
        (*ans)[i-3] = argv[i];
    }

    return ok;
}

int concatenation(int seed, int amount, char** strings, char** ans) {
    int size = 0;

    for (int i = 0; i < amount; i++) {
        size += m_strlen(strings[i]);
        if (errno == ERANGE) {
            return OVERFLOW;
        }
    }

    *ans = (char*)malloc(sizeof(char) * (size + 1));
    if (*ans == NULL) {
        return MEMORY_ISSUES;
    }
    srand(seed);
    
    int* arr = (int*)calloc(amount, sizeof(int));

    int mega_ind = 0;

    for (int i = 0; i < amount; i++) {
        int ind = rand() % amount;
        while (arr[ind]) {
            ind++;
            ind %= amount;
        }
        arr[ind] = 1;
        for (int j = 0; strings[ind][j]; j++) {
            (*ans)[mega_ind] = strings[ind][j];
            mega_ind++;
        }
    }
    (*ans)[mega_ind] = '\0';
    free(arr);

    return ok;
}

int valid_args(int argc, char** argv) {
    if (argc < 3) {
        return WRONG_AMOUNT_OF_ARGS;
    }
    char flags[] = {'l', 'r', 'u', 'n', 'c'};

    int n = m_strlen(argv[1]);
    if (errno == ERANGE) {
        return OVERFLOW;
    }
    if (n != 2) {
        return INVALID_ARGS;
    }

    if (argv[1][0] != '-') {
        return INVALID_ARGS;
    }

    bool flag = false;
    for (int i = 0; i < 5; i++) {
        if (argv[1][1] == flags[i]) {
            flag = true;
        }
    }
    if (!flag) {
        return INVALID_ARGS;
    }

    if (argv[1][1] == 'c') {
        if (argc < 4) {
            return WRONG_AMOUNT_OF_ARGS; 
        }

        int ind = 0;
        while (argv[3][ind] != '\0') {
            if (!isdigit(argv[3][ind])) {
                return INVALID_ARGS;
            }
            ind++;
        }
        int n = atoi(argv[3]);
        if (errno == ERANGE) {
            return OVERFLOW;
        }
    }
    return ok;
}

int main(int argc, char** argv) {
    
    int st = valid_args(argc, argv);
    if (st != ok) {
        print_error(st);
        return 1;
    }

    switch (argv[1][1])
    {
    case 'l':
        int ans = m_strlen(argv[2]);
        if (st != ok) {
            print_error(st);
            return 1;
        }
        printf("The length of the string is %d.\n", ans);
        break;
    case 'r':
        char* rstr;
        st = m_reverse(argv[2], &rstr);
        if (st != ok) {
            free(rstr);
            print_error(st);
            return 1;
        }
        printf("Reversed string: %s\n", rstr);
        free(rstr);
        break;
    case 'u':
        char* odd_upper_str;
        st = odd_upper_case(argv[2], &odd_upper_str);
        if (st != ok) {
            free(odd_upper_str);
            print_error(st);
            return 1;
        }
        printf("String, after upping case of every odd index: %s.\n", odd_upper_str);
        free(odd_upper_str);
        break;
    case 'n':
        char* str;
        st = sort_str(argv[2], &str);
        if (st != ok) {
            print_error(st);
            return 1;
        }
        printf("Sorted string: %s.\n", str);
        free(str);
        break;
    case 'c':
    {
        char** ans;
        int st = get_strings(argc, argv, &ans);
        if (st != ok) {
            print_error(st);
            return 1;
        }

        char* ans1;
        int seed = atoi(argv[3]);
        st = concatenation(seed, argc - 3, ans, &ans1);
        if (st != ok) {
            print_error(st);
            return 1;
        }
        printf("%s\n", ans1);
        free(ans);
        free(ans1);
        break;
    }
    default:
        break;
    }
    return 0;
}