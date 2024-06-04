#include "errors.h"

void print_error(status_code const error)
{
    const char * main_error = "Error occured: ";
    switch (error)
    {
        case INVALID_COMMAND_ARG:
            printf("%s %s\n", main_error, errors_str[0]);
            break;
        case ALLOC_ERROR:
            printf("%s %s\n", main_error, errors_str[1]);
            break;
        case FILE_ERROR:
            printf("%s %s\n", main_error, errors_str[2]);
            break;
        case INVALID_PARAMETER:
            printf("%s %s\n", main_error, errors_str[3]);
            break;
        case INVALID_BASE:
            printf("%s %s\n", main_error, errors_str[4]);
            break;
        case VAR_ERROR:
            printf("%s %s\n", main_error, errors_str[5]);
            break;
        case BASE_ERROR:
            printf("%s %s\n", main_error, errors_str[6]);
            break;
        case FIND_ERROR:
            printf("%s %s\n", main_error, errors_str[7]);
            break;
        case LEXEME_ERROR:
            printf("%s %s\n", main_error, errors_str[8]);
            break;
        case SETTINGS_ERROR:
            printf("%s %s\n", main_error, errors_str[9]);
            break;

        default: break;
    }
}

status_code free_all(status_code error, int count, ...)
{
    va_list strings;
    va_start(strings, count);
    for (int i = 0; i < count; ++i)
    {
        char * string = va_arg(strings, char*);
        if (string)
        {
            free(string);
            string = NULL;
        }
    }
    va_end(strings);
    return error;
}