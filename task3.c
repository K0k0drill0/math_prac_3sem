#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>

char *Error_text[] = {
    "Error: wrong flag.",
    "Error: wrong amount of arguments.",
    "Error: wrong argument(s).",
    "Error: wrong epsilon."
};

enum solve_statuses {
    ONE_ROOT,
    TWO_ROOTS,
    NO_ROOTS,
    A_IS_ZERO
};

enum error_statuses{
    WRONG_FLAG,
    WRONG_AMOUNG_OF_ARGUMENTS,
    WRONG_ARGUMENTS,
    WRONG_EPSILON
};

int solve_equation(double eps, double a, double b, double c, double* ans1, double* ans2) {
    if (fabs(a) < eps) {
        return A_IS_ZERO;
    }
    double discr = (b * b - 4 * a * c);
    if (discr < 0.0) {
        return NO_ROOTS;
    }
    else if (discr < eps) {
        *ans1 = - b / (2 * a);
        return ONE_ROOT;
    }
    else {
        *ans1 = (- b - sqrt(discr)) / (2 * a);
        *ans2 = (- b + sqrt(discr)) / (2 * a);
        return TWO_ROOTS;
    }
}

void pr_solve(double eps, double a, double b, double c) {
    double ans1, ans2;
    switch (solve_equation(eps, a, b, c, &ans1, &ans2)) {
        case ONE_ROOT:
            printf("For a = %f, b = %f, c = %f:\n", a, b, c);
            printf("Root = %f\n", ans1);
            break;
        case TWO_ROOTS:
            printf("For a = %f, b = %f, c = %f:\n", a, b, c);
            printf("Root1 = %f, root2 = %f\n", ans1, ans2);
            break;
        case NO_ROOTS:
            printf("For a = %f, b = %f, c = %f:\n", a, b, c);
            printf("There are no real roots\n");
            break;
        case A_IS_ZERO:
            printf("For a = %f, b = %f, c = %f:\n", a, b, c);
            printf("This is not a quadratic equation.\n");
            break;
        default:
            break;
    }
}

bool are_arrays_equal(double arr1[], double arr2[], int size, double eps) {
    for (int i = 0; i < size; i++) {
        if (fabs(arr1[i] - arr2[i]) >= eps) {
            return false;
        }
    }
    return true;
}

void remove_duplicates(double arrays[][3], int num_arrays, double unique_arrays[][3], int* num_unique_arrays, double eps) {
    for (int i = 0; i < num_arrays; i++) {
        bool is_duplicate = false;
        for (int j = 0; j < *num_unique_arrays; j++) {
            if (are_arrays_equal(arrays[i], unique_arrays[j], 3, eps)) {
                is_duplicate = true;
                break;
            }
        }
        if (!is_duplicate) {
            for (int k = 0; k < 3; k++) {
                unique_arrays[*num_unique_arrays][k] = arrays[i][k];
            }
            (*num_unique_arrays)++;
        }
    }
}

void swap(double* a, double* b) {
    double temp = *a;
    *a = *b;
    *b = temp;
}

void permute(double arr[], int start, int end, double result[][end+1], int* count) {
    if (start == end) {
        for (int i = 0; i <= end; i++) {
            result[*count][i] = arr[i];
        }
        (*count)++;
    } else {
        for (int i = start; i <= end; i++) {
            swap(&arr[start], &arr[i]);
            permute(arr, start + 1, end, result, count);
            swap(&arr[start], &arr[i]);
        }
    }
}

void func_q(double eps, double a, double b, double c) {
    double arr[3] = {a, b, c};
    int arr_size = 3;
    double all_permutations[6][3];
    double unique_permutations[6][3];
    int all_permutations_size = 0;
    int unique_permutations_size = 0;

    permute(arr, 0, arr_size-1, all_permutations, &all_permutations_size);
    remove_duplicates(all_permutations, all_permutations_size, 
    unique_permutations, &unique_permutations_size, eps);

    for (int i = 0; i < unique_permutations_size; i++) {
        pr_solve(eps, unique_permutations[i][0], unique_permutations[i][1],
         unique_permutations[i][2]);
    }

    // if (fabs(a-b) < eps && fabs(b-c) < eps && fabs(a-c) < eps) {
    //     pr_solve(eps, a, b, c);
    // }
    // else if (fabs(a-b) < eps) {
    //     pr_solve(eps, a, b, c);
    //     pr_solve(eps, a, c, b);
    //     pr_solve(eps, c, a, b); 
    // }
    // else if (fabs(a-c) < eps) {
    //     pr_solve(eps, a, b, c);
    //     pr_solve(eps, b, a, c);
    //     pr_solve(eps, a, c, b);
    // }
    // else if (fabs(b-c) < eps) {
    //     pr_solve(eps, a, b, c);
    //     pr_solve(eps, b, a, c);
    //     pr_solve(eps, c, b, a);
    // }
    // else {
    //     pr_solve(eps, a, b, c);
    //     pr_solve(eps, a, c, b);
    //     pr_solve(eps, b, a, c);
    //     pr_solve(eps, b, c, a);
    //     pr_solve(eps, c, a, b);
    //     pr_solve(eps, c, b, a);
    // }

}

bool func_m(double x, double y) {
    double eps = 0.000000001;
    return (fmod(x, y) < eps);
}

bool func_t(double eps, double a, double b, double c) {
    if (fabs(a) < eps || fabs(b) < eps || fabs(c) < eps) {
        return 0;
    }
    double sqra = a*a;
    double sqrb = b*b;
    double sqrc = c*c;
    return (fabs(sqra - sqrb - sqrc) < eps || fabs(sqrb - sqra - sqrc) < eps || (sqrc - sqrb - sqra) < eps);
}

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
    func_q(0.01, 1, 1, 1);
    return 0;
    if (strlen(argv[1]) != 2 || (argv[1][0] != '-' && argv[1][0] != '/') ||
    (argv[1][1] != 'q' && argv[1][1] != 'm' && argv[1][1] != 't')) {
        printf("%s\n", Error_text[WRONG_FLAG]);
        return 1;
    }
    if ((argv[1][1] == 'q' && argc != 6) || (argv[1][1] == 'm' && argc != 4) ||
     (argv[1][1] == 't' && argc != 6)) {
        printf("%s\n", Error_text[WRONG_AMOUNG_OF_ARGUMENTS]);
        return 1;
    }
    
    switch (argv[1][1])
    {
    case 'q':
        if (!is_float(argv[2]) || !is_float(argv[3]) ||
         !is_float(argv[4]) || !is_float(argv[5])) {
            printf("%s\n", Error_text[WRONG_ARGUMENTS]);
            return 1;
         }
        if (atof(argv[2]) <= 0.0) {
            printf("%s\n", Error_text[WRONG_EPSILON]);
            return 1;
        }
        func_q(atof(argv[2]), atof(argv[3]), atof(argv[4]), atof(argv[5]));
        break;
    case 'm':
        if (!is_float(argv[2]) || !is_float(argv[3])) {
            printf("%s\n", Error_text[WRONG_ARGUMENTS]);
            return 1;
        }
        if (func_m(atof(argv[2]), atof(argv[3]))) {
            printf("Yes, the first number is a multiple of the second.\n");
        }
        else {
            printf("No, the first number is not a multiple of the second.\n");
        }
        break;
    case 't':
        if (!is_float(argv[2]) || !is_float(argv[3]) ||
         !is_float(argv[4]) || !is_float(argv[5])) {
            printf("%s\n", Error_text[WRONG_ARGUMENTS]);
            return 1;
        }
        if (atof(argv[2]) <= 0.0) {
            printf("%s\n", Error_text[WRONG_EPSILON]);
            return 1;
        }
        if (func_t(atof(argv[2]), atof(argv[3]), atof(argv[4]), atof(argv[5]))) {
            printf("Yes, these numbers can be the lengths of a right triangle\n");
        }
        else {
            printf("No, these numbers can not be the lengths of a right triangle\n");
        }
        break;
    default:
        break;
    }

}
