#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

enum status_codes{
    ok, 
    OVERFLOWED,
    MEMORY_ISSUES,
    UNABLE_TO_OPEN_FILE, 
    NOT_EQUAL,
    EQUAL, 
    END_OF_FILE
};

void print_error(int st) {
    switch (st)
    {
    case OVERFLOWED:
        printf("OverflowED!\n");
        break;
    case MEMORY_ISSUES:
        printf("Memory issues!\n");
        break;
    case UNABLE_TO_OPEN_FILE:
        printf("Unable to open a file.\n");
        break;
    default:
        break;
    }
}

typedef struct result_list {
    char* filename;
    int line;
    int ind_in_line;
    struct result_list* next;
    struct result_list* prev;
} result_list;

int create_result_list(result_list** head, char* filename, int line, int ind_in_line) {
    *head = (result_list*)malloc(sizeof(result_list));
    if (head == NULL) {
        return MEMORY_ISSUES;
    }
    (*head)->filename = filename;
    (*head)->ind_in_line = ind_in_line;
    (*head)->line = line;
    (*head)->next = NULL;
    (*head)->prev = NULL;
    return ok;
}

int add_node(result_list*** prev, char* filename, int line, int ind_in_line) {
    if (*prev == NULL) {
        return MEMORY_ISSUES;
    }
    result_list* new_el;
    new_el = (result_list*)malloc(sizeof(result_list));
    if (!new_el) {
        return MEMORY_ISSUES;
    }
    new_el->filename = filename;
    new_el->ind_in_line = ind_in_line;
    new_el->line = line;
    new_el->next = NULL;
    new_el->prev = **prev;
    if (**prev != NULL) {
        (**prev)->next = new_el;
    }
    **prev = new_el;
    return ok;
}

void free_list(result_list* tail) {
    if (tail == NULL) {
        return;
    }
    result_list* tmp = tail->prev;
    free(tail);
    free_list(tmp);
}

void print_list(result_list* tail) {
    if (tail == NULL) {
        printf("The list is empty!\n");
        return;
    }
    result_list* tmp = tail;
    while ((tmp->prev) != NULL) {
        tmp = tmp->prev; 
    }
    while (tmp != NULL) {
        printf("The name of file is %s\n", tmp->filename);
        printf("It`s num of line = %d\n", tmp->line);
        printf("It`s index in line = %d\n", tmp->ind_in_line);
        printf("\n");
        tmp = tmp->next;
    }
}

int str_cmp(char* str, FILE* inp) {
    int n = strlen(str);
    char c;
    int st = EQUAL;
    int i = 1;
    while (i < n) {
        c = getc(inp);
        if (c == EOF) {
            st = END_OF_FILE;
            break;
        }
        if (c != str[i]) {
            st = NOT_EQUAL;
            break;
        }
        i++;
    }

    fseek(inp, -i, SEEK_CUR);
    return st;
}

int work_with_file(char* str, char* filename, result_list** ans) {
    FILE* inp = fopen(filename, "r");
    if (!inp) {
        return UNABLE_TO_OPEN_FILE;
    }

    int n = strlen(str);
    int lines_am = 1;
    int inp_ind = 0;

    char c = getc(inp);
    while (c != EOF) {
        if (c == '\n') {
            lines_am++;
            inp_ind = -1;
        }
        inp_ind++;
        if (c != str[0]) {
            c = getc(inp);
            continue;
        }
        int st = str_cmp(str, inp);
        if (st == EQUAL) {
            if (*ans == NULL) {
                st = create_result_list(ans, filename, lines_am, inp_ind);
                if (st != ok) {
                    return st;
                }
            }
            else {
                st = add_node(&ans, filename, lines_am, inp_ind);
                if (st != ok) {
                    free_list(*ans);
                    return st;
                }
            }
            c = getc(inp);
        }
        else if (st == END_OF_FILE) {
            break;
        }
        c = getc(inp);
    }

    fclose(inp);
    return ok;
}

int solve(result_list** ans, char* str, int amount_of_files, ...) {
    *ans = NULL;
    va_list param;
    va_start(param, amount_of_files);
    for (int i = 0; i < amount_of_files; i++) {
        int st = work_with_file(str, va_arg(param, char*), ans);
        if (st != ok) {
            free_list(*ans);
            return st;
        }
    }
    va_end(param);
    return ok;
}

int main() {
    result_list* ans = NULL;
    int st = solve(&ans, "aba\n", 3, "input1.txt", "input2.txt", "input3.txt");
    if (st != ok) {
        print_error(st);
        return 1;
    }
    print_list(ans);
    free_list(ans);

    return 0;
}