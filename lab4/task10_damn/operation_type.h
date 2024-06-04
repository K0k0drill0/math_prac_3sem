#ifndef _OPERATION_TYPE_H
#define _OPERATION_TYPE_H

#include "errors.h"
#include <ctype.h>
#include <string.h>

#define OPERATIONS_COUNT 13

typedef enum
{
    NOT,
    INPUT,
    OUTPUT,
    ADD,
    MULT,
    SUB,
    POW,
    DIV,
    REM,
    XOR,
    AND,
    OR,
    ASSIGN
} operation;

typedef enum
{
    left,
    right,
    middle
} operation_syntax;

typedef enum
{
    unary,
    binary
} operation_type;

typedef struct current_settings
{
    char ** operations_names;
    operation_type * basic_types;
    operation_syntax operation_syntax_type; //left/middle/right
    operation_syntax operation_result_type; //left/right left= right=
    int debug;

} Current_settings;

static char * start_operations_names[] = {"not", "input", "output", "add", "mult", "sub", "pow", "div", "rem", "xor", "and", "or", "="};
static int operation_prior[] = {          0,     0,         0,     3,  2,      3,      1,      2,      2,     4, 5,      6, 7};
Current_settings* create_current_settings(int debug);
void free_current_settings(Current_settings* settings);
status_code change_name(Current_settings* settings, operation operation_id, char * new_name);
void change_operation_result_type(Current_settings* settings, operation_syntax new_res_type);
void change_operation_syntax_type(Current_settings* settings, operation_syntax new_syntax_type);

status_code is_in_base(char * number, int base);
int get_symbols_count(uint32_t number, int base);
status_code is_variable(char * string);
status_code is_operation(char * string, operation * operation_name);
status_code is_new_operation(Current_settings* settings, char * string, operation * operation_name);
status_code is_debug(char * string);

#endif