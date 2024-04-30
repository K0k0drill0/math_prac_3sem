#ifndef VALIDATORS_H
#define VALIDATORS_H

#include "data_structs.h"

#include "map/map.h"
#include "priority_queue/priority_queue.h"

int is_valid_time_ISO_8601(const char *dateString);
int is_valid_uint(const char* str);
int is_valid_udouble(const char* str);

int validate_application_data(
    char *time,
    char *message_id,
    char *priority,
    char *dep_id,
    char *text,
    int* runtime_error);

int is_valid_config_file(
    char* pq_type,
    char* map_type, 
    char* start_time,
    char* finish_time, 
    char* min_handling_time, 
    char* max_handling_time,
    char* dep_amount, 
    char* overload_coeff,
    str_pair_arr arr);

int which_pq_type(const char* str, Priority_queue_type* t);
int is_valid_pq_type(const char* str);

int which_map_type(const char* str, map_base_type* t);

#endif