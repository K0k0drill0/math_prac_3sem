#include <stdio.h>
#include "errors.h"

void print_error(FILE* outp_stream, const int st) {
    switch (st)
    {
    case UNABLE_TO_OPEN_FILE:
        fprintf(outp_stream, "Unable to open a file!\n");
        break;
    case INVALID_FUNCTION_ARGUMENT:
        fprintf(outp_stream, "Invalid argument in function!\n");
        break;
    case MEMORY_ISSUES:
        fprintf(outp_stream, "Unable to allocate the memory!\n");
        break;
    case INVALID_FILE:
        fprintf(outp_stream, "Invalid file!\n");
        break;
    case INVALID_INPUT:
        fprintf(outp_stream, "Invalid input!\n");
        break;
    case BAD_ACCESS:
        fprintf(outp_stream, "Bad access!\n");
        break;
    
    case INVALID_CONFIG_FILE:
        fprintf(outp_stream, "Invalid config file!\n");
        break;
    
    case INVALID_AMOUNT_OF_ARGUMENTS:
        fprintf(outp_stream, "Invalid amount of arguments!\n");
        break;
    default:
        break;
    }
}

int print_runtime_error(FILE* outp_stream, int st) {
    switch (st)
    {
    case INVALID_APPLICATION:
        fprintf(outp_stream, "Invalid application!\n");
        break;
    case INVALID_TIME:
        fprintf(outp_stream, "Invalid time!\n");
        break;
    case INVALID_PRIORITY:
        fprintf(outp_stream, "Invalid priority!\n");
        break;
    case INVALID_MESSAGE_ID:
        fprintf(outp_stream, "Invalid message id!\n");
        break;
    case ZERO_POWERED_ZERO:
        fprintf(outp_stream, "Zero in power of zero!\n");
        break;
    case OVERFLOW:
        fprintf(outp_stream, "Overflow!\n");
        break;
    case INVALID_MAX_PRIORITY:
        fprintf(outp_stream, "Invalid max priority!\n");
        break;
    case INVALID_TEXT:
        fprintf(outp_stream, "Invalid text!\n");
        break;
    case INVALID_DEPARTMENT_NAME:
        fprintf(outp_stream, "Invalid department name!\n");
        break;
    case INVALID_BASE:
        fprintf(outp_stream, "Invalid base!");
        break;
    default:
        return INVALID_FUNCTION_ARGUMENT;
        break;
    }

    return ok;
}