#include "interpreter.h"

#include "interpreter.h"

status_code add_to_expression(char ** expression, char * to_add)
{
    size_t old_size = strlen(*expression);
    size_t new_size = old_size + strlen(to_add);
    char * tmp = realloc(*expression, (new_size + 2) * sizeof(char));
    if (!tmp)
    {
        free(expression);
        expression = NULL;
        return ALLOC_ERROR;
    }
    *expression = tmp;
    if (old_size != 0)
    {
        (*expression)[old_size] = ',';
        old_size++;
    }
    strcpy(*expression + old_size, to_add);
    return OK;
}

status_code make_unary_expression(Trie* trie, char * variable_name, operation operation_name, uint32_t value, uint32_t * result, int input_base, int output_base, int assign_base)
{
    switch (operation_name)
    {
        case NOT:
            return not_operation(trie, value, result);
        case INPUT:
            return input_operation(trie, input_base, result, variable_name);
        case OUTPUT:
            return output_operation(trie, value, output_base);
        default:
            break;
    }
    return BAD;
}

status_code remove_brackets(char** str, int debug, int* debug_activated)
{
    *debug_activated =0;
    if(*str == NULL)
        return INVALID_PARAMETER;
    int j = 0;
    int is_commenting = 0;
    char* BR = "#BREAKPOINT\n";
    int br_pointer = 0;
    for(int i = 0; i < strlen(*str); i++)
    {

        if(is_commenting > 0)
        {
            if((*str)[i] == ']')
                is_commenting--;
        }
        else
        {
            if((*str)[i] == '[')
                is_commenting++;
            else
            {
                (*str)[j] = (*str)[i];
                j++;
            }
        }
    }
    (*str)[j] = '\0';
    if(is_commenting > 0)
    {
        return LEXEME_ERROR;
    }
    int k = 0;
    is_commenting = 0;
    for(int i = 0; i < j; i++)
    {
        if(BR[br_pointer] == (*str)[i])
            br_pointer++;
        else
            br_pointer = 0;
        if((*str)[i] == '#')
        {
            is_commenting = 1;
        }
        if(!is_commenting)
            (*str)[k++] = (*str)[i];
        if((*str)[i] == '\n')
        {
            is_commenting = 0;
            if(br_pointer == strlen(BR))
            {
                br_pointer = 0;
                if(debug)
                    *debug_activated = 1;
            }
        }    
    }
    (*str)[k] = '\0';
    return OK;
}

status_code make_binary_expression(Trie* trie, operation operation_name, uint32_t value_1, uint32_t value_2, uint32_t * result)
{
    switch (operation_name)
    {
        case ADD:
            return add_operation(trie, value_1, value_2, result);
        case MULT:
            return mult_operation(trie, value_1, value_2, result);
        case SUB:
            return sub_operation(trie, value_1, value_2, result);
        case POW:
            return pow_operation(trie, value_1, value_2, result);
        case DIV:
            return div_operation(trie, value_1, value_2, result);
        case REM:
            return rem_operation(trie, value_1, value_2, result);
        case XOR:
            return xor_operation(trie, value_1, value_2, result);
        case AND:
            return and_operation(trie, value_1, value_2, result);
        case OR:
            return or_operation(trie, value_1, value_2, result);
        default:
            break;
    }
    return BAD;
}

status_code is_delim(char character, const char * delim)
{
    int i = 0;
    while (delim[i])
    {
        if (delim[i] == character) return OK;
        ++i;
    }
    return BAD;
}

void shift_string(char ** string, int index)
{
    int i = 0;
    int start_index = index;
    while ((*string)[index])
    {
        (*string)[i] = (*string)[index];
        ++i;
        ++index;
    }
    for (int j = i; j < index; ++j) (*string)[j] = 0;
}

status_code my_strtok(char ** result, char ** st_string, const char * delim)
{
    int is_empty_string = 1;
    char * string = *st_string;
    *result = NULL;
    if (!string[0]) return OK;
    char * new_string = (char*)calloc((strlen(string) + 1), sizeof(char));
    if (!new_string) return ALLOC_ERROR;
    
    int i = 0;

    while (string[i] && is_delim(string[i], delim) != OK)
    {
        if (string[i] != ' ' && string[i] != '\n' && string[i] != '\t') is_empty_string = 0;
        new_string[i] = string[i];
        ++i;
    }
    if (i == 0 && is_delim(string[i], delim) == OK) 
    {
        shift_string(st_string, i + 1);
        free(new_string);
        return my_strtok(result, st_string, delim);
    }
    if (is_empty_string)
    {
        free(new_string);
        return OK;
    }
    if (strcmp(delim, ";") == 0 && is_delim(string[i], delim) != OK)
    {
        free(new_string);
        return LEXEME_ERROR;
    }
    if (is_delim(string[i], delim) == OK) ++i;

    shift_string(st_string, i);
    if (strcmp(delim, ";") != 0) free_from_delims(&new_string);
    *result = new_string;
    return OK;
}

status_code my_assign_strtok(char ** result, char ** st_string, const char * delim)
{
    int is_empty_string = 1;
    char * string = *st_string;
    *result = NULL;
    if (!string[0]) return OK;
    char * new_string = (char*)calloc((strlen(string) + 1), sizeof(char));
    if (!new_string) return ALLOC_ERROR;
    int i = 0;

    char * ptr = strstr(string, delim);
    if (!ptr)
    {
        strcpy(new_string, *st_string);
        shift_string(st_string, strlen(string));
        free_from_delims(&new_string);
        *result = new_string;
        return OK;
    }
    while (string[i] != *ptr)
    {
        if (string[i] != ' ' && string[i] != '\n' && string[i] != '\t') is_empty_string = 0;
        new_string[i] = string[i];
        ++i;
    }
    if (string[0] == *ptr)
    {
        shift_string(st_string, i + 1);
        free(new_string);
        return my_assign_strtok(result, st_string, delim);
    }
    if (is_empty_string)
    {
        free(new_string);
        return OK;
    }
    shift_string(st_string, i + strlen(delim));
    
    free_from_delims(&new_string);
    *result = new_string;
    return OK;
}

status_code prefix_to_postfix(Current_settings* settings, char** input, char*** output, int count)
{
    int output_count = 0;
    int st_last = -1;
    char** st = (char**) malloc(sizeof(char*) * count);
    if(!st)
        return ALLOC_ERROR;
    *output = (char**) malloc(sizeof(char*) * count);
    if(!output)
        return free_all(ALLOC_ERROR, 1, st);
    for(int i = count - 1; i >= 0; i--)
    {
        int is_op = 0;
        int prior = 0;
        operation op_name = 0;
        if(is_new_operation(settings, input[i], &op_name) == OK)
        {
            prior = operation_prior[op_name];
            is_op = 1;
        }

        if(!is_op)
        {
            st[++st_last] = input[i];
        }
        else
        {
            if(settings->basic_types[op_name] == unary)
            {
                if(st_last == -1)
                {
                    (*output)[output_count++] = input[i];
                }
                else
                {
                    char* first_op = st[st_last--];
                    (*output)[output_count++] = first_op;
                    (*output)[output_count++] = input[i];
                }
            }
            else
            {
                if(st_last != -1)
                {
                char* first_op = st[st_last--];
                (*output)[output_count++] = first_op;
                }
                if(st_last != -1)
                {
                char* second_op = st[st_last--];
                (*output)[output_count++] = second_op;
                }
                (*output)[output_count++] = input[i];
            }
        }
    }
    while(st_last != -1)
    {
        (*output)[output_count++] = st[st_last--];
    }
    return free_all(OK, 1, st);
}

status_code infix_to_postfix(Current_settings* settings, char** input, char*** output, int count)
{
    int output_count = 0;
    int st_last = -1;
    char** st = (char**) malloc(sizeof(char*) * count);
    if(!st)
        return ALLOC_ERROR;
    *output = (char**) malloc(sizeof(char*) * count);
    if(!output)
        return free_all(ALLOC_ERROR, 1, st);

    for(int i = 0; i < count; i++)
    {
        int is_op = 0;
        int prior = 0;
        operation op_name = 0;
        if(is_new_operation(settings, input[i], &op_name) == OK)
        {
            prior = operation_prior[op_name];
            is_op = 1;
        }

        if(!is_op)
        {
            (*output)[output_count++] = input[i];
        }
        else
        {
            if(st_last == -1)
                st[++st_last] = input[i];
            else
            {
                int op_last;
                operation op_last_name = 0;
                is_new_operation(settings, st[st_last], &op_last_name);
                op_last = operation_prior[op_last_name];
                if(prior < op_last)
                {
                    st[++st_last] = input[i];
                }
                else
                {
                    while(prior >= op_last)
                    {
                        if(prior == op_last && settings->basic_types[op_name] == unary)
                            break;
                        (*output)[output_count++] = st[st_last--];
                        if(st_last == -1)
                            break;
                        op_last_name = 0;
                        is_new_operation(settings, st[st_last], &op_last_name);
                        op_last = operation_prior[op_last_name];
                    }
                    st[++st_last] = input[i];
                }
            }
        }
    }
    while(st_last != -1)
    {
        (*output)[output_count++] = st[st_last--];
    }
    return free_all(OK, 1, st);
}

status_code solve_expression_new(Current_settings* settings, Trie* trie, char* st_expr, uint32_t* result, int input_base, int output_base, int assign_base)
{
    status_code error = OK;
    int count = 0;
    int capacity = 2;
    char** buffer = (char**)malloc(sizeof(char*) * capacity);//!
    if(!buffer)
        return ALLOC_ERROR;

    char* buf = (char*)calloc(strlen(st_expr) + 1, sizeof(char));//!
    if (!buf) 
        return free_all(OK, 1, buffer);
    strcpy(buf, st_expr);

    char* line = NULL;//!
    while(my_strtok(&line, &buf, " (),") == OK)
    {
        if(line == NULL)
            break;
        buffer[count] = line;
        count++;
        if(count == capacity)
        {
            capacity *= 2;
            char** tmp = (char**)realloc(buffer, capacity * sizeof(char*));
            if(tmp == NULL)
                return free_all(ALLOC_ERROR, 3, buffer, buf, line);
            buffer = tmp;
        }
    }
    char** new_buf = (char**)malloc(sizeof(char*) * capacity);//!

    switch (settings->operation_syntax_type)
    {
    case left:
        if ((error = prefix_to_postfix(settings, buffer, &new_buf, count)) != OK)
            return free_all(error, 4, buf, buffer, line, new_buf);
        break;
    case middle:
    {
        if ((error = infix_to_postfix(settings, buffer, &new_buf, count)) != OK)
            return free_all(error, 4, buf, buffer, line, new_buf);
        break;
    }
    case right:
        for(int i = 0; i < count; ++i)
        {
            new_buf[i] = buffer[i];
        }
    default:
        break;
    }
    char** st = (char**)malloc(sizeof(char*) * count);//!
    int st_last = -1;
    for(int i = 0; i < count; i++)
    {
        char* KL = new_buf[i];
        int is_op = 0;
        operation op_name = 0;
        int var_value = 0;
        if(is_new_operation(settings, new_buf[i], &op_name) == OK)
            is_op = 1;

        if (is_op == 0 && is_variable(new_buf[i]) == OK || is_in_base(new_buf[i], assign_base) == OK)
        {
            status_code error = get_value(trie, new_buf[i], &var_value, assign_base);
            if(error)
                return free_all(error, 5, st, buf, buffer, line, new_buf);
        }
        else if(is_op == 0)
            return free_all(LEXEME_ERROR, 5, st, buf, buffer, line, new_buf);
        
        if(is_op)
        {
            if(settings->basic_types[op_name] == binary)
            {
                int val_1 = 0;
                int val_2 = 0;
                int res = 0;
                char* val1 = st[st_last--];
                char* val2 = st[st_last--];
                char* res_str = NULL;
                get_value(trie, val1, &val_1, assign_base);
                get_value(trie, val2, &val_2, assign_base);
                make_binary_expression(trie, op_name, val_1, val_2, &res);
                convert_to_base(res, assign_base, &res_str);
                st[++st_last] = res_str;
                free(val1);
                free(val2);
            }
            else
            {
                int val_1 = 0;
                int res = 0;
                char* val1 = st[st_last--];
                char* res_str = NULL;
                get_value(trie, val1, &val_1, assign_base);
                make_unary_expression(trie, val1, op_name, val_1, &res, input_base, output_base, assign_base);
                convert_to_base(res, assign_base, &res_str);
                st[++st_last] = res_str;
                free(val1);
            }
        }
        else
        {
            st[++st_last] = new_buf[i];
        }
    } 
    if(st_last != 0)
    {
        while(st_last != -1)
        {
            char* smh = st[st_last--];
            free(smh);
        }
        return free_all(error, 5, st, buf, buffer, line, new_buf);
    }
    int g = 0;
    error = get_value(trie, st[st_last], &g, assign_base);
    *result = g;
    return free_all(error, 5, st, buf, buffer, line, new_buf);
}

status_code free_from_delims(char ** string)
{
    if (!*string) return OK;
    int i = 0;
    while ((*string)[i] && ((*string)[i] == ' ' || (*string)[i] == '\n' || (*string)[i] == '\t') || (*string)[i] == ';') ++i;
    shift_string(string, i);
    i = strlen(*string) - 1;
    while (i != 0 && ((*string)[i] == ' ' || (*string)[i] == '\n' || (*string)[i] == '\t')|| (*string)[i] == ';')
    {
        (*string)[i] = 0;
        --i;
    }
    return OK;
}

status_code scan_buffer(Current_settings* settings, Trie* trie, char * st_buffer, int input_base, int output_base, int assign_base)
{
    char * buffer = NULL;
    status_code error = OK;
    char * line = NULL;
    int comment = 0;
    int i = 0;
    int breakpoint = 0;
    char * line_copy_1 = NULL;
    char * line_copy = NULL;
    char * string = NULL;
    char * copy_res = NULL;
    uint32_t value;
    char * variable_name = NULL;
    char * string_copy = NULL;
    char * expression = NULL;
    int count = 9;

    buffer = (char*)calloc(strlen(st_buffer) + 2, sizeof(char));
    if (!buffer) return ALLOC_ERROR;
    strcpy(buffer, st_buffer);
    buffer[strlen(buffer)] = ';';
    buffer[strlen(buffer)] = '\0';
    if ((error = my_strtok(&line, &buffer, ";")) != OK) return free_all(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);

    int is_commenting = 0;
    while (line)
    {
        free_from_delims(&line);
        if((error = remove_brackets(&line, settings->debug, &breakpoint)) != OK)
            return free_all(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
        if(strlen(line) != 0)
        {    
            if (breakpoint && settings->debug) 
            {
                int flag = 1;
                if ((error = debugger(trie, &flag)) != OK) return free_all(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
                if (!flag) return free_all(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
                breakpoint = 0;
            }
            line_copy = (char*)malloc((strlen(line) + 1) * sizeof(char));
            if (!line_copy)
            {
                error = ALLOC_ERROR;
                return free_all(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
            }
            strcpy(line_copy, line);
            if (
                (error = my_assign_strtok(&string, &line_copy, settings->operations_names[OPERATIONS_COUNT - 1])) != OK) return free_all(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
            string_copy = (char*)calloc(strlen(string) + 1, sizeof(char));
            if (!string_copy)
            {
                error = ALLOC_ERROR;
                return free_all(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
            }

            strcpy(string_copy, string);
            if ((error = my_strtok(&copy_res, &string_copy, "() ")) != OK) 
                return free_all(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
            if(settings->operation_syntax_type == right)
                if ((error = my_strtok(&copy_res, &string_copy, "() ")) != OK) 
                    return free_all(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
            if (copy_res != NULL &&
                (strcmp(copy_res, settings->operations_names[2]) == OK || strcmp(copy_res, settings->operations_names[1]) == OK)
                ) 
            {
                free(copy_res);
                copy_res = NULL;
                free(string_copy);
                string_copy = NULL;
                if ((error = solve_expression_new(settings, trie, string, &value, input_base, output_base, assign_base)) != OK) return free_all(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
            }
            else
        {
            free(copy_res);
            copy_res = NULL;
            if (string_copy) free(string_copy);
            string_copy = NULL;
            if (copy_res) free(copy_res);
            copy_res = NULL;
            if (settings->operation_result_type == left)
            {
                if (is_variable(string) != OK)
                {
                    error = VAR_ERROR;
                    return free_all(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
                }
                variable_name = (char*)malloc((strlen(string) + 1) * sizeof(char));
                if (!variable_name)
                {
                    error = ALLOC_ERROR;
                    return free_all(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
                }
                strcpy(variable_name, string);
                if (string) free(string);
                string = NULL;    
                if ((error = my_strtok(&string, &line_copy, settings->operations_names[OPERATIONS_COUNT - 1])) != OK) return free_all(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
                if ((error = solve_expression_new(settings, trie, string, &value, input_base, output_base, assign_base)) != OK) return free_all(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
            }
            else
            {
                expression = (char*)malloc((strlen(string) + 1) * sizeof(char));
                if (!expression)
                {
                    error = ALLOC_ERROR;
                    return free_all(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
                }
                strcpy(expression, string);
                if (string) free(string);
                string = NULL;
                if ((error = my_strtok(&string, &line_copy, settings->operations_names[OPERATIONS_COUNT - 1])) != OK) return free_all(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
                if ((error = is_variable(string)) != OK) return free_all(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
                variable_name = (char*)malloc((strlen(string) + 1) * sizeof(char));
                if (!variable_name)
                {
                    error = ALLOC_ERROR;
                    return free_all(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
                }
                strcpy(variable_name, string);
                if (string) free(string);
                string = NULL;
                if ((error = solve_expression_new(settings, trie, expression, &value, input_base, output_base, assign_base)) != OK) return free_all(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
                if (expression) free(expression);
                expression = NULL;
            }
            if ((error = Trie_insert(trie, variable_name, value)) != OK) return free_all(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
        }
        }
        //-----------------------------------------------
        if (variable_name && variable_name != string) free(variable_name);
        variable_name = NULL;
        if (string) free(string);
        string = NULL;
        if (line_copy) free(line_copy);
        line_copy = NULL;
        
        if(line[strlen(line) - 1] == ']')
            is_commenting = 0;
        if (line) free(line);
            line = NULL;

        if ((error = my_strtok(&line, &buffer, ";")) != OK) return free_all(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
    }

    return free_all(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
}

status_code read_full_file(FILE * file, char ** string)
{
    size_t size = 0, capacity = 16;
    *string = (char*)calloc(capacity, sizeof(char));
    if (!*string)
    {
        fclose(file);
        return ALLOC_ERROR;
    }
    char c = getc(file);
    while (c != EOF)
    {
        if (size + 1 == capacity)
        {
            capacity *= 2;
            char * tmp = (char*)realloc(*string, capacity * sizeof(char));
            if (!tmp)
            {
                free(*string);
                fclose(file);
                return ALLOC_ERROR;
            }
            *string = tmp;
        }
        (*string)[size] = c;
        ++size;
        c = getc(file);
    }
    (*string)[size] = 0;
    fclose(file);
    return OK;
}

status_code interpretate(FILE * file, Current_settings* settings, int input_base, int assign_base, int output_base)
{
    Trie* trie = Trie_create();
    if (!trie)
    {
        fclose(file);
        free_current_settings(settings);
        return FILE_ERROR;
    }

    char * buffer = NULL;
    status_code error;
    if ((error = read_full_file(file, &buffer)) != OK)
    {
        free_current_settings(settings);
        Trie_free(trie);
        return error;
    }

    if ((error = scan_buffer(settings, trie, buffer, input_base, output_base, assign_base)) != OK)
    {
        Trie_free(trie);
        free_current_settings(settings);
        free(buffer);
        return error;
    }

    Trie_free(trie);
    free_current_settings(settings);
    free(buffer);
    return OK;
}