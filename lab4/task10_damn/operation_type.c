#include "operation_type.h"

status_code is_in_base(char * number, int base)
{
    if(number == NULL)
        return INVALID_PARAMETER;
    if (base < 2 || base > 36) return INVALID_BASE;
    int i = 0;
    while (number[i])
    {
        if (!isdigit(number[i]) && !isalpha(number[i])) return VAR_ERROR;
        else if (isdigit(number[i]) && number[i] - '0' >= base) return BASE_ERROR;
        else if (isalpha(number[i]) && toupper(number[i]) - 'A' + 10 >= base) return BASE_ERROR;
        ++i;
    }
    return OK;
}

int get_symbols_count(uint32_t number, int base)
{
    if(number == 0)
        return 1;
    int count = 0;
    while (number > 0)
    {
        number /= base;
        count++;
    }
    return count;
}

status_code is_new_operation(Current_settings* settings, char * string, operation * operation_name)
{
    if(string == NULL)
        return INVALID_PARAMETER;
    for (int i = 0; i < OPERATIONS_COUNT; ++i)
    {
        if (strcmp(string, settings->operations_names[i]) == 0)
        {
            *operation_name = i;
            return OK;
        }
    }
    return BAD;
}

status_code is_operation(char * string, operation * operation_name)
{
    if(string == NULL)
        return INVALID_PARAMETER;
    for (int i = 0; i < OPERATIONS_COUNT; ++i)
    {
        if (strcmp(string, start_operations_names[i]) == 0)
        {
            *operation_name = i;
            return OK;
        }
    }
    return BAD;
}

status_code is_variable(char * string)
{
    if (!string) return LEXEME_ERROR;
    if (strlen(string) == 0) return LEXEME_ERROR;
    if (isdigit(string[0])) return VAR_ERROR;
    operation smh = NOT;
    if(is_operation(string, &smh) == OK) return BAD;
    for (int i = 0; string[i] != 0; ++i)
    {
        if (!isdigit(string[i]) && !isalpha(string[i]) && string[i] != '_') return VAR_ERROR;
    }
    return OK;
}

status_code is_debug(char * string)
{
    if(string == NULL)
        return BAD;
    int count = 3;
    char * variations[] = {"--debug", "-d", "/debug"};
    for (int i = 0; i < count; ++i) if (!strcmp(variations[i], string)) return OK;
    return BAD;
}