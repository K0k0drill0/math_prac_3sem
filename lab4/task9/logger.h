#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

enum SITUATION_CODE {
    NEW_REQUEST,
    REQUEST_HANDLING_STARTED,
    REQUEST_HANDLING_FINISHED,
    DEPARTMENT_OVERLOADED,

    RUNTIME_ERROR
};

typedef struct Message {
    char* time;

    int situation_code;
    int runtime_error_code;

    int application_id;
    char* dep_id;

    char* op_name;
    unsigned int handling_time;
} Message;

typedef struct Message_queue {
    Message* data;

    unsigned int size;
    unsigned int max_size;
} Message_queue;

typedef struct Logger {
    Message_queue queue;
    FILE* log_file;
} Logger;

int Logger_init(Logger* log, FILE* stream);
int Logger_add_message(Logger* log, Message msg);
int Logger_log(Logger* log);
int Logger_free(Logger* log);

int Logger_add_and_init_message(Logger* log, 
    char* time, 
    int situation_code, 
    int runtime_error_code, 
    int application_id, 
    char* dep_id,
    char* op_name, 
    unsigned int handling_time);

int Message_init(Message* msg, 
    char* time, 
    int situation_code, 
    int runtime_error_code, 
    int application_id, 
    char* dep_id,
    char* op_name, 
    unsigned int handling_time);

#endif