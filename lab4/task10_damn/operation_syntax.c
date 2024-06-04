#include "operation_syntax.h"

status_code convert_to_decimal_base(char * to_convert, int original_base, uint32_t * result)
{
    if (!to_convert) return INVALID_PARAMETER;
    status_code error;
    if ((error = is_in_base(to_convert, original_base)) != OK) return error;

    uint32_t preresult = 0;
    int size = strlen(to_convert);
    int power = 1;
    for (int i = size - 1; i >= 0; --i)
    {
        if (isdigit(to_convert[i])) preresult += (power) * (to_convert[i] - '0');
        else preresult += (power) * (toupper(to_convert[i]) - 'A' + 10);
        power *= original_base;
    }
    *result = preresult;
    return OK;
}

status_code convert_to_base(uint32_t to_convert, int to_base, char ** result)
{
    if (to_base < 2 || to_base > 36) return INVALID_BASE;
    int size = get_symbols_count(to_convert, to_base);
    *result = (char*)malloc((size + 1) * sizeof(char));
    if (!*result) return ALLOC_ERROR;
    (*result)[size] = 0;
    int i = size - 1;
    while (i >= 0)
    {
        int remainder = to_convert % to_base;
        if (remainder < 10) (*result)[i] = remainder + '0';
        else (*result)[i] = remainder - 10 + 'A';
        to_convert /= to_base;
        --i;
    }
    return OK;
}

//trying_to_find_variable in Trie*
status_code get_value(Trie* trie, char * name, uint32_t * result, int original_base) 
{
    Trie_node* node = NULL;
    uint32_t value;
    status_code error = Trie_find(trie, name, &node);

    if (error == OK) value = node->value;
    else if (error == FIND_ERROR)
    {
        error = convert_to_decimal_base(name, original_base, &value);
        if (error != OK) return error;
    }
    else return error;
    *result = value;
    return OK;
}

status_code not_operation(Trie* trie, uint32_t value, uint32_t * result)
{
    *result = ~value;
    return OK;
}

status_code output_operation(Trie* trie, uint32_t value, int output_base)
{
    if (output_base != 10)
    {
        char * result = NULL;
        status_code error;
        if ((error = convert_to_base(value, output_base, &result)) != OK) return error;
        printf("%s\n", result);
        free(result);
    }
    else printf("%d\n", value);
    return OK;
}

status_code input_operation(Trie* trie, int input_base, uint32_t * result, char * name)
{
    char * line = NULL;
    size_t size = 0;
    if (getline(&line, &size, stdin) < 0) return VAR_ERROR;
    size = strlen(line);
    while (line[size - 1] == '\n') line[size - 1] = 0;
    status_code error;
    if ((error = convert_to_decimal_base(line, input_base, result)) != OK)
    {
        free(line);
        return error;
    }
    if ((error = Trie_insert(trie, name, *result)) != OK)
    {
        free(line);
        return error;
    }
    free(line);
    return OK;
}

status_code add_operation(Trie* trie, uint32_t value_1, uint32_t value_2, uint32_t * result)
{
    *result = value_1 + value_2;
    return OK;
}

status_code mult_operation(Trie* trie, uint32_t value_1, uint32_t value_2, uint32_t * result)
{
    *result = value_1 * value_2;
    return OK;
}

status_code sub_operation(Trie* trie, uint32_t value_1, uint32_t value_2, uint32_t * result)
{
    *result = value_1 - value_2;
    return OK;
}

uint32_t fast_mod_pow(uint32_t base, uint32_t exponent, uint32_t modulus) 
{
    if (exponent == 0) return 1;
    uint32_t z = fast_mod_pow(base % modulus, exponent / 2, modulus) % modulus;
    if (exponent % 2 == 0) return (z * z) % modulus;
    return ((base % modulus) * ((z * z) % modulus)) % modulus;

}

status_code pow_operation(Trie* trie, uint32_t value_1, uint32_t value_2, uint32_t * result)
{
    *result = fast_mod_pow(value_1, value_2, (1 << 31));
    return OK;
}

status_code div_operation(Trie* trie, uint32_t value_1, uint32_t value_2, uint32_t * result)
{
    if (value_2 == 0) return LEXEME_ERROR;
    *result = value_1 / value_2;
    return OK;
}

status_code rem_operation(Trie* trie, uint32_t value_1, uint32_t value_2, uint32_t * result)
{
    if (value_2 == 0) return LEXEME_ERROR;
    *result = value_1 % value_2;
    return OK;
}

status_code xor_operation(Trie* trie, uint32_t value_1, uint32_t value_2, uint32_t * result)
{
    *result = value_1 ^ value_2;
    return OK;
}

status_code and_operation(Trie* trie, uint32_t value_1, uint32_t value_2, uint32_t * result)
{
    *result = value_1 & value_2;
    return OK;
}

status_code or_operation(Trie* trie, uint32_t value_1, uint32_t value_2, uint32_t * result)
{
    *result = value_1 | value_2;
    return OK;
}

//------------------------------------------------------------------------------------------
