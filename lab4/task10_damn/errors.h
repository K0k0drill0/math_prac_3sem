#ifndef _ERRORS_H_
#define _ERRORS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>

typedef enum
{
    OK,
    INVALID_COMMAND_ARG,
    ALLOC_ERROR,
    BAD,
    FILE_ERROR,
    INVALID_PARAMETER,
    INVALID_BASE,
    VAR_ERROR,
    BASE_ERROR,
    FIND_ERROR,
    LEXEME_ERROR,
    SETTINGS_ERROR
} status_code;

static const char* errors_str[] = 
{
        "Incorrect command line arguments.",
        "Memory error.",
        "File open error.",
        "Invalid function argument.",
        "Invalid base.",
        "Invalid variable.",
        "Variable is not in base.",
        "Variable is not initialized.",
        "Invalid lexeme.",
        "Invalid settings file."
};

void print_error(status_code const error);

status_code free_all(status_code error, int count, ...);

#endif