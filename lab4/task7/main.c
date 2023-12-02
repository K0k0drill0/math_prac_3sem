#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include "universal_header.h"

int is_separator(char c) {
    return (c == ' ' || c == '\t' || c == '\n'); 
}

int read_line(FILE* inp, char** line) {
    int arr_max_size = 16;
    *line = (char*)malloc(sizeof(char) * arr_max_size);
    if (*line == NULL) {
        return MEMORY_ISSUES;
    }

    char c = getc(inp);
    while (is_separator(c)) {
        c = getc(inp);
    }

    int ind = 0;
    while (c != ';' && c != EOF) {
        if (ind >= arr_max_size - 3) {
            arr_max_size *= 2;
            char *tmp = (char*)realloc(*line, arr_max_size * sizeof(char));
            if (tmp == NULL) {
                return MEMORY_ISSUES;
            }
            *line = tmp;
        }
        (*line)[ind] = c;
        ind++;
        c = getc(inp);
    }

    (*line)[ind] = ';';
    ind++;
    (*line)[ind] = '\0';

    if (c == EOF && ind == 1) {
        return EOF;
    }

    return ok;
}

int command_type(char* str) {
    if (str == NULL) {
        return INVALID_ARGUMENT_OF_FUNCTION;
    }

    int line_size = strlen(str);
    char* first_word = (char*)malloc(sizeof(char) * line_size);
    if (first_word == NULL) {
        return MEMORY_ISSUES;
    }

    int ind = 0;
    while (str[ind] != '=' && str[ind] != ' ' && str[ind] != ';') {
        first_word[ind] = str[ind];
        ind++;
    }
    first_word[ind] = '\0';

    if (!strcmp(first_word, "print")) {
        free(first_word);
        return PRINT;
    }
    else {
        free(first_word);
        return ASSIGN;
    }
}

int process_file(FILE* inp) {
    Memory* mem = NULL;
    int st = init_memory(&mem);
    if (st != ok) {
        return st;
    }

    char* str = NULL;
    st = ok;
    while ((st = read_line(inp, &str)) == ok) {
        int q = command_type(str);
        switch (q)
        {
        case ASSIGN:
            st = op_assign(&mem, str);
            if (st != ok) {
                free(str);
                free_memory(mem);
                return st;
            }
            break;
        case PRINT:
            st = op_print(mem, str);
            if (st != ok) {
                free(str);
                free_memory(mem);
                return st;
            }
            break;
        default:
            break;
        }
        free(str);
    }
    free(str);

    free_memory(mem);
    if (st == EOF) {
        return ok;
    }
    return st;
}


/*
Алгоритм:
1. Читаем строку.
2. Проверяем слово до пробела или знака равно, распределяем по веткам. С веткой принт все пон
Ветка инициализации:
Обрабатываем строку. Смотрим срез от знака равно до ;
Определяем арифметическую операцию, если она есть.
Находим значения переменных, если они есть. Считаем значение будущей переменной
Ищем в массиве имя переменной, которой хотим что-то присвоить. Если оно там есть,
то перезаписываем. Если его там нет, то инициализируем 

УЧЕСТЬ!!!

1. Отрицательные числа
*/


int main(int argc, char** argv) {
    argc = 2;
    argv[1] = "input.txt";

    if (argc != 2) {
        print_error(WRONG_AMOUNT_OF_ARGUMENTS);
        return 1;
    }

    FILE* inp = fopen(argv[1], "r");
    if (!inp) {
        print_error(UNABLE_TO_OPEN_A_FILE);
        return -1;
    }

    int st = process_file(inp);
    if (st != ok) {
        print_error(st);
        fclose(inp);
        return 1;
    }

    fclose(inp);
    return 0;
}

