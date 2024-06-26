#ifndef READERS_H
#define READERS_H

#include "data_structs.h"

int remove_quotes(char* str);

int read_line(FILE* stream, char** line);
int read_word_from_file(FILE* inp, char** str_inp);

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
    );
int read_config_for_application_generator(FILE* inp, char** start_time, 
    char** finish_time, int* deps_amount, char*** deps_names);

int read_application_data(
    FILE* stream,
    Unvalidated_application* a);

#endif