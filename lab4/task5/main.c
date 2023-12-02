#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "stack.h"
#include "arr.h"

enum status_code {
    ok, 
    UNABLE_TO_OPEN_FILE,
    MEMORY_ISSUES,
    OVERFLOWED,
    INVALID_FUNCTION_ARGUMENT,

    INVALID_CHARACTER,
    UNBALANCED_BRACKETS,
    INVALIND_OPERATOR_POSTURE,
    NO_VALUE_IN_BRACKETS, 
    EMPTY_EXPRESSION,

    DIVISION_BY_ZERO,
    ZERO_POW_ZERO,
    NEGATIVE_POWER
};

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
    case INVALID_CHARACTER:
        printf("Invalid character met!\n");
        break;
    case UNBALANCED_BRACKETS:
        printf("Unbalanced brackets!\n");
        break;
    case INVALIND_OPERATOR_POSTURE:
        printf("Invalid operator position!\n");
        break;
    case NO_VALUE_IN_BRACKETS:
        printf("Empty brackets!\n");
        break;
    case EMPTY_EXPRESSION:
        printf("Empty expression!\n");
        break;
    case DIVISION_BY_ZERO:
        printf("Division by zero!\n");
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

int is_arithmetic_operation(const char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^');
}

int is_separator(const char c) {
    return (c == ' ' || c == '\t' || c == '\n');
}

int cut_line(char* str) {
    if (str == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    int ind = 0;
    while (str[ind] != '\0') ind++;
    ind--;

    while (is_separator(str[ind])) {
        ind--;
    }
    ind++;
    str[ind] = '\0';
    return ok;
}

int priority(const char op) {
    if (op == '^' || op == '~') {
        return 3;
    } else if (op == '*' || op == '/') {
        return 2;
    } else if (op == '+' || op == '-') {
        return 1;
    } else {
        return 0;
    }
}

int bpow(int x, int n, int* ans) {
    if (n < 0 || (x == 0 && n == 0)) return INVALID_FUNCTION_ARGUMENT; 
    if (n == 0) {
        *ans = 1;
        return ok;
    }
    if (n == 1) {
        *ans = x;
        return ok;
    } 
    int half;
    int st = bpow(x, n / 2, &half);
    if (st != ok) {
        return st;
    }
    if (!(n & 1)) {
        *ans = half * half;
        return ok;
    } else  {
        *ans = half * half * x;
        return ok;
    }
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
    Stack* stack = NULL;
    while (str[ind] != '\0') {
        if (!isdigit(str[ind]) && !is_arithmetic_operation(str[ind])
         && str[ind] != '(' && str[ind] != ')') {
            return INVALID_CHARACTER;
        }
        
        if (str[ind] == '(') {
            st = stack_push(&stack, "(");
            if (st != ok) {
                stack_destroy(&stack);
                return st;
            }
        }
        else if (str[ind] == ')') {
            if (stack_is_empty(stack)) {
                stack_destroy(&stack);
                return UNBALANCED_BRACKETS;
            }
            st = stack_pop(&stack);
            if (st != ok) {
                stack_destroy(&stack);
                return st;
            }
        }

        if (is_arithmetic_operation(str[ind])) {
            if (str[ind] == '-') {
                if (!(ind == 0 && (isdigit(str[ind+1] || str[ind] == '('))
                 || ((isdigit(str[ind-1]) || str[ind-1] == '(' || str[ind-1] == ')')
                  && isdigit(str[ind+1]) || str[ind+1] == '('))) {
                    stack_destroy(&stack);
                    return INVALIND_OPERATOR_POSTURE;
                }
            }
            else {
                if (ind == 0 || (!isdigit(str[ind-1]) && str[ind-1] != ')') ||
                (!isdigit(str[ind+1]) && str[ind+1] != '(')) {
                    stack_destroy(&stack);
                    return INVALIND_OPERATOR_POSTURE;
                }
            }
        }

        if (str[ind] == '(' && str[ind+1] == ')') {
            stack_destroy(&stack);
            return NO_VALUE_IN_BRACKETS;
        }
        ind++;
    }

    if (ind == 0) {
        stack_destroy(&stack);
        return EMPTY_EXPRESSION;
    }

    if (!stack_is_empty(stack)) {
        stack_destroy(&stack);
        return UNBALANCED_BRACKETS;
    }
    stack_destroy(&stack);

    return ok;
}

int read_number(const char* str, int* ind, char** num) {
    if (str == NULL || *ind < 0) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    int num_max_size = 16;
    int num_tmp_size = 0;
    *num = (char*)malloc(sizeof(char) * num_max_size);
    if (*num == NULL) {
        return MEMORY_ISSUES;
    }

    while (isdigit(str[*ind])) {
        if (num_tmp_size + 2 == num_max_size) {
            num_max_size *= 2; 
            char* tmp = (char*)realloc(*num, num_max_size);
            if (tmp == NULL) {
                return MEMORY_ISSUES;
            }
        }
        (*num)[num_tmp_size] = str[*ind];
        num_tmp_size++;
        (*ind)++;
    }
    (*ind)--;
    (*num)[num_tmp_size] = '\0';
    return ok;
}

int string_concatenation(char* s1, const char* s2) {
    if (s1 == NULL || s2 == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    int ind = 0;
    while (s1[ind] != '\0') {
        ind++;
    }
    s1[ind] = ' ';
    ind++;

    int si = 0;
    while (s2[si] != '\0') {
        s1[ind] = s2[si];
        ind++;
        si++;
    }
    s1[ind] = '\0';
    return ok;
}

int from_stack_to_postfix_expr(Stack** stack, char** str) {
    Stack* tmp = NULL;
    int st = ok;

    int new_len = 0;
    while (!stack_is_empty(*stack)) {
        char* top = NULL;
        st = stack_top(*stack, &top);
        if (st != ok) {
            stack_destroy(&tmp);
            return st;
        }

        new_len += strlen(top) + 1;

        st = stack_push(&tmp, top);
        if (st != ok) {
            stack_destroy(&tmp);
            return st;
        }

        st = stack_pop(stack);
        if (st != ok) {
            stack_destroy(&tmp);
            return st;
        }
    }

    *str = (char*)malloc(sizeof(char) * (new_len+1));
    if (*str == NULL) {
        stack_destroy(&tmp);
        return MEMORY_ISSUES;
    }
    (*str)[0] = '\0';

    while (!stack_is_empty(tmp)) {
        char* top = NULL;
        st = stack_top(tmp, &top);
        if (st != ok) {
            stack_destroy(&tmp);
            return st;
        }
        
        st = string_concatenation(*str, top);
        if (st != ok) {
            stack_destroy(&tmp);
            return st;
        }

        st = stack_push(stack, top);
        if (st != ok) {
            stack_destroy(&tmp);
            return st;
        }

        st = stack_pop(&tmp);
        if (st != ok) {
            stack_destroy(&tmp);
            return st;
        }
    }
    (*str)[new_len] = '\0';
    return ok;
}

int from_infix_to_postfix(Stack** california, const char* str) {
    *california = NULL;
    int st = ok;

    Stack* texas = NULL;

    int ind = 0;
    while (str[ind] != '\0') {
        if (isdigit(str[ind])) {
            char* num = NULL;
            st = read_number(str, &ind, &num);
            if (st != ok) {
                stack_destroy(&texas);
                free(num);
                return st;
            }
            st = stack_push(california, num);
            free(num);
            if (st != ok) {
                stack_destroy(&texas);
                return st;
            }
        }
        else if (str[ind] == '(') {
            st = stack_push(&texas, "(");
            if (st != ok) {
                stack_destroy(&texas);
                return st;
            }
        }
        else if (str[ind] == ')') {
            char* top = NULL;
            st = stack_top(texas, &top);
            if (st != ok) {
                stack_destroy(&texas);
                return st;
            }
            while (top[0] != '(') {
                char* new_el = NULL;
                st = my_strcpy(&new_el, top);
                if (st != ok) {
                    free(new_el);
                    stack_destroy(&texas);
                    return st;
                }

                st = stack_push(california, new_el);
                free(new_el);
                if (st != ok) {
                    stack_destroy(&texas);
                    return st;
                }

                st = stack_pop(&texas);
                if (st != ok) {
                    stack_destroy(&texas);
                    return st;
                }

                st = stack_top(texas, &top);
                if (st != ok) {
                    stack_destroy(&texas);
                    return st;
                }
            }
            st = stack_pop(&texas);
            if (st != ok) {
                stack_destroy(&texas);
                return st;
            }
        }
        else if (is_arithmetic_operation(str[ind])) {
            char op = str[ind];
            if (op == '-' && (ind == 0 || (str[ind-1] == '('))) {
                op = '~';
            }

            char* top = NULL;
            if (!stack_is_empty(texas)) {
                st = stack_top(texas, &top);
                if (st != ok) {
                    stack_destroy(&texas);
                    return st;
                }
            }
            else {
                top = " ";
            }
            
            while (priority(top[0]) >= priority(op)) {
                char* new_el = NULL;
                st = my_strcpy(&new_el, top);
                if (st != ok) {
                    free(new_el);
                    stack_destroy(&texas);
                    return st;
                }

                st = stack_push(california, new_el);
                free(new_el);
                if (st != ok) {
                    stack_destroy(&texas);
                    return st;
                }

                st = stack_pop(&texas);
                if (st != ok) {
                    stack_destroy(&texas);
                    return st;
                }

                if (!stack_is_empty(texas)) {
                    st = stack_top(texas, &top);
                    if (st != ok) {
                        stack_destroy(&texas);
                        return st;
                    }
                }
                else {
                    top = " ";
                }
                
            }

            char new_el[2];
            sprintf(new_el, "%c", op);
            st = stack_push(&texas, new_el);
            if (st != ok) {
                stack_destroy(&texas);
                return st;
            }
        }  
        ind++;
    }

    while (!stack_is_empty(texas)) {
        char* top = NULL;
        st = stack_top(texas, &top);
        if (st != ok) {
            stack_destroy(&texas);
            return st;
        }

        st = stack_push(california, top);
        if (st != ok) {
            stack_destroy(&texas);
            return st;
        }

        st = stack_pop(&texas);
        if (st != ok) {
            stack_destroy(&texas);
            return st;
        }
    }

    return ok;
} 

int arithmetic(char op, int n1, int n2, int* ans) {
    switch (op)
    {
    case '+':
        *ans = n1 + n2;
        break;
    case '-':
        *ans = n1 - n2;
        break;
    case '*':
        *ans = n1 * n2;
        break;
    case '/':
        if (n2 == 0) {
            return DIVISION_BY_ZERO;
        }
        *ans = n1 / n2;
        break;
    case '%':
        if (n2 == 0) {
            return DIVISION_BY_ZERO;
        }
        *ans = n1 % n2;
        break;
    case '^':
        if (n1 == 0 && n2 == 0) {
            return ZERO_POW_ZERO;
        }
        if (n2 < 0) {
            return NEGATIVE_POWER;
        }

        int st = bpow(n1, n2, ans);
        if (st != ok) {
            return st;
        }
        break;
    default:
        break;
    }
    return ok;
} 

int calculate_postfix_expr(Stack** stack, int* ans) {
    if (*stack == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    int st = stack_reverse(stack);
    if (st != ok) {
        return st;
    }

    Stack* numbers = NULL;
    Stack* tmp = NULL;

    while (!stack_is_empty(*stack)) {
        char* top = NULL;
        st = stack_top(*stack, &top);
        if (st != ok) {
            stack_destroy(&numbers);
            stack_destroy(&tmp);
            return st;
        }

        st = stack_push(&tmp, top);
        if (st != ok) {
            stack_destroy(&numbers);
            stack_destroy(&tmp);
            return st;
        }

        if (isdigit(top[0])) {
            st = stack_push(&numbers, top);
            if (st != ok) {
                stack_destroy(&numbers);
                stack_destroy(&tmp);
                return st;
            }
        }
        else {
            int num1, num2;
            char* snum1 = NULL, *snum2 = NULL;
            char new_num[12];
            if (top[0] == '~') {
                st = stack_top(numbers, &snum1);
                if (st != ok) {
                    stack_destroy(&numbers);
                    stack_destroy(&tmp);
                    return st;
                }
                num1 = atoi(snum1);
                if (errno == ERANGE) {
                    stack_destroy(&numbers);
                    stack_destroy(&tmp);
                    return OVERFLOWED;
                }
                st = stack_pop(&numbers);
                if (st != ok) {
                    stack_destroy(&numbers);
                    stack_destroy(&tmp);
                    return st;
                }
                num1 *= -1;
                sprintf(new_num, "%d", num1);
                st = stack_push(&numbers, new_num);
                if (st != ok) {
                    stack_destroy(&numbers);
                    stack_destroy(&tmp);
                    return st;
                }
            }
            else {
                st = stack_top(numbers, &snum1);
                if (st != ok) {
                    stack_destroy(&numbers);
                    stack_destroy(&tmp);
                    return st;
                }
                num1 = atoi(snum1);

                st = stack_pop(&numbers);
                if (st != ok) {
                    stack_destroy(&numbers);
                    stack_destroy(&tmp);
                    return st;
                }

                st = stack_top(numbers, &snum2);
                if (st != ok) {
                    stack_destroy(&numbers);
                    stack_destroy(&tmp);
                    return st;
                }
                num2 = atoi(snum2);

                st = stack_pop(&numbers);
                if (st != ok) {
                    stack_destroy(&numbers);
                    stack_destroy(&tmp);
                    return st;
                }

                if (errno == ERANGE) {
                    stack_destroy(&numbers);
                    stack_destroy(&tmp);
                    return OVERFLOWED;
                }

                int ans;
                st = arithmetic(top[0], num2, num1, &ans);
                if (st != ok) {
                    stack_destroy(&numbers);
                    stack_destroy(&tmp);
                    return st;
                }

                if (errno == ERANGE) {
                    stack_destroy(&numbers);
                    stack_destroy(&tmp);
                    return OVERFLOWED;
                }

                sprintf(new_num, "%d", ans);
                st = stack_push(&numbers, new_num);
                if (st != ok) {
                    stack_destroy(&numbers);
                    stack_destroy(&tmp);
                    return st;
                }
            }
        }
        st = stack_pop(stack);
        if (st != ok) {
            stack_destroy(&numbers);
            stack_destroy(&tmp);
            return st;
        }
    }

    char* ans_str = NULL;
    st = stack_top(numbers, &ans_str);
    if (st != ok) {
        stack_destroy(&numbers);
        stack_destroy(&tmp);
        return st;
    }
    *ans = atoi(ans_str);

    *stack = tmp;
    stack_destroy(&numbers);
    return ok;
}

int process_file(FILE* inp, Array* ans) {
    int st = ok, res;
    char* infix = NULL;
    char* postfix = NULL;
    Stack* expr = NULL;
    Expression new;
    int line_cnt = 0;
    while ((st = read_line(inp, &infix)) == ok) {
        cut_line(infix);
        new.infix = infix;
        new.line = line_cnt;
        new.postfix = NULL;
        new.res = 0;
        new.err = ok;

        st = is_valid_expression(infix);
        if (st == ok) {
            st = from_infix_to_postfix(&expr, infix);
            if (st != ok) {
                free(infix);
                stack_destroy(&expr);
                return st;
            }

            st = from_stack_to_postfix_expr(&expr, &postfix);
            if (st != ok) {
                free(infix);
                stack_destroy(&expr);
                return st;
            }

            st = calculate_postfix_expr(&expr, &res);
            if (st >= DIVISION_BY_ZERO) {
                new.err = st;
            }
            new.postfix = postfix;
            new.res = res;
            stack_destroy(&expr);
        }
        else if (st >= INVALID_CHARACTER) {
            new.err = st;
        }
        else {
            free(infix);
            stack_destroy(&expr);
            return st;
        }
        
        st = arr_append(ans, new);
        if (st != ok) {
            free(infix);
            stack_destroy(&expr);
            return st;
        }
        line_cnt++;
    }
    stack_destroy(&expr);
    free(infix);
    if (st != EOF) {
        return st;
    }
    return ok;
}

void add_out(char* filename) {
    int ind = 0;
    while (filename[ind] != '\0') ind++;
    
    while (ind != 0) {
        filename[ind+4] = filename[ind];
        ind--;
    }
    filename[ind+4] = filename[ind];

    filename[0] = 'o', filename[1] = 'u', filename[2] = 't', filename[3] = '_';
}

int main(int argc, char** argv) {

    argc = 4;
    argv[1] = "input.txt", argv[2] = "input2.txt", argv[3] = "input3.txt";

    int st = ok;

    for (int i = 1; i < argc; i++) {
        FILE* inp = fopen(argv[i], "r");
        if (inp == NULL) {
            print_error(UNABLE_TO_OPEN_FILE);
            return -1;
        }

        char* new_filename = (char*)malloc(sizeof(char) * (strlen(argv[i]) + 5));
        if (new_filename == NULL) {
            fclose(inp);
            print_error(MEMORY_ISSUES);
            return 1;
        }
        strcpy(new_filename, argv[i]);
        add_out(new_filename);
        FILE* outp = fopen(new_filename, "w+");
        //free(new_filename);
        if (outp == NULL) {
            fclose(inp);
            free(new_filename);
            print_error(UNABLE_TO_OPEN_FILE);
            return -1;
        }

        Array ans;
        st = arr_init(&ans);
        if (st != ok) {
            fclose(inp);
            fclose(outp);
            free(new_filename);
            print_error(st);
            return 1;
        }

        st = process_file(inp, &ans);
        if (st != ok) {
            fclose(inp);
            fclose(outp);
            free(new_filename);
            arr_delete(&ans);
            print_error(st);
            return 1;
        }    

        printf("For file %s:\n", argv[i]);
        arr_print(outp, ans);
        printf("\n");
        arr_delete(&ans);

        fseek(outp, 0, SEEK_END);
        long pos = ftell(outp);
        if(pos == 0) {
            remove(new_filename);
            free(new_filename);
            fclose(inp);
        }
        else {
            free(new_filename);
            fclose(inp);
            fclose(outp);
        }

    }

    return 0;

    // FILE* inp = fopen("input.txt", "r");
    // Array ans;
    // int st = arr_init(&ans);
    // if (st != ok) {
    //     print_error(st);
    //     return 1;
    // }
    // st = process_file(inp, &ans);
    // if (st != ok) {
    //     print_error(st);
    //     arr_delete(&ans);
    //     fclose(inp);
    //     return 1;
    // }
    
    // arr_print(stdout, ans);
    // arr_delete(&ans);
    // fclose(inp);
    // return 0;
}