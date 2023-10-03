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
    int pivot; // разрешающий элемент
    int l_hold = left; //левая граница
    int r_hold = right; // правая граница
    pivot = numbers[left];
    while (left < right) // пока границы не сомкнутся
    {
        while ((numbers[right] >= pivot) && (left < right))
            right--; // сдвигаем правую границу пока элемент [right] больше [pivot]
        if (left != right) // если границы не сомкнулись
        {
            numbers[left] = numbers[right]; // перемещаем элемент [right] на место разрешающего
            left++; // сдвигаем левую границу вправо
        }
        while ((numbers[left] <= pivot) && (left < right))
            left++; // сдвигаем левую границу пока элемент [left] меньше [pivot]
        if (left != right) // если границы не сомкнулись
        {
            numbers[right] = numbers[left]; // перемещаем элемент [left] на место [right]
            right--; // сдвигаем правую границу влево
        }
    }
    numbers[left] = pivot; // ставим разрешающий элемент на место
    pivot = left;
    left = l_hold;
    right = r_hold;
    if (left < pivot) // Рекурсивно вызываем сортировку для левой и правой части массива
        quick_sort(numbers, left, pivot - 1);
    if (right > pivot)
        quick_sort(numbers, pivot + 1, right);
}

int bs_lower_bound(int a[], int n, int x) {
    int l = 0;
    int h = n; // Not n - 1
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
    //int arr_size = rand() % 10000 + 10;
    int arr_size = 10;

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
        A[i] = rand() % 20 - 10;
        B[i] = rand() % 20 - 10;
    }

    quick_sort(B, 0, arr_size-1);
    fill_C(C, A, B, arr_size);

    for (int i = 0; i < arr_size; i++) {
        printf("%d ", A[i]);
    }
    printf("\n");

    for (int i = 0; i < arr_size; i++) {
        printf("%d ", B[i]);
    }
    printf("\n");

    for (int i = 0; i < arr_size; i++) {
        printf("%d ", C[i]);
    }
    printf("\n");
}