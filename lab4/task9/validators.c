#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>

#include "validators.h"
#include "errors.h"

int is_leap_year(const int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int is_valid_time_ISO_8601(const char *dateString) {
    // 2024-02-22T18:09:22Z
    // YYYY-MM-DDTHH:MM:SSZ
    // 0123456789abcdefghij
    if (strlen(dateString) != 20) {
        return 0; 
    }

    for (int i = 0; i < 20; i++) {
        if (i == 4 || i == 7) {
            if (dateString[i] != '-') {
                return 0; 
            }
        }
        else if (i == 13 || i == 16) {
            if (dateString[i] != ':') {
                return 0; 
            }
        }
        else if (i == 10) {
            if (dateString[i] != 'T') {
                return 0;
            }
        }
        else if (i == 19) {
            if (dateString[i] != 'Z') {
                return 0;
            }
        }
        else {
            if (!isdigit(dateString[i])) {
                return 0;
            }
        }
    }

    int year, month, day, hour, minute, second;
    sscanf(dateString, "%d-%d-%dT%d:%d:%dZ", &year, &month, &day, &hour, &minute, &second);

    if (year < 1000 || year > 9999 || month < 1 || month > 12 || day < 1 || day > 31 ||
     hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59) {
        return 0; 
    }

    int daysInMonth[] = {31, 28 + is_leap_year(year), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (day > daysInMonth[month - 1]) {
        return 0; 
    }

    return 1; 
}

int is_valid_uint(const char* str) {
    int ind = 0;
    while (str[ind] != '\0') {
        if (!isdigit(str[ind])) {
            return 0;
        }
        ind++;
    }
    return (ind == 0) ? 0 : 1;
}


int is_valid_udouble(const char* str) {
    int dot_counter = 0;
    int ind = 0;
    while (str[ind] != '\0') {
        if (!isdigit(str[ind])) {
            if (str[ind] == '.' && dot_counter == 0) {
                dot_counter++;
            }
            else {
                return 0;
            }
        }
        ind++;
    }
    return (ind == 0) ? 0 : 1;
}


int validate_application_data(
    char *time,
    char *message_id,
    char *priority,
    char *dep_id,
    char *text,
    int* runtime_error) {
    // <время поступления заявки> <приоритет> <идентификатор отделения> “<текстзаявки>"

    //runtime_error = ok;

    if (runtime_error == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    if (time == NULL || message_id == NULL || priority == NULL || dep_id == NULL || text == NULL) {
        *runtime_error = INVALID_APPLICATION;
        return ok;
    }

    if (!is_valid_time_ISO_8601(time)) {
        *runtime_error = INVALID_TIME;
        return ok;
    }

    if (!is_valid_uint(message_id)) {
        *runtime_error = INVALID_MESSAGE_ID;
        return ok;
    }

    if (!is_valid_uint(priority)) {
        *runtime_error = INVALID_PRIORITY;
        return ok;
    }

    int l = strlen(text);
    if (l < 2 || text[0] != '"' || text[l-1] != '"') {
        *runtime_error = INVALID_TEXT;
        return ok;
    }

    return ok;
}

int which_pq_type(const char* str, Priority_queue_type* t) {
    if (str == NULL || t == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    if (!strcmp(str, "BinaryHeap")) {
        *t = PQ_BINARY;
    }
    else if (!strcmp(str, "BinomialHeap")) {
        *t = PQ_BINOMIAL;
    }
    else if (!strcmp(str, "FibonacciHeap")) {
        *t = PQ_FIBONACCI;
    }
    else if (!strcmp(str, "SkewHeap")) {
        *t = PQ_SKEW;
    }
    else if (!strcmp(str, "LeftistHeap")) {
        *t = PQ_LEFTIST;
    }
    else if (!strcmp(str, "Treap")) {
        *t = PQ_TREAP;
    }
    else {
        *t = PQ_INVALID;
    }

    return ok;
}

int which_map_type(const char* str, map_base_type* t) {
    if (str == NULL || t == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    if (!strcmp(str, "HashSet")) {
        *t = MAP_HASHSET;
    }
    else if (!strcmp(str, "DynamicArray")) {
        *t = MAP_ARR;
    }
    else if (!strcmp(str, "BinarySearchTree")) {
        *t = MAP_BST;
    }
    else if (!strcmp(str, "Trie")) {
        *t = MAP_TRIE;
    }
    else {
        *t = MAP_INVALID;
    }

    return ok;
}

int is_valid_config_file(
    char* pq_type,
    char* map_type, 
    char* start_time,
    char* finish_time, 
    char* min_handling_time, 
    char* max_handling_time,
    char* dep_amount, 
    char* overload_coeff,
    str_pair_arr arr) {

    if (pq_type == NULL ||
        map_type == NULL ||
        start_time == NULL ||
        finish_time == NULL ||
        min_handling_time == NULL ||
        max_handling_time == NULL ||
        dep_amount == NULL ||
        overload_coeff == NULL) {
            
        return INVALID_FUNCTION_ARGUMENT;
    }

    Priority_queue_type pq_t;
    map_base_type map_t;
    which_pq_type(pq_type, &pq_t);
    which_map_type(map_type, &map_t);

    if (pq_t == PQ_INVALID || map_t == MAP_INVALID ||
     !is_valid_time_ISO_8601(start_time) ||
     !is_valid_time_ISO_8601(finish_time) || 
     !is_valid_uint(min_handling_time) ||
     !is_valid_uint(min_handling_time) ||
     !is_valid_udouble(overload_coeff)) {
        return 0;
    }
    
    for (int i = 0; i < arr.size; i++) {
        if (strlen(arr.data[i].name) > 11) {
            return 0;
        }
        if (!is_valid_uint(arr.data[i].op_am)) {
            return 0;
        }
    }

    if (strcmp(start_time, finish_time) > 0) {
        return 0;
    }

    return 1;
}