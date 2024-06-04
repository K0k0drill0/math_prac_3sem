#include "settings_work.h"


Current_settings* create_current_settings(int debug)
{
    operation_type start_basic_types[] = {unary, unary, unary, binary, binary, binary, binary, binary, binary, binary, binary, binary, binary};
    operation_syntax start_operation_result_type = left;

    Current_settings* settings = (Current_settings*)malloc(sizeof(Current_settings));
    if (!settings) return NULL;

    //массив имен типов
    settings->operations_names = (char**)(calloc(OPERATIONS_COUNT, sizeof(char*)));
    if (!settings->operations_names)
    {
        free(settings);
        return NULL;
    }

    settings->operation_syntax_type = start_operation_result_type;

    //выделение памяти под массив unary/binary, names
    settings->basic_types = (operation_type *)calloc(OPERATIONS_COUNT, sizeof(operation_syntax));
    if (!settings->basic_types)
    {
        free(settings->operations_names);
        free(settings);
        return NULL;
    }
    for (int i = 0; i < OPERATIONS_COUNT; ++i)
    {
        char * tmp = (char*)calloc(strlen(start_operations_names[i]) + 1, sizeof(char));
        if (!tmp)
        {
            for (int j = 0; j < i; ++j) free(settings->operations_names[j]);
            free(settings->operations_names);
            free(settings);
            return NULL;
        }
        strcpy(tmp, start_operations_names[i]);
        settings->operations_names[i] = tmp;
        settings->basic_types[i] = start_basic_types[i];
    }
    //
    settings->debug = debug;
    return settings;
}

void free_current_settings(Current_settings* settings)
{
    for (int i = 0; i < OPERATIONS_COUNT; ++i)
    {
        if (settings->operations_names[i]) free(settings->operations_names[i]);
    }
    free(settings->operations_names);
    free(settings->basic_types);
    free(settings);
    settings = NULL;
}

status_code change_name(Current_settings* settings, operation operation_id, char * new_name)
{
    int ok = 1;
    for(int i = 0; i < OPERATIONS_COUNT; ++i)
    {
        if(!strcmp(settings->operations_names[i], new_name) && i != operation_id)
            return SETTINGS_ERROR;
    }
    char * tmp = (char*)calloc(strlen(new_name) + 1, sizeof(char));
    if(!tmp)
        return ALLOC_ERROR;
    strcpy(tmp, new_name);
    if (settings->operations_names[operation_id]) 
        free(settings->operations_names[operation_id]);
    settings->operations_names[operation_id] = tmp;
    return OK;
}

void change_operation_result_type(Current_settings* settings, operation_syntax new_res_type)
{
    settings->operation_result_type = new_res_type;
}

void change_operation_syntax_type(Current_settings* settings, operation_syntax new_syn_type)
{
    if(new_syn_type != middle)
        settings->operation_syntax_type = new_syn_type;
    //else nothing
}


status_code read_file_with_settings(FILE *file, Current_settings* settings)
{
    if(!file) return SETTINGS_ERROR;

    char *line = NULL;
    size_t size = 0;
    int is_reading_comment = 0;
    status_code error = OK;
    operation operation_name;
    int operation_changing = 0;

    while(getline(&line, &size, file) > 0)
    {
        if(line == NULL)
            break;
        size = strlen(line);
        if (is_reading_comment == 1)
        {
            if(line[size - 1] == ']')
                is_reading_comment = 0;
            continue;
        }
        if(line[size - 1] == '[')
        {
            is_reading_comment = 1;
            continue;
        }
        
        for(int i = 0; i < size; i++)
        {
            if(line[i] == '#')
            {
                line[i] = '\0';
                size = i + 1;
            }
        }
        


        char * token = NULL;
        //если 
        if ((error = my_strtok(&token, &line, " \n\t\r")))
        {
            free(line);
            return error;
        }

        while (token)
        {
            //if cringe.....
            size_t token_size = strlen(token);


            if (operation_changing)
            {
                error = change_name(settings, operation_name, token);
                if (error != OK)
                {
                    free(line);
                    free(token);
                    return error;
                }
                operation_changing = 0;
                free(token);
                token = NULL;
                if ((error = my_strtok(&token, &line, " \n\t\r")))
                {
                    free(line);
                    return error;
                }
                continue;
            }


            //op
            if (!strcmp(token, "(op)"))
            {
                settings->operation_syntax_type = middle;
            }
            else if (!strcmp(token, "op()"))
            {
                settings->operation_syntax_type = left;
            }
            else if (!strcmp(token, "()op"))
            {
                settings->operation_syntax_type = right;
            }
            else if (!strcmp(token, "right="))
            {
                settings->operation_result_type = right;
            }
            else if (!strcmp(token, "left="))
            {
                settings->operation_result_type = left;
            }
            else if (is_operation(token, &operation_name) == OK) 
                operation_changing = 1;
            else
            {
                free(token);
                free(line);
                return SETTINGS_ERROR;
            }

            free(token);

            if ((error = my_strtok(&token, &line, " \n\t\r")))
            {
                free(line);
                return error;
            }
        }
        if (token) free(token);
        free(line);
        line = NULL;
        size = 0;
        
    }
    free(line);
    if (operation_changing) 
        return SETTINGS_ERROR;
    if(is_reading_comment)
        return SETTINGS_ERROR;
    return OK;
}

status_code write_new_settings(Current_settings* settings)
{
    FILE * config_file = fopen(CONFIG_FILE, "w");
    if (!config_file) return FILE_ERROR;

    if (settings->operation_result_type == left) fprintf(config_file, "left=\n");
    else fprintf(config_file, "right=\n");

    if(settings->operation_syntax_type == right) fprintf(config_file, "()op\n");
    else if(settings->operation_syntax_type == middle) fprintf(config_file, "(op)\n");
    else fprintf(config_file, "op()\n");

    for (int i = 0; i < OPERATIONS_COUNT; ++i)
    {
        fprintf(config_file, "%s %s\n", start_operations_names[i], settings->operations_names[i]); 
    }
    fclose(config_file);
    return OK;
}