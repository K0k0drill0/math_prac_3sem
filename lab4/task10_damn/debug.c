#include "debug.h"
#define _GNU_SOURCE
#include <stdio.h>


status_code print_all_variables(Trie_node* node, char * prefix)
{
    if (!node) return OK;
    size_t size = strlen(prefix);
    char * new_prefix = NULL;
    status_code error;

    if (node->is_final) printf("%s : %d\n", prefix, node->value);

    for (int i = 0; i < VARIABLE_CHARAS_COUNT; ++i)
    {
        new_prefix = (char*)calloc(size + 2, sizeof(char));
        if (!new_prefix) return ALLOC_ERROR;
        strcpy(new_prefix, prefix);

        char chara;
        if (i < 10) chara = i + '0';
        else if (i < 62) chara = i + 'A';
        else chara = '_';

        new_prefix[size] = chara;
        new_prefix[size + 1] = 0;

        if ((error = print_all_variables(node->childs[i], new_prefix)) != OK)
        {
            free(new_prefix);
            return error;
        }
        free(new_prefix);
    }
    return OK;
}

status_code print_variables(Trie* trie)
{
    char * prefix = (char*)calloc(1, sizeof(char));
    if (!prefix) return ALLOC_ERROR;
    status_code error = OK;
    if ((error = print_all_variables(trie->root, prefix)) != OK)
        return free_all(error, 1, prefix); 

     return free_all(error, 1, prefix); 
}

void print_memory_dump(void * ptr, size_t size)
{
    unsigned char * byte_p = (unsigned char *)ptr;
    for (size_t i = 0; i < size; ++i)
    {
        for (int j = 7; j >= 0; --j) printf("%d", (byte_p[i] >> j) & 1);
        if (i < size - 1) printf(" ");
    }
    printf("\n");
}

status_code print_variable_and_value(Trie* trie)
{
    char * line = NULL;
    size_t size = 0;
    char * result = NULL;
    status_code error = OK;
    printf("\nWrite down the variable name >> ");

    if (getline(&line, &size, stdin) < 0) return VAR_ERROR;
    size = strlen(line);
    if (line[size - 1] == '\n') line[size - 1] = 0;

    while (is_variable(line) != OK)
    {
        if (strlen(line) != 0)
        {
            printf("\nTry again\n");
            printf("\nWrite down the variable name >> ");
        }
        line = NULL;
        size = 0;
        if (getline(&line, &size, stdin) < 0) return VAR_ERROR;
        size = strlen(line);
        if (line[size - 1] == '\n') line[size - 1] = 0;
    }

    //добились нормального названия
    Trie_node* node = NULL;
    if ((error = Trie_find(trie, line, &node)) != OK)
    {
        return free_all(error, 2, line, result);
    }

    if ((error = convert_to_base(node->value, 16, &result)) != OK)
    {
        return free_all(error, 2, line, result);
    }

    printf("variable: %s\nvalue in 16 base: %s\ndump: ", line, result);
    print_memory_dump(&(node->value), sizeof(node->value));
    printf("\n");
    return free_all(error, 2, line, result);
}

status_code change_variable_value(Trie* trie)
{
    char * line = NULL;
    size_t size = 0;
    status_code error = OK;
    printf("\nWrite down the variable name >> ");

    //жесткое считывание
    if (getline(&line, &size, stdin) < 0) return VAR_ERROR;
    size = strlen(line);
    if (line[size - 1] == '\n') line[size - 1] = 0;
    while (is_variable(line) != OK)
    {
        if (strlen(line) != 0)
        {
            printf("\nTry again\n");
            printf("\nWrite down the variable name >> ");
        }
        line = NULL;
        size = 0;
        if (getline(&line, &size, stdin) < 0) return VAR_ERROR;
        size = strlen(line);
        if (line[size - 1] == '\n') line[size - 1] = 0;
    }

    char * name = line;

    line = NULL;
    size = 0;
    printf("\nWrite down new value >> ");


    if (getline(&line, &size, stdin) < 0) 
    {
        free(name);
        return ALLOC_ERROR;
    }
    size = strlen(line);
    if (line[size - 1] == '\n') line[size - 1] = 0;
    uint32_t value;

    while ((error = get_value(trie, line, &value, 16)) != OK)
    {
        if (strlen(line) != 0)
        {
            printf("\nTry again\n");
            printf("\nWrite down the variable name >> ");
        }
        free(line);
        line = NULL;
        if (getline(&line, &size, stdin) < 0) 
        {
            free(name);
            return VAR_ERROR;
        }
        size = strlen(line);
        if (line[size - 1] == '\n') line[size - 1] = 0;
    }

    Trie_node* node = NULL;
    if ((error = Trie_find(trie, name, &node)) != OK) 
    {
        return free_all(error,2, name, line);
    }
    node->value = value;
    printf("\nOK!\n");

    free_all(error, 2, name, line);
    return error;
}

status_code insert_new_variable(Trie* trie, char ** var_name)
{
    char * line = NULL;
    size_t size = 0;
    status_code error = OK;
    printf("\nWrite down the variable name >> ");
    if (getline(&line, &size, stdin) < 0) return VAR_ERROR;
    size = strlen(line);
    if (line[size - 1] == '\n') line[size - 1] = 0;
    while (is_variable(line) != OK)
    {
        if (strlen(line) != 0)
        {
            printf("\nTry again\n");
            printf("\nWrite down the variable name >> ");
        }
        line = NULL;
        size = 0;
        if (getline(&line, &size, stdin) < 0) return VAR_ERROR;
        size = strlen(line);
        if (line[size - 1] == '\n') line[size - 1] = 0;
    }

    *var_name = line;
    // проверить переменная ли
    char * variants[] = 
    {
        "Scan value as zeckendorf;",
        "Scan value as roman."
    };
    for (int i = 0; i < 2; ++i) printf("%d. %s\n", i + 1, variants[i]);
    int res;
    printf("\nWrite down the number of function >> ");
    scanf("%d", &res);
    while (1)
    {
        if (res == 1 || res == 2) break;
        else
        {
            printf("\nTry again\n");
            while (getchar() != '\n');
        }
        printf("\nWrite down the number of function >> ");
        scanf("%d", &res);
    }
    uint32_t result;
    switch (res)
    {
        case 1:
            if ((error = insert_new_variable_zekendorf(trie, line)) != OK) 
            {
                free(line);
                return error;
            }
            break;
        case 2:
            if ((error = insert_new_variable_rim(trie, line)) != OK) 
            {
                free(line);
                return error;
            }
            break;
    }
    printf("\nDone!\n");
    return error;
}

status_code zeckendorf_to_decimal(char* zeckendorf, uint32_t * res) 
{
    if (!zeckendorf) return INVALID_PARAMETER;
    uint32_t decimal = 0;
    int length = strlen(zeckendorf);

    for (int i = 0; i < length; i++) 
    {
        if (zeckendorf[i] == '1') decimal += (uint32_t)pow(2, i);
        else if (zeckendorf[i] != '0') return INVALID_PARAMETER;
    }
    *res = decimal;
    return OK;
}

status_code insert_new_variable_zekendorf(Trie* trie, char * name)
{
    char * line = NULL;
    size_t size = 0;
    status_code error = OK;
    printf("\nWrite down the value in zeckendorf >> ");
    if (getline(&line, &size, stdin) < 0) return VAR_ERROR;
    size = strlen(line);
    if (line[size - 1] == '\n') line[size - 1] = 0;

    uint32_t res;
    while ((error = zeckendorf_to_decimal(line, &res)) != OK)
    {
        if (strlen(line) != 0)
        {
            printf("\nTry again\n");
            printf("\nWrite down the value in zeckendorf >> ");
        }
        free(line);
        line = NULL;
        size = 0;
        if (getline(&line, &size, stdin) < 0) return VAR_ERROR;
        size = strlen(line);
        if (line[size - 1] == '\n') line[size - 1] = 0;
    }

    if ((error = Trie_insert(trie, name, res)) != OK) 
    {
        return free_all(error, 1, line);
    }

    return free_all(error, 1, line);
}

status_code roman_to_decimal(char* roman, uint32_t * res) 
{
    if (strlen(roman) == 0) return INVALID_PARAMETER;
    uint32_t decimal = 0;
    int length = strlen(roman);

    for (int i = 0; i < length; i++) 
    {
        switch (roman[i]) 
        {
        case 'I':
            if (i + 1 < length && (roman[i + 1] == 'V' || roman[i + 1] == 'X')) decimal -= 1;
            else decimal += 1;
            break;
        case 'V':
            decimal += 5;
            break;
        case 'X':
            if (i + 1 < length && (roman[i + 1] == 'L' || roman[i + 1] == 'C')) decimal -= 10; 
            else decimal += 10;
            break;
        case 'L':
            decimal += 50;
            break;
        case 'C':
            if (i + 1 < length && (roman[i + 1] == 'D' || roman[i + 1] == 'M')) decimal -= 100; 
            else decimal += 100;
            break;
        case 'D':
            decimal += 500;
            break;
        case 'M':
            decimal += 1000;
            break;
        default:
            return INVALID_PARAMETER;
        }
    }
    *res = decimal;
    return OK;
}

status_code insert_new_variable_rim(Trie* trie, char * name)
{
    char * line = NULL;
    size_t size = 0;
    status_code error = OK;
    printf("\nWrite down the value in roman >> ");
    if (getline(&line, &size, stdin) < 0) return VAR_ERROR;
    size = strlen(line);
    if (line[size - 1] == '\n') line[size - 1] = 0;

    uint32_t res;
    while ((error = roman_to_decimal(line, &res)) != OK)
    {
        if (strlen(line) != 0)
        {
            printf("\nTry again\n");
            printf("\nWrite down the value in roman >> ");
        }
        free(line);
        line = NULL;
        size = 0;
        if (getline(&line, &size, stdin) < 0) return VAR_ERROR;
        size = strlen(line);
        if (line[size - 1] == '\n') line[size - 1] = 0;
    }

    if ((error = Trie_insert(trie, name, res)) != OK) 
    {
        return free_all(error, 1, line);
    }

    return free_all(error, 1, line);
}

status_code remove_variable(Trie* trie, char** variables, size_t * size)
{
    status_code error = OK;
    for (size_t i = 0; i < *size; ++i)
    {   
        Trie_node* node = NULL;
        if ((error = Trie_find(trie, variables[i], &node)) != OK) return error;
        node->is_final = 0;
        free(variables[i]);
        variables[i] = NULL;
    }
    *size = 0;
    printf("\nDone!\n");
    return OK;
}

status_code add_to_names_array(char *** names, size_t * size, size_t * capacity, char * name)
{
    if ((*size) + 1 == *capacity)
    {
        *capacity *= 2;
        char ** tmp = (char**)realloc(*names, (*capacity) * sizeof(char*));
        if (!tmp) return ALLOC_ERROR;
        *names = tmp;
    }
    (*names)[*size] = name;
    (*size)++;
    return OK;
}

status_code debugger(Trie* trie, int * work_flag)
{
    char * new_var = NULL;
    *work_flag = 1;
    // TODO DIALOG
    status_code error = OK;
    printf("\tDEBUGGER\n");
    int count = 7;
    char * functions[] = {
        "Print variable, its value and dump;",
        "Print all initialized variables;",
        "Change variable value;",
        "Insert new variable;",
        "Delete variables inserted while debugging;",
        "Stop debugging;",
        "Stop interpreter."
    };

    char * write_down = "Write down the number of function >> ";
    char * wrong = "Something went wrong!";
    uint32_t number;
    int function = -1;

    size_t size = 0;
    size_t capacity = 1;
    char ** new_var_names = (char**)calloc(1, sizeof(char*));
    if (!new_var_names) return ALLOC_ERROR;

    int flag = 1;

    while (flag)
    {
        for (int i = 0; i < count; ++i) printf("%d. %s\n", i + 1, functions[i]);
        printf("\n%s", write_down);
        scanf("%d", &number);
        switch (number)
        {
            case 1:
                if ((error = print_variable_and_value(trie)) != OK)
                {
                    if (new_var_names)
                    {
                        for (int i = 0; i < size; ++i)
                        {
                            if (new_var_names[i])
                            {
                                free(new_var_names[i]);
                                new_var_names[i] = NULL;
                            }
                        }
                        free(new_var_names);
                        new_var_names = NULL;
                    }
                    return error;
                }
                break;
            case 2:
                if ((error = print_variables(trie)) != OK)
                {
                    if (new_var_names)
                    {
                        for (int i = 0; i < size; ++i)
                        {
                            if (new_var_names[i])
                            {
                                free(new_var_names[i]);
                                new_var_names[i] = NULL;
                            }
                        }
                        free(new_var_names);
                        new_var_names = NULL;
                    }
                    return error;
                }
                break;
            case 3:
                if ((error = change_variable_value(trie)) != OK)
                {
                    if (new_var_names)
                    {
                        for (int i = 0; i < size; ++i)
                        {
                            if (new_var_names[i])
                            {
                                free(new_var_names[i]);
                                new_var_names[i] = NULL;
                            }
                        }
                        free(new_var_names);
                        new_var_names = NULL;
                    }
                    return error;
                }
                break;
            case 4:
                if ((error = insert_new_variable(trie, &new_var)) != OK)
                {
                    if (new_var_names)
                    {
                        for (int i = 0; i < size; ++i)
                        {
                            if (new_var_names[i])
                            {
                                free(new_var_names[i]);
                                new_var_names[i] = NULL;
                            }
                        }
                        free(new_var_names);
                        new_var_names = NULL;
                    }
                    return error;
                }
                if ((error = add_to_names_array(&new_var_names, &size, &capacity, new_var)) != OK)
                {
                    if (new_var_names)
                    {
                        for (int i = 0; i < size; ++i)
                        {
                            if (new_var_names[i])
                            {
                                free(new_var_names[i]);
                                new_var_names[i] = NULL;
                            }
                        }
                        free(new_var_names);
                        new_var_names = NULL;
                    }
                    return error;
                }
                break;
            case 5:
                if ((error = remove_variable(trie, new_var_names, &size)) != OK)
                {
                    if (new_var_names)
                    {
                        for (int i = 0; i < size; ++i)
                        {
                            if (new_var_names[i])
                            {
                                free(new_var_names[i]);
                                new_var_names[i] = NULL;
                            }
                        }
                        free(new_var_names);
                        new_var_names = NULL;
                    }
                    return error;
                }
                break;
            case 6:
                flag = 0;
                break;
            case 7:
                flag = 0;
                *work_flag = 0;
                break;
            default:
                printf("\n%s\n\n", wrong);
                while (getchar() != '\n');
                break;
        }
    }
    if (new_var_names)
    {
        for (int i = 0; i < size; ++i)
        {
            if (new_var_names)
            {
                free(new_var_names[i]);
                new_var_names[i] = NULL;
            }
        }
        free(new_var_names);
        new_var_names = NULL;
    }
    return error;
}