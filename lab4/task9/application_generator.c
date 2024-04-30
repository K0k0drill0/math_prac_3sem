#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "errors.h"
#include "data_structs.h"
#include "readers.h"

int rand_comp(const void* lhs, const void* rhs)
{
    return rand() & 1;
}

int main(int argc, char** argv)
{
    if (argc == 1) {
        printf("Usage: ./application_generator <max priority> <config path> <output>\n");
        return ok;
    }
    if (argc != 4) {
        print_error(stdout, INVALID_AMOUNT_OF_ARGUMENTS);
        return 1;
    }
    
    srand(time(NULL));
    
    int st = ok;
    ull max_prior = 0;
    char* start_time = NULL;
    char* finish_time = NULL;
    ull start_time_int = 0;
    ull finish_time_int = 0;
    unsigned int deps_amount = 0;
    char** deps_names = NULL;

    FILE* config_file = fopen(argv[2], "r");
    if (config_file == NULL) {
        print_error(stdout, UNABLE_TO_OPEN_FILE);
        return -1;
    }

    st = read_config_for_application_generator(config_file, &start_time, &finish_time, &deps_amount, &deps_names);
    if (st != ok) {
        free_all(2, start_time, finish_time);
        print_error(stdout, INVALID_CONFIG_FILE);
        return 1;
    }
    
    unsigned int req_cnt = rand() % (1 << 10);
    unsigned* req_ids = (unsigned*) malloc(req_cnt * sizeof(unsigned));
    if (req_ids == NULL) {
        return MEMORY_ISSUES;
    }
    
    for (unsigned int i = 0; i < req_cnt; ++i) {
        req_ids[i] = i + 1;
    }
    qsort(req_ids, req_cnt, sizeof(unsigned), rand_comp);
    
    st = st ? st : parse_ullong(argv[1], 10, &max_prior);                       
    
    st = st ? st : iso_time_convert_to_int(start_time, &start_time_int);
    st = st ? st : iso_time_convert_to_int(finish_time, &finish_time_int);
    
    FILE* file = fopen(argv[3], "w");
    if (file == NULL) {
        st = st ? st : INVALID_FILE;
    }
    
    for (unsigned int i = 0; !st && i < req_cnt; ++i) {
        char rand_st_time[21];
        unsigned rand_prior = max_prior == (unsigned) -1 ? rand_32() : (rand_32() % max_prior);
        char* rand_dep_name = deps_names[rand() % deps_amount];
        char* rand_message = NULL;
        
        ull rand_start_time_int = start_time_int + rand() % (finish_time_int - start_time_int + 1);
        st = st ? st : iso_time_convert_to_str(rand_start_time_int, 3, rand_st_time);
        st = st ? st : generate_random_str(&rand_message, "a-zA-Z0-9 ", -1);
        
        if (!st)
        {
            fprintf(file, "%s %09u %u %s \"%s\"\n", rand_st_time, req_ids[i], rand_prior, rand_dep_name, rand_message);
        }
        
        free(rand_message);
    }
    
    for (unsigned int i = 0; i < deps_amount; ++i) {
        free(deps_names[i]);
    }
    free_all(2, req_ids, deps_names);
    fclose(file);
    
    return st;
}