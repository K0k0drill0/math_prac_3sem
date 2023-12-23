#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>
#include "validators.h"
#include "memory.h"

void print_error(FILE* outp, const int c) {
    switch (c)
    {
    case MEMORY_ISSUES:
        fprintf(outp, "Unable to allocate the memory!\n");
        break;
    case UNABLE_TO_OPEN_FILE:
        fprintf(outp, "Unable to open file!\n");
        break;
    case INVALID_FUNCTION_PARAMETER:
        fprintf(outp, "Invalid function parameter!\n");
        break;
    case INVALID_AMOUNT_OF_ARGUMENTS:
        fprintf(outp, "Invalid amount of arguments!\n");
        break;
    case INVALID_FLAG:
        fprintf(outp, "Invalid flag!\n");
        break;
    case EQUAL_FILES:
        fprintf(outp, "Files are equal!\n");
        break;
    case INVALID_COMMENT:
        fprintf(outp, "Invalid comment!\n");
        break;
    case INVALID_COMMAND:
        fprintf(outp, "Invalid command!\n");
        break;
    case INVALID_OPERATION:
        fprintf(outp, "Invalid operation!\n");
        break;
    case OVERFLOWED:
        fprintf(outp, "Value is too big!\n");
        break;
    case INVALID_BASE:
        fprintf(outp, "Invalid base was get.\n");
        break;
    case INVALID_NUMBER:
        fprintf(outp, "Invalid number was entered!\n");
        break;
    case UNINITIALISED_VARIABLE:
        fprintf(outp, "Uninitialised variable was used!\n");
        break;
    default:
        break;
    }
}


int cut(char* str) {
    if (str == NULL) {
        return INVALID_FUNCTION_PARAMETER;
    }
    int ind = strlen(str);
    ind--;
    while (is_separator(str[ind])) {
        ind--;
    }
    ind++;
    str[ind] = '\0';
}

char skip_one_line_comment(FILE* inp) {
    char c = getc(inp);
    while ((c = getc(inp)) != '\n' && c != EOF);
    while (is_separator(c = getc(inp)));
    return c;
}

int skip_multiple_line_comment(FILE* inp, char* c) {
    int brackets_cnt = 1;

    *c = getc(inp);
    while (brackets_cnt != 0 && *c != EOF) {
        if (*c == '}') {
            brackets_cnt--;
        }
        else if (*c == '{') {
            brackets_cnt++;
        }
        *c = getc(inp);
    } 
    if (*c == EOF && brackets_cnt != 0) {
        return INVALID_COMMENT;
    }

    while(is_separator(*c)) {
        *c = getc(inp);
    }
    return ok;
}

int read_сommand_from_file(FILE* inp, char** str_inp) {
    int arr_max_size = 16;
    *str_inp = (char*)malloc(sizeof(char) * arr_max_size);
    if (*str_inp == NULL) {
        return MEMORY_ISSUES;
    }

    char c;
    c = getc(inp);
    int ind = 0;
    while (is_separator(c) && c != EOF) {
        c = getc(inp);
    }

    if (c == EOF) {
        return EOF;
    }

    while (c != ';' && c != EOF) {
        if (ind >= arr_max_size - 1) {
            arr_max_size *= 2;
            char *tmp = (char*)realloc(*str_inp, arr_max_size * sizeof(char));
            if (tmp == NULL) {
                return MEMORY_ISSUES;
            }
            *str_inp = tmp;
        }

        if (c == '%') {
            c = skip_one_line_comment(inp);
            continue;
        }
        else if (c == '{') {
            int st = skip_multiple_line_comment(inp, &c);
            if (st != ok) {
                return st;
            }
            continue;
        }
        (*str_inp)[ind] = c;
        ind++;
        c = getc(inp);
    }
    
    if (ind != 0 && c != ';') {
        return INVALID_COMMAND;
    }

    if (ind == 0 && c == EOF) {
        return EOF;
    }

    (*str_inp)[ind] = '\0';
    return ok;
}

int read_word_from_file(FILE* inp, char** str_inp) {
    int arr_max_size = 16;
    *str_inp = (char*)malloc(sizeof(char) * arr_max_size);
    if (*str_inp == NULL) {
        return MEMORY_ISSUES;
    }

    char c;
    c = getc(inp);
    int ind = 0;
    while (is_separator(c) && c != EOF) {
        c = getc(inp);
    }

    if (c == EOF) {
        return EOF;
    }

    while (!is_separator(c) && c != EOF) {
        if (ind >= arr_max_size - 1) {
            arr_max_size *= 2;
            char *tmp = (char*)realloc(*str_inp, arr_max_size * sizeof(char));
            if (tmp == NULL) {
                return MEMORY_ISSUES;
            }
            *str_inp = tmp;
        }
        (*str_inp)[ind] = c;
        ind++;
        c = getc(inp);
    }
    
    (*str_inp)[ind] = '\0';
    return ok;
}

int calculate(int op, int a, int b) {
    switch (op)
    {
        case CONJUNCTION: return a & b;
        case DISJUNCTION: return a | b;
        case IMPLICATION: return a <= b;
        case REVERSED_IMPLICATION: return b <= a;
        case EQUIVALENCE: return a == b;
        case MOD2: return a != b;
        case COIMPLICATION: return ~(~a | b);
        case SHEFFER: return ~(a & b);
        case PIERCE: return ~(a | b);
        case NEGATION: return ~a;
        default: return INVALID_FUNCTION_PARAMETER;
    }
}

int process_binary_command(FILE* outp, Memory mem, char* command) {
    fprintf(outp, "Processing binary command...\n");
    char A = command[0];
    int i = 0;
    next_symbol(command, &i);
    i++;
    next_symbol(command, &i);

    char B = command[i];
    next_symbol(command, &i);
    int op = which_operation(command[i], command[i+1]);
    next_symbol(command, &i);
    if (!is_separator(command[i]) && !isalpha(command[i])) {
        next_symbol(command, &i);
    }

    char C = command[i];

    A = toupper(A);
    B = toupper(B);
    C = toupper(C);

    if (mem.vars[B - 'A'] == NULL || mem.vars[C - 'A'] == NULL) {
        return UNINITIALISED_VARIABLE;
    }

    MemoryCell* before = get_memory_cell_by_name(mem, A);
    if (before == NULL) {
        fprintf(outp, "Before: %c is uninitialised.\n", A);
    }
    else {
        fprintf(outp, "Before: %c = %d.\n", A, before->val);
    }

    int ans = calculate(op, mem.vars[B - 'A']->val, mem.vars[C - 'A']->val);
    int st = assign_memory_cell(mem, A, ans);

    fprintf(outp, "After: %c = %d.\n\n", A, mem.vars[A - 'A']->val);
    return ok;
}

int process_unary_command(FILE* outp, Memory mem, char* command) {
    fprintf(outp, "Processing unary command...\n");
    char A = command[0];
    int i = 0;
    next_symbol(command, &i);
    i++;
    next_symbol(command, &i);

    int op = which_operation(command[i], 0);

    next_symbol(command, &i);
    char B = command[i];

    A = toupper(A);
    B = toupper(B);

    if (mem.vars[B - 'A'] == NULL) {
        return UNINITIALISED_VARIABLE;
    }

    MemoryCell* before = get_memory_cell_by_name(mem, A);
    if (before == NULL) {
        fprintf(outp, "Before: %c is uninitialised.\n", A);
    }
    else {
        fprintf(outp, "Before: %c = %d.\n", A, before->val);
    }

    int ans = calculate(op, mem.vars[B - 'A']->val, 0);
    int st = assign_memory_cell(mem, A, ans);

    fprintf(outp, "After: %c = %d.\n\n", A, mem.vars[A - 'A']->val);
    return ok;
}

int process_read_command(FILE* outp, Memory mem, char* command) {
    fprintf(outp, "Processing read command...\n");
    int i = 3;
    next_symbol(command, &i);
    next_symbol(command, &i);

    char A = command[i];
    A = toupper(A);

    next_symbol(command, &i);
    next_symbol(command, &i);

    char* num = (char*)malloc(strlen(command));
    if (num == NULL) {
        return MEMORY_ISSUES;
    }

    int ind = 0;
    while (isdigit(command[i])) {
        num[ind] = command[i];
        ind++; i++;
    }
    num[ind] = '\0';

    if (command[i] != ')') {
        next_symbol(command, &i);
    }

    if (command[i+1] != '\0') {
        free(num);
        return INVALID_COMMAND;
    }

    int base = atoi(num);
    free(num);
    num = NULL;
    if (errno == ERANGE) {
        return OVERFLOWED;
    }

    if (base < 2 || base > 36) {
        return INVALID_BASE;
    }

    printf("Enter the value of %c: ", A);
    int st = read_word_from_file(stdin, &num);
    fflush(stdin);
    if (st != ok) {
        return st;
    }

    if (!is_valid_base(num, base)) {
        free(num);
        return INVALID_NUMBER;
    }
    int value = 0;
    st = to_decimal(num, base, &value);
    free(num);
    num = NULL;
    if (st != ok) {
        return st;
    }

    MemoryCell* before = get_memory_cell_by_name(mem, A);
    if (before == NULL) {
        fprintf(outp, "Before: %c is uninitialised.\n", A);
    }
    else {
        fprintf(outp, "Before: %c = %d.\n", A, before->val);
    }

    st = assign_memory_cell(mem, A, value);

    fprintf(outp, "After: %c = %d.\n\n", A, mem.vars[A - 'A']->val);
    return st;
}

int process_write_command(FILE* outp, Memory mem, char* command) {
    fprintf(outp, "Processing write command...\n");
    int i = 4;
    next_symbol(command, &i);
    next_symbol(command, &i);

    char A = command[i];
    A = toupper(A);

    next_symbol(command, &i);
    next_symbol(command, &i);

    char* num = (char*)malloc(strlen(command));
    if (num == NULL) {
        return MEMORY_ISSUES;
    }

    int ind = 0;
    while (isdigit(command[i])) {
        num[ind] = command[i];
        ind++; i++;
    }
    num[ind] = '\0';

    if (command[i] != ')') {
        next_symbol(command, &i);
    }

    if (command[i+1] != '\0') {
        free(num);
        return INVALID_COMMAND;
    }

    int base = atoi(num);
    free(num);
    num = NULL;
    if (errno == ERANGE) {
        return OVERFLOWED;
    }

    if (base < 2 || base > 36) {
        return INVALID_BASE;
    }

    MemoryCell* found = get_memory_cell_by_name(mem, A);
    if (found == NULL) {
        return UNINITIALISED_VARIABLE;
    }

    char* res = my_itoa(found->val, base);
    if (res == NULL) {
        return MEMORY_ISSUES;
    }
    fprintf(outp, "%c = %s in base %d\n\n", A, res, base);
    free(res);
    return ok;
}

int process_file(FILE* inp, FILE* outp, Memory* mem) {
    int st = ok;
    char* command = NULL;
    while ((st = read_сommand_from_file(inp, &command)) == ok) {
        cut(command);
        
        if (is_valid_binary(command)) {
            st = process_binary_command(outp, *mem, command);
            if (st != ok && !is_runtime_error(st)) {
                free(command);
                return st;
            }
            print_error(outp, st);
        }
        else if (is_valid_unary(command)) {
            st = process_unary_command(outp, *mem, command);
            if (st != ok && !is_runtime_error(st)) {
                free(command);
                return st;
            }
            print_error(outp, st);
        }
        else if (is_valid_read(command)) {
            st = process_read_command(outp, *mem, command);
            if (st != ok && !is_runtime_error(st)) {
                free(command);
                return st;
            }
            print_error(outp, st);
        }
        else if (is_valid_write(command)) {
            st = process_write_command(outp, *mem, command);
            if (st != ok && !is_runtime_error(st)) {
                free(command);
                return st;
            }
            print_error(outp, st);
        }
        else {
            print_error(outp, INVALID_COMMAND);
        }
        fprintf(outp, "\n");
        free(command);
    }
    free(command);
    if (st == EOF) {
        return ok;
    }
    return st;
}

int are_equal_files(const char* f1, const char* f2) {
    char fp1[PATH_MAX];
    char fp2[PATH_MAX];

    char* st = realpath(f1, fp1);
    if (st == NULL) {
        return UNABLE_TO_OPEN_FILE;
    }

    st = realpath(f2, fp2);
    if (st == NULL) {
        return UNABLE_TO_OPEN_FILE;
    }

    if (strcmp(fp1, fp2) == 0) {
        return EQUAL_FILES;
    }
    return ok;
}

int is_valid_args(int argc, char** argv) {
    if (argc != 2 && argc != 4) {
        return INVALID_AMOUNT_OF_ARGUMENTS;
    }

    if (argc == 4 && strcmp(argv[2], "/trace") != 0) {
        return INVALID_FLAG;
    }

    // int st = ok;
    // if (argc == 4) {
    //     if ((st = are_equal_files(argv[1], argv[3])) != ok) {
    //         return st;
    //     }
    // }

    // if_equal_filenames

    return ok;
}

int main(int argc, char** argv) {
    // argc = 4;
    // argv[1] = "input.txt";
    // argv[2] = "/trace";
    // argv[3] = "output.txt";

    int st = ok;

    if ((st = is_valid_args(argc, argv)) != ok) {
        print_error(stdout, st);
        return 1;
    }

    FILE* inp = fopen(argv[1], "r");
    if (inp == NULL) {
        print_error(stdout, UNABLE_TO_OPEN_FILE);
        return -1;
    }
    FILE* outp = NULL;
    if (argc == 4) {
        outp = fopen(argv[3], "w+");
        if (outp == NULL) {
            print_error(stdout, UNABLE_TO_OPEN_FILE);
            fclose(inp);
            return -1;
        }
    }
    
    if (outp == NULL) {
        Memory memory;
        st = init_memory(&memory);
        if (st != ok) {
            print_error(stdout, st);
            fclose(inp);
            return 1;
        }
        
        st = process_file(inp, stdout, &memory);
        if (st != ok) {
            print_error(stdout, st);
            free_memory(memory);
            fclose(inp);
            return 1;
        }

        fclose(inp);
        free_memory(memory);
    }
    else {
        Memory memory;
        st = init_memory(&memory);
        if (st != ok) {
            print_error(outp, st);
            fclose(inp);
            fclose(outp);
            return 1;
        }
        
        st = process_file(inp, outp, &memory);
        if (st != ok) {
            print_error(outp, st);
            free_memory(memory);
            fclose(inp);
            fclose(outp);
            return 1;
        }

        fclose(inp);
        fclose(outp);
        free_memory(memory);
    }

    return 0;
}