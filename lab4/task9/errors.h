#ifndef ERRORS_H
#define ERRORS_H

enum status_code {
    ok,
    UNABLE_TO_OPEN_FILE,
    INVALID_FUNCTION_ARGUMENT,
    MEMORY_ISSUES,
    INVALID_FILE,

    INVALID_INPUT,
    BAD_ACCESS,

    INVALID_CONFIG_FILE,
    INVALID_AMOUNT_OF_ARGUMENTS,
    
};

enum runtime_error {

    INVALID_APPLICATION = 1,
    INVALID_TIME,
    INVALID_PRIORITY,
    INVALID_MESSAGE_ID,
    INVALID_TEXT,

    ZERO_POWERED_ZERO,
    OVERFLOW,
    INVALID_MAX_PRIORITY,
    INVALID_DEPARTMENT_NAME,
    INVALID_BASE
};

void print_error(FILE* outp_stream, const int st);
int print_runtime_error(FILE* outp_stream, int st);

#endif