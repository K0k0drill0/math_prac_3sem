#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

//const int ARR_SIZE = 10;
const int INF = 1e9;

static char *Error_names[] = {
    "Wrong amount of arguments.",
    "Wrong arguments."
};

enum Errors{
    ok = -1,
    WRONG_AMOUNT_OF_ARGUMENTS,
    WRONG_ARGUMENTS
};

void solve(int* arr) {
    int max_num = arr[0];
    int min_num = arr[0];
    int max_ind = 0;
    int min_ind = 0;
    for (int i = 1; i < ARR_SIZE; i++) {
        if (arr[i] > max_num) {
            max_num = arr[i];
            max_ind = i;
        }
        if (arr[i] < min_num) {
            min_num = arr[i];
            min_ind = i;
        }
    }
    int tmp = arr[min_ind];
    arr[min_ind]  = arr[max_ind];
    arr[max_ind] = tmp;
    // a b
    // a ^= b;
    // b ^= a;
    // a ^= b;
}

bool is_int(char* str) {
    int n = strlen(str);
    for (int i = 0; i < n; i++) {
        if (!isdigit(str[i])) {
            if (i == 0 && str[i] == '-') {
                continue;
            }
            return 0;
        }
    }
    return 1;
    //sscanf
    //TODO
}

int valid_argc(int argc, char** argv) {
    if (argc != 3) {
        
        return WRONG_AMOUNT_OF_ARGUMENTS;
    }
    if (!is_int(argv[1]) || !is_int(argv[2])) {
        return WRONG_ARGUMENTS;
    }
    return ok;
}

int main(int argc, char** argv) {
    const int ARR_SIZE = 10;

    srand(time(NULL));
    switch (valid_argc(argc, argv))
    {
    case WRONG_AMOUNT_OF_ARGUMENTS:
        printf("%s\n", Error_names[WRONG_AMOUNT_OF_ARGUMENTS]);
        return 1;
        break;
    case WRONG_ARGUMENTS:
        printf("%s\n", Error_names[WRONG_ARGUMENTS]);
        return 1;
        break;
    default:
        break;
    }

    int a = atoi(argv[1]);
    int b = atoi(argv[2]);
    int arr[ARR_SIZE];
    for (int i = 0; i < ARR_SIZE; i++) {
        arr[i] = rand() % (b-a+1) + a;
    }

    printf("Array before: ");
    for (int i = 0; i < ARR_SIZE; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    solve(arr, ARR_SIZE);

    printf("Array after: ");
    for (int i = 0; i < ARR_SIZE; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}