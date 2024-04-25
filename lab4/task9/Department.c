#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#include "Department.h"
#include "errors.h"

void Operator_array_free(Operator** arr, unsigned int size);

int Operator_init(Operator* op, char* name) {
    if (op == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    op->name = name;

    op->a = NULL;
    op->start_time = NULL;
    op->finish_time = NULL;
    op->handling_time = 0;

    return ok;
}

void Operator_set_null(Operator* op) {
    if (op == NULL) {
        return;
    }

    op->a = NULL;
    op->finish_time = NULL;
    op->start_time = NULL;
    op->name = NULL;
    op->handling_time = 0;
}

void Operator_free(Operator* op) {
    if (op == NULL) {
        return;
    }

    free_application(op->a);
    free(op->start_time);
    free(op->finish_time);
    free(op->name);

    Operator_set_null(op);

    //return ok;
}


int Operator_give_task(Department* dep, size_t op_id, char* time, Application* req)
{
    if (dep == NULL || time == NULL || req == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    if (op_id >= dep->operators_amount) {
        return INVALID_INPUT;
    }
    
    Operator* op = &(dep->staff[op_id]);
    dep->free_operators_amount--;
    
    unsigned handling_time = 0;
    if (dep->max_handlind_time == UINT_MAX && dep->min_handling_time == 0) {
        handling_time = rand_32();
    }
    else {
        handling_time = dep->min_handling_time + rand_32() % (dep->max_handlind_time - dep->min_handling_time + 1);
    }
    
    op->a = req;
    op->handling_time = handling_time;
    
    int st = strcpy_with_malloc(&(op->start_time), time);
    if (st != ok) {
        return st;
    }
    iso_time_add(op->start_time, op->handling_time * 60, &(op->finish_time));
    
    return ok;
}


int generate_random_str(char** name) {
    //srand(time(NULL));
    int max_length = 10, min_length = 1;

    int length = rand() % (max_length - min_length + 1) + min_length; 

    char * file_name = (char*)malloc((length + 1) * sizeof(char));
    if (file_name == NULL) {
        return MEMORY_ISSUES;
    }
    file_name[length] = '\0';
    for (int i = 0; i < length;) {
        file_name[i] = rand() % ('Z' - '0' + 1) + '0';
        if (isalpha(file_name[i]) || isdigit(file_name[i])) {
            i++;
        } 
    }

    // file_name[length + 1] = '.';
    // file_name[length + 2] = 't';
    // file_name[length + 3] = 'x';
    // file_name[length + 4] = 't';

    *name = file_name;
    return ok;
}

int Operator_array_init(Operator** arr, unsigned int size) {
    *arr = malloc(sizeof(Operator) * size);
    if (*arr == NULL) {
        return MEMORY_ISSUES;
    }

    int st = ok;
    for (int i = 0; i < size; i++) {
        int flag = 0;

        do {
            char* new_name = NULL;
            st = generate_random_str(&new_name);
            flag = 0;

            if (st != ok) {
                Operator_array_free(arr, i);
                return st;
            }

            for (int j = 0; j < i; j++) {
                if (!strcmp((*arr)[j].name, new_name)) {
                    flag = 1;
                    free(new_name);
                    continue;
                }
            }

            st = Operator_init(&((*arr)[i]), new_name);
            if (st != ok) {
                Operator_array_free(arr, i);
                return st;
            }
            //(*arr)[i].name = new_name;
        } while (flag);
    }

    return ok;
}

void Operator_array_free(Operator** arr, unsigned int size) {
    if (*arr == NULL) {
        return;
    }

    for (int i = 0; i < size; i++) {
        Operator_free(&((*arr)[i]));
    }

    free(*arr);

    *arr = NULL;

    //return ok;
}

int Department_init(
    Department** dep,
    char* name,
    int pq_type,
    unsigned int operators_amount,
    double overload_coeff,
    double eps, 
    
    unsigned int min_handling_time,
    unsigned int max_handling_time,
    int (*comparator) (const Application*, const Application*)
    ) {
    if (name == NULL || comparator == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    int st = ok;
    (*dep) = (Department*)malloc(sizeof(Department));
    if (dep == NULL) {
        return MEMORY_ISSUES;
    }

    (*dep)->name = name;
    
    Priority_queue* tmp = (Priority_queue*)malloc(sizeof(Priority_queue));
    if (tmp == NULL) {
        free(*dep);
        return MEMORY_ISSUES;
    }

    st = Priority_queue_init(tmp, pq_type, comparator);
    if (st != ok) {
        free(*dep);
        free(tmp);
        return st;
    }
    (*dep)->applications = tmp;

    (*dep)->operators_amount = operators_amount;
    st = Operator_array_init(&((*dep)->staff), operators_amount);
    if (st != ok) {
        Priority_queue_free((*dep)->applications);
        free(*dep);
        free(tmp);
        return st;
    }

    (*dep)->free_operators_amount = operators_amount;
    (*dep)->load_coeff = 0.0;
    (*dep)->overload_coeff = overload_coeff;
    (*dep)->eps = eps;

    (*dep)->max_handlind_time = max_handling_time;
    (*dep)->min_handling_time = min_handling_time;

    return ok;
}

int Department_set_null(Department* dep) {
    if (dep == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    dep->name = NULL;
    dep->applications = NULL;
    dep->staff = NULL;

    return ok;
}

void Department_free(Department* dep) {
    if (dep == NULL) {
        return;
    }

    Priority_queue_free(dep->applications);
    free(dep->applications);
    free(dep->name);
    Operator_array_free(&(dep->staff), dep->operators_amount);

    Department_set_null(dep);

    free(dep);
}

void Department_print(FILE* stream, Department* dep) {
    if (dep == NULL) {
        fprintf(stream, "Ptr is null.\n");
        return;
    }

    fprintf(stream, "Name: %s\n", dep->name);
    fprintf(stream, "Op. am.: %u\n", dep->operators_amount);
    fprintf(stream, "Free_op_amount: %u\n", dep->free_operators_amount);
    fprintf(stream, "Load_coeff: %f\n", dep->load_coeff);
    fprintf(stream, "Overloaf_coeff: %f\n", dep->overload_coeff);
    fprintf(stream, "Eps: %f\n", dep->eps);
    fprintf(stream, "min_handling_time: %u\n", dep->min_handling_time);
    fprintf(stream, "max_handling_time: %u\n\n", dep->max_handlind_time);
}

int Department_handling_finishing(Department* dep, char* tmp_time, Logger* log) {
    if (dep == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    for (int i = 0; i < dep->operators_amount; i++) {
        if (dep->staff[i].a != NULL) {
            if (strcmp(dep->staff[i].finish_time, tmp_time) <= 0) {
                //log_this
                //printf("Handled: %s\n", dep->staff[i].a->text);
                
                int st = Logger_add_and_init_message(log, dep->staff[i].finish_time,
                 REQUEST_HANDLING_FINISHED, 0, dep->staff[i].a->id, NULL, dep->staff[i].name,
                  dep->staff[i].handling_time);

                free_application(dep->staff[i].a);
                dep->staff[i].a = NULL;

                free(dep->staff[i].start_time);
                free(dep->staff[i].finish_time);
                dep->staff[i].start_time = NULL;
                dep->staff[i].finish_time = NULL;
                dep->staff[i].handling_time = 0;
                
                (dep->free_operators_amount)++;
                
                if (st != ok) {
                    return st;
                }

            }
        }
    }

    int size;
    int st = Priority_queue_size(dep->applications, &size);
    if (st != ok) {
        return st;
    }

    dep->load_coeff = size / dep->operators_amount;

    return ok;
}

int Department_give_application(Department* dep, Application* a, char* tmp_time, Logger* log) {
    if (dep == NULL || a == NULL || tmp_time == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    int st = ok;

    st = Priority_queue_insert(dep->applications, a);
    if (st != ok) {
        return st;
    }

    if (dep->free_operators_amount > 0) {
        for (int i = 0; i < dep->operators_amount; i++) {
            if (dep->staff[i].a == NULL) {
                Application* for_handling = NULL;
                st = Priority_queue_del_max(dep->applications, &for_handling);
                if (st != ok) {
                    return st;
                }

                st = Operator_give_task(dep, i, tmp_time, for_handling);
                if (st != ok) {
                    free_application(for_handling);
                    return st;
                }

                st = Logger_add_and_init_message(log, tmp_time,
                 REQUEST_HANDLING_STARTED, 0, a->id, NULL,
                  dep->staff[i].name, dep->staff[i].handling_time);

                if (st != ok) {
                    free_application(for_handling);
                    return st;
                }
                //log_this

                break;
            }
        }
    }

    int size;
    st = Priority_queue_size(dep->applications, &size);
    if (st != ok) {
        return st;
    }

    dep->load_coeff = size / dep->operators_amount;

    return ok;
}


