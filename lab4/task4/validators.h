#ifndef VALIDATORS_H
#define VALIDATORS_H

enum status_code {
    ok, 
    MEMORY_ISSUES,
    UNABLE_TO_OPEN_FILE,
    INVALID_FUNCTION_PARAMETER,
    INVALID_AMOUNT_OF_ARGUMENTS,
    INVALID_FLAG,
    EQUAL_FILES,

    INVALID_COMMENT,
    INVALID_COMMAND,
    INVALID_OPERATION,
    OVERFLOWED,
    INVALID_BASE,
    INVALID_NUMBER,
    UNINITIALISED_VARIABLE,

    DISJUNCTION,
    CONJUNCTION,
    IMPLICATION,
    REVERSED_IMPLICATION,
    EQUIVALENCE,
    MOD2,
    COIMPLICATION,
    SHEFFER,
    PIERCE,
    NEGATION
};

int is_separator(const char c);
int is_valid_int(const char* str);
int is_valid_uint(const char* str);
int is_runtime_error(const int er);
int which_operation(const char c1, const char c2);
int next_symbol(const char* str, int* ind);

int is_valid_binary(const char* command);
int is_valid_unary(const char* command);
int is_valid_read(const char* command);
int is_valid_base(const char* num, int base);
int is_valid_write(const char* command);

int which_number(char c);
int to_decimal(char* str, int base, int* ans);
char* my_itoa(int decimal, int new_sn);

#endif