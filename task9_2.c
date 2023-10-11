#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

static char *Error_names[] = {
    "Wrong amount of arguments.",
    "Wrong arguments.",
    "Error: memory allocation."
};

enum Errors{
    ok = -1,
    WRONG_AMOUNT_OF_ARGUMENTS,
    WRONG_ARGUMENTS,
    MEMORY_ISSUES
};

void quick_sort(int *numbers, int left, int right)
{
    int pivot; 
    int l_hold = left; 
    int r_hold = right; 
    pivot = numbers[left];
    while (left < right) 
    {
        while ((numbers[right] >= pivot) && (left < right))
            right--; 
        if (left != right) 
        {
            numbers[left] = numbers[right]; 
            left++; 
        }
        while ((numbers[left] <= pivot) && (left < right))
            left++; 
        if (left != right) 
        {
            numbers[right] = numbers[left]; 
            right--;
        }
    }
    numbers[left] = pivot;
    pivot = left;
    left = l_hold;
    right = r_hold;
    if (left < pivot)
        quick_sort(numbers, left, pivot - 1);
    if (right > pivot)
        quick_sort(numbers, pivot + 1, right);
}

int bs_lower_bound(int a[], int n, int x) {
    int l = 0;
    int h = n; 
    while (l < h) {
        int mid =  l + (h - l) / 2;
        if (x <= a[mid]) {
            h = mid;
        } else {
            l = mid + 1;
        }
    }
    return l;
}

int ind_cnt(int b[], int el, int arr_size) {
    int ans = bs_lower_bound(b, arr_size, el);
    if (ans == 0) {
        return ans;
    }
    if (ans == arr_size) {
        return --ans;
    }
    int el1 = b[ans];
    int el2 = b[ans-1];

    return (abs(el - el1) < abs(el - el2)) ? ans : --ans;
}

void fill_C(int c[], int a[], int b[], int arr_size) {
    for (int i = 0; i < arr_size; i++) {
        int ind = ind_cnt(b, a[i], arr_size);
        c[i] = a[i] + b[ind];
    }
}

int main(int argc, char** argv) {
    srand(time(NULL));
    int arr_size = rand() % 10000 + 10;
    //int arr_size = 10;

    int* A;
    int* B;
    int* C;
    A = (int*)malloc(sizeof(int) * arr_size);
    B = (int*)malloc(sizeof(int) * arr_size);
    C = (int*)malloc(sizeof(int) * arr_size);
    if (A == NULL || B == NULL || C == NULL) {
        printf("%s\n", Error_names[MEMORY_ISSUES]);
        free(A);
        free(B);
        free(C);
        return 1;
    }

    for (int i = 0; i < arr_size; i++) {
        A[i] = rand() % 2001 - 1000;
        B[i] = rand() % 2001 - 1000;
    }

    quick_sort(B, 0, arr_size-1);
    fill_C(C, A, B, arr_size);

    printf("Array A: ");
    for (int i = 0; i < arr_size; i++) {
        printf("%d ", A[i]);
    }
    printf("\n");

    printf("Array B: ");
    for (int i = 0; i < arr_size; i++) {
        printf("%d ", B[i]);
    }
    printf("\n");

    printf("Array C: ");
    for (int i = 0; i < arr_size; i++) {
        printf("%d ", C[i]);
    }
    printf("\n");

    free(A);
    free(B);
    free(C);
    return 0;
}