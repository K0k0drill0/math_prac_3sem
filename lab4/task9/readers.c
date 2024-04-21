#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <errno.h>

#include "readers.h"
#include "validators.h"
#include "errors.h"


int is_separator(char c) {
    return (c == ' ' || c == '\n' || c == '\t');
}

int cut_line(char* str) {
    if (str == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    int len = strlen(str);
    if (len == 0) {
        return ok;
    }

    int ind = len-1;
    while ((is_separator(str[ind]) || str[ind] == '\0') && ind != 0) {
        ind--;
    }

    if (ind == 0) {
        str[ind] = '\0';
        return ok;
    }
    
    str[ind+1] = '\0';
    return ok;
}

int remove_quotes(char* str) {
    if (str == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    int ind = 0;
    while (str[ind+1] != '\"' ) {
        str[ind] = str[ind+1];
        ind++;
    }
    str[ind] = '\0';

    return ok;
}

int read_line(FILE* inp, char** line) {
    int arr_max_size = 16;
    *line = (char*)malloc(sizeof(char) * arr_max_size);
    if (*line == NULL) {
        return MEMORY_ISSUES;
    }

    char c = getc(inp);
    while (is_separator(c)) {
        c = getc(inp);
    }

    int ind = 0;
    while (c != '\n' && c != EOF) {
        if (ind >= arr_max_size - 3) {
            arr_max_size *= 2;
            char *tmp = (char*)realloc(*line, arr_max_size * sizeof(char));
            if (tmp == NULL) {
                free(line);
                return MEMORY_ISSUES;
            }
            *line = tmp;
        }
        (*line)[ind] = c;
        ind++;
        c = getc(inp);
    }
    (*line)[ind] = '\0';

    if (c == EOF && ind == 0) {
        return EOF;
    }

    return ok;
}

int read_word_from_file(FILE* inp, char** str_inp) {
    int arr_max_size = 16;
    *str_inp = (char*)malloc(sizeof(char) * arr_max_size);
    if (*str_inp == NULL) {
        return MEMORY_ISSUES;
    }

    char c;
    c = getc(inp);
    int ind = 0;
    while (is_separator(c) && c != EOF) {
        c = getc(inp);
    }

    if (c == EOF) {
        return EOF;
    }

    while (!is_separator(c) && c != EOF) {
        if (ind >= arr_max_size - 1) {
            arr_max_size *= 2;
            char *tmp = (char*)realloc(*str_inp, arr_max_size * sizeof(char));
            if (tmp == NULL) {
                return MEMORY_ISSUES;
            }
            *str_inp = tmp;
        }
        (*str_inp)[ind] = c;
        ind++;
        c = getc(inp);
    }
    
    (*str_inp)[ind] = '\0';
    return ok;
}

int separate_words(char* line, const unsigned int amount, ...) {
    va_list args;
    va_start(args, amount);

    int ind = 0;
    for (int i = 0; i < amount; i++) {
        int str_max_size = 16;
        int str_tmp_size = 0;

        char** str_tmp = va_arg(args, char**);
        *str_tmp = (char*)malloc(sizeof(char) * str_max_size);
        if (*str_tmp == NULL) {
            return MEMORY_ISSUES;
        } 
        
        while (line[ind] == ' ' || line[ind] == '\t') {
            ind++;
        } 
        if (line[ind] == '\0') {
            free(*str_tmp);
            *str_tmp = NULL;
            return INVALID_APPLICATION;
        }

        if (i != 4) {
            while (line[ind] != ' ' && line[ind] != '\t' && line[ind] != '\0') {
                if (str_tmp_size + 1 == str_max_size) {
                    str_max_size *= 2;
                    char *tmp = (char*)realloc(*str_tmp, sizeof(char) * str_max_size);
                    if (tmp == NULL) {
                        //free(str_tmp);
                        return MEMORY_ISSUES;
                    }
                    *str_tmp = tmp;
                }
                (*str_tmp)[str_tmp_size] = line[ind];
                ind++;
                str_tmp_size++;
            }
            (*str_tmp)[str_tmp_size] = '\0';
        }
        else {
            while (line[ind] != '\0') {
                if (str_tmp_size + 1 == str_max_size) {
                    str_max_size *= 2;
                    char *tmp = (char*)realloc(*str_tmp, sizeof(char) * str_max_size);
                    if (tmp == NULL) {
                        //free(str_tmp);
                        return MEMORY_ISSUES;
                    }
                    *str_tmp = tmp;
                }
                (*str_tmp)[str_tmp_size] = line[ind];
                ind++;
                str_tmp_size++;
            }
            (*str_tmp)[str_tmp_size] = '\0';
            cut_line(*str_tmp);
        }

        
    }

    va_end(args);
    return ok;
}

int read_application_data(
    FILE* stream,
    Unvalidated_application* a) {
        
    if (stream == NULL || a == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    int st = ok;

    a->arrival_time = NULL;
    a->dep_id = NULL;
    a->id = NULL;
    a->priority = NULL;
    a->text = NULL;

    char* line = NULL;
    st = read_line(stream, &line);
    if (st != ok) {
        free(line);
        return st;
    }

    st = separate_words(line, 5, &(a->arrival_time), &(a->id),
     &(a->priority), &(a->dep_id), &(a->text));
    //st = separate_words(line, 5, time, message_id, priority, dep_id, text);
    free(line);
    if (st != ok && st != INVALID_APPLICATION) {
        Unvalidated_application_free(a);
        return st;
    } 

    return ok;
}

int read_config_file(
    FILE* stream, 
    char** pq_type,
    char** map_type, 
    char** start_time,
    char** finish_time, 
    char** min_handling_time, 
    char** max_handling_time,
    char** dep_amount, 
    char** overload_coeff,
    str_pair_arr* arr
    ) {

    if (stream == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    *pq_type = NULL;
    *map_type = NULL;
    *start_time = NULL;
    *finish_time = NULL;
    *min_handling_time = NULL;
    *max_handling_time = NULL;
    *dep_amount = NULL;
    *overload_coeff = NULL;

    int st = ok;

    st = st ? st : read_word_from_file(stream, pq_type); 
    st = st ? st : read_word_from_file(stream, map_type);
    st = st ? st : read_word_from_file(stream, start_time);
    st = st ? st : read_word_from_file(stream, finish_time);
    st = st ? st : read_word_from_file(stream, min_handling_time);
    st = st ? st : read_word_from_file(stream, max_handling_time);
    st = st ? st : read_word_from_file(stream, dep_amount);

    if (st != ok) {
        free_all(8, *pq_type, *map_type, *start_time, *finish_time,
         *min_handling_time, *max_handling_time, *dep_amount, *overload_coeff);
        
        str_pair_arr_free(arr);
        return (st == EOF) ? INVALID_CONFIG_FILE : st;
    }

    if (!is_valid_uint(*dep_amount)) {
        free_all(8, *pq_type, *map_type, *start_time, *finish_time,
         *min_handling_time, *max_handling_time, *dep_amount, *overload_coeff);
        
        str_pair_arr_free(arr);
        return INVALID_CONFIG_FILE;
    }

    int idep_amount = atoi(*dep_amount);

    if (errno == ERANGE || idep_amount < 1 || idep_amount > 100) {
        free_all(8, *pq_type, *map_type, *start_time, *finish_time,
         *min_handling_time, *max_handling_time, *dep_amount, *overload_coeff);
        
        str_pair_arr_free(arr);
        return INVALID_CONFIG_FILE;
    }

    for (int i = 0; i < idep_amount; i++) {
        char* name = NULL, *op_am = NULL;

        st = st ? st : read_word_from_file(stream, &name);
        st = st ? st : read_word_from_file(stream, &op_am);

        if (st != ok) {
            free_all(10, *pq_type, *map_type, *start_time,
            *finish_time, *min_handling_time, *max_handling_time,
            *dep_amount, *overload_coeff, name, op_am);

            str_pair_arr_free(arr);
            return (st == EOF) ? INVALID_CONFIG_FILE : st;
        }

        str_pair p = {name, op_am};

        st = str_pair_arr_push_back(arr, p);
        if (st != ok) {
            free_all(10, *pq_type, *map_type, *start_time,
            *finish_time, *min_handling_time, *max_handling_time,
            *dep_amount, *overload_coeff, name, op_am);

            str_pair_arr_free(arr);
            return st;
        }
    }

    st = read_word_from_file(stream, overload_coeff);
    if (st != ok) {
        free_all(8, *pq_type, *map_type, *start_time, *finish_time,
         *min_handling_time, *max_handling_time, *dep_amount, *overload_coeff);
        
        str_pair_arr_free(arr);
        return st;
    }

    return ok;
}
