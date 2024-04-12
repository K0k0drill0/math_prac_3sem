#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

#include "errors.h"
#include "data_structs.h"
#include "readers.h"
#include "validators.h"

#include "priority_queue/priority_queue.h"
#include "priority_queue/Binary_heap.h"
#include "priority_queue/Treap.h"

#include "map/map.h"

#include "Department.h"

#include "map/Dynamic_array.h"

int setup_configuration(FILE* inp, Map* Departments, char** start_time, char** finish_time) {
    char *pq_type = NULL;
    char *map_type = NULL;
     *start_time = NULL;
     *finish_time = NULL;
    char *min_handling_time = NULL;
    char *max_handling_time = NULL;
    char *dep_amount = NULL;
    char *overload_coeff = NULL;

    str_pair_arr arr;

    int st = ok;

    st = str_pair_arr_init(&arr);
    if (st != ok) {
        return st;
    }

    st = read_config_file(
        inp,
        &pq_type,
        &map_type, 
        start_time, 
        finish_time,
        &min_handling_time,
        &max_handling_time, 
        &dep_amount,
        &overload_coeff,
        &arr);

    if (st != ok) {
        return st;
    }

    if (!is_valid_config_file(pq_type, map_type, *start_time, *finish_time, min_handling_time, max_handling_time, dep_amount, overload_coeff, arr)) {
        free_all(8 ,
        pq_type,
        map_type, 
        *start_time, 
        *finish_time,
        min_handling_time,
        max_handling_time, 
        dep_amount,
        overload_coeff
        );

        str_pair_arr_free(&arr);

        return INVALID_CONFIG_FILE;
    }

    Priority_queue_type pq_t;
    map_base_type map_b;

    which_pq_type(pq_type, &pq_t);
    which_map_type(map_type, &map_b);
    
    unsigned int imin_handling_time = atoi(min_handling_time);
    if (errno == ERANGE) {
        st = INVALID_CONFIG_FILE;
    }
    unsigned int imax_handling_type = atoi(max_handling_time);
    if (errno == ERANGE) {
        st = INVALID_CONFIG_FILE;
    }
    double doverload_coeff = atof(overload_coeff);
    if (doverload_coeff < 1.0) {
        st = INVALID_CONFIG_FILE;
    }

    free_all(6, pq_type, map_type, min_handling_time,
     max_handling_time, dep_amount, overload_coeff);

    if (st != ok) {
        str_pair_arr_free(&arr);
        free(*start_time);
        free(*finish_time);
        return st;
    }

    st = Map_init(Departments, map_b);
    if (st != ok) {
        str_pair_arr_free(&arr);
        free(*start_time);
        free(*finish_time);

        return st;
    }

    for (int i = 0; i < arr.size; i++) {
        char* name = NULL, *op_am = NULL;

        name = arr.data[i].name;
        op_am = arr.data[i].op_am;

        arr.data[i].name = NULL;
        arr.data[i].op_am = NULL;

        unsigned int iop_am = atoi(op_am);
        if (errno == ERANGE || iop_am < 10 || iop_am > 50) {
            str_pair_arr_free(&arr);
            free(*start_time);
            free(*finish_time);

            free(name);
            free(op_am);

            Map_free(Departments);

            return INVALID_CONFIG_FILE;
        }
        free(op_am);

        Department* dep = NULL;
        st = Department_init(&dep, name, pq_t, iop_am, doverload_coeff,
         1e-9, imin_handling_time, imax_handling_type,
          compare_applications);

        if (st != ok) {
            str_pair_arr_free(&arr);
            free(*start_time);
            free(*finish_time);

            free(name);

            Map_free(Departments);

            return st;
        }

        st = Map_insert(Departments, name, dep);
        if (st != ok) {
            str_pair_arr_free(&arr);
            free(*start_time);
            free(*finish_time);

            Department_free(dep);

            Map_free(Departments);

            return st;
        }
    }
    str_pair_arr_free(&arr);

    return ok;
}

int build_application(FILE* inp, Application** res, int* runtime_error) {
    if (inp == NULL || res == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    int st = ok;

    char* time = NULL;
    char* message_id = NULL;
    char* priority = NULL;
    char* dep_id = NULL;
    char* text = NULL;

    *runtime_error = ok;

    st = read_application_data(inp, &time, &message_id, &priority, &dep_id, &text, runtime_error);
    if (st != ok || *runtime_error != ok) {
        free_all(5, time, message_id, priority, dep_id, text);
        return st;
    }

    st = validate_application_data(time, message_id, priority, dep_id, text, runtime_error);
    if (st != ok || *runtime_error != ok) {
        free_all(5, time, message_id, priority, dep_id, text);
        return st;
    } 

    int imessage_id = atoi(message_id);
    int ipriority = atoi(priority);

    if (errno == ERANGE) {
        free_all(5, time, message_id, priority, dep_id, text);
        *runtime_error = OVERFLOW;
        return ok;
    }
    free(message_id);
    free(priority);

    remove_quotes(text);

    //Application* a = NULL;
    st = initiate_application(res, time, ipriority, text, imessage_id, dep_id);
    if (st != ok) {
        free_all(5, time, message_id, priority, dep_id, text);
        return st;
    }

    return ok;
}

int handle_file_with_applications(FILE* inp, Application_arr* arr) {
    if (inp == NULL || arr == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    int st = ok;
    int runtime_error = ok;
    Application* a = NULL;
    
    while ((st = build_application(inp, &a, &runtime_error)) == ok) {
        if (runtime_error != ok) {
            //log_it
            print_runtime_error(stdout, runtime_error);  // tmp_log
            runtime_error = ok;
            continue;
        }

        st = Application_arr_push_back(arr, a);
        if (st != ok) {
            free_application(a);
            return st;
        }
    }

    if (st == EOF) {
        return ok;
    }

    return st;
}

int handle_files_with_applications(int argc, char** argv, Application_arr* arr) {
    if (argv == NULL || arr == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    for (int i = 3; i < argc; i++) {

        FILE* appl_file = fopen(argv[i], "r");
        if (appl_file == NULL) {
            return UNABLE_TO_OPEN_FILE;
        }

        int st = ok;
        st = handle_file_with_applications(appl_file, arr);
        
        fclose(appl_file);

        if (st != ok) {
            return st;
        }
    }

    Application_arr_sort(*arr);

    return ok;
}

int handle_applications(Application_arr arr, Map departments) {
    
}

int is_valid_args(int argc, char** argv) {
    if (argc < 2) {
        return INVALID_AMOUNT_OF_ARGUMENTS;
    }

    if (!is_valid_uint(argv[1])) {
        return INVALID_MAX_PRIORITY;
    }

    return ok;

    // validate_filenames
}

int main(int argc, char** argv) {

    argc = 5;

    argv[1] = "20";
    argv[2] = "config";
    argv[3] = "application1";
    argv[4] = "application2";

    int st = ok;

    Application_arr arr;

    st = Application_arr_init(&arr);
    if (st != ok) {
        print_error(stdout, st);
        return 1;
    }

    st = handle_files_with_applications(argc, argv, &arr);
    if (st != ok) {
        print_error(stdout, st);
        Application_arr_free(&arr);
        return 1;
    }

    Application_arr_print(stdout, arr);
    Application_arr_free(&arr);

    return 0;
}
