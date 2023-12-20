#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "Tree.h"
#include "arithmetic.h"

void print_error(const int st) {
    switch (st)
    {
    case UNABLE_TO_OPEN_FILE:
        fprintf(stdout, "Unable to open a file!\n");
        break;
    case MEMORY_ISSUES:
        printf("Unable to allocate the memory!\n");
        break;
    case OVERFLOWED:
        printf("Overflowed!\n");
        break;
    case INVALID_FUNCTION_ARGUMENT:
        printf("Invalid function argument!\n");
        break;
    case INVALID_AMOUNT_OF_ARGUMENTS:
        printf("Invalid amount of arguments!\n");
        break;
    case INVALID_CHARACTER:
        printf("Invalid character met!\n");
        break;
    case UNBALANCED_BRACKETS:
        printf("Unbalanced brackets!\n");
        break;
    case INVALIND_OPERATOR_POSTURE:
        printf("Invalid operator position!\n");
        break;
    case INVALID_EXPRESSION:
        printf("Invalid expression!\n");
        break;
    case NO_VALUE_IN_BRACKETS:
        printf("Empty brackets!\n");
        break;
    case EMPTY_EXPRESSION:
        printf("Empty expression!\n");
        break;
    case INVALID_VAR_NAME:
        printf("Invalid name of variable!\n");
        break;
    case DIVISION_BY_ZERO:
        printf("Division by zero!\n");
        break;
    case INVALID_BRACKETS:
        printf("Invalid brackets!\n");
        break;
    case ZERO_POW_ZERO:
        printf("Zero in power of zero!\n");
        break;
    case NEGATIVE_POWER:
        printf("Negative power!\n");
        break;
    default:
        break;
    }
}

void cut(char* str) {
    if (str == NULL) {
        return;
    }

    int ind = strlen(str);
    while (is_separator(str[ind]) || str[ind] == '\0') {
        ind--;
    }

    ind++;
    str[ind] = '\0';
}

int remove_right_angle(char* before, char** after) {
    char* ans = (char*)malloc(sizeof(char) * (strlen(before) + 1));
    if (ans == NULL) {
        free(before);
        return MEMORY_ISSUES;
    } 

    int ind1 = 0, ind2 = 0;
    while (before[ind1] != '\0') {
        if (before[ind1] == '>') {
            ind1++;
            continue;
        }
        ans[ind2] = before[ind1];
        ind1++; ind2++;
    }
    ans[ind2] = '\0';

    *after = ans;
    free(before);
    return ok;
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
    while (c != '\n' && c != EOF) {
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

    (*line)[ind] = '\0';

    if (c == EOF && ind == 0) {
        return EOF;
    }

    return ok;
}

int is_valid_expression(const char* str) {
    if (str == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    int st = ok;

    int ind = 0;
    //Stack* stack = NULL;
    int bracket_cnt = 0;
    while (str[ind] != '\0') {
        if (!is_allowed_char(str[ind]) && !is_operator(str[ind])
         && str[ind] != '(' && str[ind] != ')') {
            return INVALID_CHARACTER;
        }
        
        if (str[ind] == '(') {
            bracket_cnt++;
        }
        else if (str[ind] == ')') {
            if (bracket_cnt == 0) {
                return UNBALANCED_BRACKETS;
            }
            bracket_cnt--;
        }

        if (is_operator(str[ind])) {
            if (is_unar(str[ind])) {
                if (!((ind == 0 && (is_allowed_char(str[ind+1]) || str[ind] == '('))
                 || (((ind != 0 && (is_allowed_char(str[ind-1]) || str[ind-1] == '(' || str[ind-1] == ')')))
                  && is_allowed_char(str[ind+1]) || str[ind+1] == '('))) {
                    return INVALIND_OPERATOR_POSTURE;
                }
            }
            else {
                if ((str[ind] == '-' || str[ind] == '+' || str[ind] == '<')) { 
                    if (str[ind+1] != '>') {
                        return INVALIND_OPERATOR_POSTURE;
                    }
                    if (ind == 0 || (!is_allowed_char(str[ind-1]) && str[ind-1] != ')') ||
                     (!is_allowed_char(str[ind+2]) && str[ind+2] != '(')) {
                        return INVALIND_OPERATOR_POSTURE;
                    }
                    ind++;
                }
                else {
                    if (ind == 0 || (!is_allowed_char(str[ind-1]) && str[ind-1] != ')') ||
                    (!is_allowed_char(str[ind+1]) && str[ind+1] != '(')) {
                        return INVALIND_OPERATOR_POSTURE;
                    }
                }    
            }
        }
        else if (is_allowed_char(str[ind])) {
            if ((ind != 0 && is_allowed_char(str[ind-1])) || (is_allowed_char(str[ind+1]))) {
                return INVALID_VAR_NAME;
            }
            if (str[ind+1] == '(' || (ind != 0 && str[ind-1] == ')')) {
                return INVALID_EXPRESSION;   
            }
        }

        if (str[ind] == '(' && str[ind+1] == ')') {
            return NO_VALUE_IN_BRACKETS;
        }
        if (str[ind] == ')' && str[ind+1] == '(') {
            return INVALID_BRACKETS;
        }
        ind++;
    }

    if (ind == 0) {
        return EMPTY_EXPRESSION;
    }

    if (bracket_cnt != 0) {
        return UNBALANCED_BRACKETS;
    }

    return ok;
}

int includes_char(char * vars, char symbol)
{
    int ind = 0;
    while (vars[ind] != '\0') {
        if (vars[ind] == symbol) {
            return 1;
        }
        ind++;
    }
    return 0;
}

int get_variables(char* infix, char** variables) {

    int len = strlen(infix);

    *variables = (char*)malloc(sizeof(char) * (len+1));
    if (*variables == NULL) {
        return MEMORY_ISSUES;
    }
    int ind = 0;
    (*variables)[ind] = '\0';
    for (int i = 0; i < len; i++) {
        if (isalpha(infix[i]) && !includes_char(*variables, infix[i])) {
            (*variables)[ind] = infix[i];
            ind++;
            (*variables)[ind] = '\0';
        }
    }
    (*variables)[ind] = '\0';
    return ok;
}

int generate_file_name(char** name) {
    srand(time(NULL));
    int max_length = 10, min_length = 1;

    int length = rand() % (max_length - min_length + 1) + min_length; 

    char * file_name = (char*)malloc((length + 6) * sizeof(char));
    if (file_name == NULL) {
        return MEMORY_ISSUES;
    }
    file_name[length + 6] = 0;
    for (int i = 0; i <= length;) {
        file_name[i] = rand() % ('Z' - '0' + 1) + '0';
        if (isalpha(file_name[i]) || isdigit(file_name[i])) {
            i++;
        } 
    }
    file_name[length + 1] = '.';
    file_name[length + 2] = 't';
    file_name[length + 3] = 'x';
    file_name[length + 4] = 't';

    *name = file_name;
    return ok;
}

int generate_truth_table(FILE* outp, Node* root, const char* variables) {
    Node* tmp = root;
    
    int len = 0;
    while (variables[len] != '\0') {
        fprintf(outp, " %c |", variables[len]);
        len++;
    }

    fprintf(outp, " ");
    print_expr(outp, root);
    fprintf(outp, "\n");
    
    int combinations = 1 << len;

    char* variables_values = (char*)malloc(sizeof(char) * (len+1));
    if (variables_values == NULL) {
        return MEMORY_ISSUES;
    }
    variables_values[len] = '\0';

    int result = 0;
    for (int i = 0; i < combinations; ++i)
    {
        for (int j = 0; j < len; ++j)
        {
            fprintf(outp, " %d |", (i >> j) & 1);
            variables_values[j] = ((i >> j) & 1) + '0';
        }
        
        result = calculate_tree(root, variables, variables_values);
        fprintf(outp, " %d\n", result);
    }
    free(variables_values);
    return ok;
}

int main(int argc, char** argv) {

    if (argc != 2) {
        print_error(INVALID_AMOUNT_OF_ARGUMENTS);
        return 1;
    }

    FILE* inp = fopen(argv[1], "r");
    if (inp == NULL) {
        print_error(UNABLE_TO_OPEN_FILE);
        return -1;
    }
    
    int st = ok;
    char* expr = NULL;
    st = read_line(inp, &expr);
    if (st != ok) {
        if (st == EOF) {
            printf("No expression!\n");
            free(expr);
            fclose(inp);
            return 1;
        }
        free(expr);
        fclose(inp);
        return st;
    }
    fclose(inp);

    cut(expr);

    st = is_valid_expression(expr);
    if (st != ok) {
        free(expr);
        print_error(st);
        return 1;
    }

    char* variables = NULL;
    st = get_variables(expr, &variables);
    if (st != ok) {
        free(expr);
        print_error(st);
        return 1;
    }

    st = remove_right_angle(expr, &expr); 
    if (st != ok) {
        print_error(st);
        free(expr);
        free(variables);
        return 1;
    }

    Node* root = NULL;
    st = make_tree(&root, expr, 0, strlen(expr)-1);
    if (st != ok) {
        print_error(st);
        free(expr);
        free(variables);
        return 1;
    }
    free(expr);

    char* filename = NULL;
    st = generate_file_name(&filename);
    if (st != ok) {
        print_error(st);
        free(expr);
        free(variables);
        return 1;
    }
    FILE* outp = fopen(filename, "w+");
    free(filename);
    if (outp == NULL) {
        print_error(st);
        free(expr);
        free(variables);
        return 1;
    }

    st = generate_truth_table(outp, root, variables);
    if (outp == NULL) {
        print_error(st);
        free(expr);
        free(variables);
        fclose(outp);
        return 1;
    }

    free_tree(root);
    free(variables);
    fclose(outp);

    return 0;
}