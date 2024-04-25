#include <stdlib.h>
#include <string.h>

#include "logger.h"
#include "errors.h"
#include "data_structs.h"

int Message_init(Message* msg, 
    char* time, 
    int situation_code, 
    int runtime_error_code, 
    int application_id, 
    char* dep_id,
    char* op_name, 
    unsigned int handling_time) {

    if (msg == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    switch (situation_code)
    {
    case NEW_REQUEST:
        if (dep_id == NULL || time == NULL) {
            return INVALID_FUNCTION_ARGUMENT;
        }
        break;
    case REQUEST_HANDLING_STARTED:
        if (op_name == NULL || time == NULL) {
            return INVALID_FUNCTION_ARGUMENT;
        }
        break;
    case REQUEST_HANDLING_FINISHED:
        if (op_name == NULL || time == NULL) {
            return INVALID_FUNCTION_ARGUMENT;
        }
        break;
    case DEPARTMENT_OVERLOADED:
        if (time == NULL) {
            return INVALID_FUNCTION_ARGUMENT;
        }
        // if (dep_id == NULL) {
        //     return INVALID_FUNCTION_ARGUMENT;
        // }
    case RUNTIME_ERROR:
        break;
    default:
        return INVALID_FUNCTION_ARGUMENT;
        break;
    }

    msg->time = time;
    msg->situation_code = situation_code;
    msg->runtime_error_code = runtime_error_code;
    msg->application_id = application_id;
    msg->dep_id = dep_id;
    msg->op_name = op_name;
    msg->handling_time = handling_time;

    return ok;
}

int Message_print(FILE* stream, Message msg) {
    if (stream == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    switch (msg.situation_code)
    {
        case NEW_REQUEST:
        {
            if (msg.dep_id == NULL)
            {
                return INVALID_INPUT;
            }
            
            fprintf(stream, "[%s] [NEW_REQUEST]: Request %d was received by dep. %s.\n",
                    msg.time, msg.application_id, msg.dep_id);
            
            break;
        }
        case REQUEST_HANDLING_STARTED:
        {
            if (msg.op_name == NULL)
            {
                return INVALID_INPUT;
            }
            
            fprintf(stream, "[%s] [REQUEST_HANDLING_STARTED]: Handling of request %d was started by operator %s.\n",
                    msg.time, msg.application_id, msg.op_name);
            
            break;
        }
        case REQUEST_HANDLING_FINISHED:
        {
            if (msg.op_name == NULL)
            {
                return INVALID_INPUT;
            }
            
            fprintf(stream, "[%s] [REQUEST_HANDLING_FINISHED]: Handling of request %d was finished in by operator %s in %u minutes.\n",
                    msg.time, msg.application_id, msg.op_name, msg.handling_time);
            
            break;
        }
        case DEPARTMENT_OVERLOADED:
        {
            if (msg.dep_id == NULL)
            {
                fprintf(stream, "[%s] [DEPARTMENT_OVERLOADED]: Department was overloaded after receiving request %d. ",
                        msg.time, msg.application_id);
                fprintf(stream, "No transfer can be executed.\n");
            }
            else
            {
                fprintf(stream, "[%s] [DEPARTMENT_OVERLOADED]: Department was overloaded after receiving request %d. ",
                        msg.time, msg.application_id);
                fprintf(stream, "Its request was transfered to department %s\n",
                        msg.dep_id);
            }
            break;
        }
        case RUNTIME_ERROR:
        {
            if (msg.time == NULL) {
                print_runtime_error(stream, INVALID_APPLICATION);
            }
            else {
                fprintf(stream, "[%s] [RUNTIME_ERROR]: ", msg.time);
                if (print_runtime_error(stream, msg.runtime_error_code) != ok) {
                    fprintf(stream, "Something unexpected happaned.\n");
                }
            }
             
            break;
        }

    }
    
    return ok;
}

int Message_set_null(Message* msg) {
    if (msg == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    msg->time = NULL;
    msg->dep_id = NULL;
    msg->op_name = NULL;

    msg->situation_code = 0;
    msg->runtime_error_code = 0;
    msg->application_id = 0;
    msg->handling_time = 0;

    return ok;
}

int Message_free(Message* msg) {
    if (msg == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    free(msg->time);
    free(msg->dep_id);
    free(msg->op_name);

    Message_set_null(msg);

    return ok;
}

int Message_queue_init(Message_queue* q) {
    if (q == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    q->size = 0;
    q->max_size = 0;
    q->data = (Message*)malloc(sizeof(Message) * 16);
    if (q->data == NULL)  {
        return MEMORY_ISSUES;
    }

    q->max_size = 16;

    return ok;
}

int Message_arr_shift(Message* arr, unsigned int size, int ind) {
    if (arr == NULL || ind < 0) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    for (int i = size; i > ind; i--) {
        arr[i] = arr[i-1];
    }

    return ok;
}

int Message_queue_insert(Message_queue* q, Message msg) {
    if (q == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    if (q->size == 0) {
        q->data[0] = msg;
        q->size = 1;
        return ok;
    }

    if (q->size == q->max_size) {
        Message* tmp = (Message*)realloc(q->data, q->max_size * 2);
        if (tmp == NULL) {
            return MEMORY_ISSUES;
        }

        q->max_size *= 2;
    }

    int new_ind = 0;

    Message hepler = q->data[new_ind];
    
    while (new_ind < q->size && strcmp(q->data[new_ind].time, msg.time) <= 0) {
        new_ind++;
        hepler = q->data[new_ind];
    }

    if (new_ind != q->size) {
        Message_arr_shift(q->data, q->size, new_ind);
        q->data[new_ind] = msg;
        // memmove(&(q->data[new_ind]), &(q->data[new_ind]),
        //  (q->size - new_ind) * sizeof(Message));
    }

    q->data[new_ind] = msg;
    q->size++;

    return ok;
}

int Message_queue_empty(Message_queue* q) {
    if (q == NULL || q->data == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    for (int i = 0; i < q->size; i++) {
        Message_free(&(q->data[i]));
    }

    q->size = 0;

    return ok;
}

int Message_queue_print(FILE* stream, Message_queue q) {
    if (q.data == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    for (int i = 0; i < q.size; i++) {
        Message_print(stream, q.data[i]);
        fprintf(stream, "\n");
    }

    return ok;
}

int Message_queue_free(Message_queue* q) {
    if (q == NULL || q->data == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    Message_queue_empty(q);
    free(q->data);

    return ok;
}

int Logger_init(Logger* log, FILE* stream) {
    if (log == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    int st = ok;

    st = Message_queue_init(&(log->queue));
    if (st != ok) {
        return st;
    }

    log->log_file = stream;

    return ok;
}

int Logger_add_message(Logger* log, Message msg) {
    if (log == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    int st = ok;

    st = Message_queue_insert(&(log->queue), msg);
    if (st != ok) {
        //free??
        return st;
    }
}

int Logger_log(Logger* log) {
    if (log == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    int st = ok;

    st = Message_queue_print(log->log_file, log->queue);
    if (st != ok) {
        return st;
    }

    st = Message_queue_empty(&(log->queue));
    if (st != ok) {
        return st;
    }

    return ok;
}

int Logger_free(Logger* log) {
    if (log == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    int st = ok;

    st = Message_queue_free(&(log->queue));
    if (st != ok) {
        return st;
    }

    fclose(log->log_file);

    return ok;
}

int Logger_add_and_init_message(Logger* log, 
    char* time, 
    int situation_code, 
    int runtime_error_code, 
    int application_id, 
    char* dep_id,
    char* op_name, 
    unsigned int handling_time) {

    char* time_cpy = NULL; 
    char* dep_id_cpy = NULL;
    char* op_name_cpy = NULL;

    int st = ok;

    if (time != NULL) {
        st = st ? st : strcpy_with_malloc(&time_cpy, time);
    }

    if (dep_id != NULL) {
        st = st ? st : strcpy_with_malloc(&dep_id_cpy, dep_id);
    }

    if (op_name != NULL) {
        st = st ? st : strcpy_with_malloc (&op_name_cpy, op_name);
    }

    if (st != ok) {
        free_all(3, time_cpy, dep_id_cpy, op_name_cpy);
        return st;
    }

    Message msg;
    st = Message_init(&msg, time_cpy, situation_code, runtime_error_code,
     application_id, dep_id_cpy, op_name_cpy, handling_time);

    if (st != ok) {
        free_all(3, time_cpy, dep_id_cpy, op_name_cpy);
        return st;
    }    

    st = Message_queue_insert(&(log->queue), msg);
    if (st != ok) {
        free_all(3, time_cpy, dep_id_cpy, op_name_cpy);
        return st;
    }

    return ok;
}
