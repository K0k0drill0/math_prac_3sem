#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <string.h>

#include "errors.h"
#include "data_structs.h"
#include "readers.h"
#include "validators.h"

#include "priority_queue/priority_queue.h"
#include "priority_queue/Binary_heap.h"
#include "priority_queue/Treap.h"

#include "map/map.h"

#include "Department.h"
#include "logger.h"

#include "map/Dynamic_array.h"

int setup_configuration(FILE* inp, Map* Departments, char** start_time,
 char** finish_time, unsigned int *deps_amount, char*** deps_names) {
    
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

    *deps_amount = arr.size;
    *deps_names = (char**)malloc(sizeof(char*) * *deps_amount);
    if (*deps_names == NULL) {
        str_pair_arr_free(&arr);
        free(*start_time);
        free(*finish_time);
        Map_free(Departments);
        
        return MEMORY_ISSUES;
    }

    for (int i = 0; i < arr.size; i++) {
        char* name = NULL, *op_am = NULL;

        name = arr.data[i].name;
        op_am = arr.data[i].op_am;

        arr.data[i].name = NULL;
        arr.data[i].op_am = NULL;

        (*deps_names)[i] = name;

        unsigned int iop_am = atoi(op_am);
        if (errno == ERANGE || iop_am < 10 || iop_am > 50) {
            str_pair_arr_free(&arr);
            free(*start_time);
            free(*finish_time);

            free(name);
            free(op_am);

            Map_free(Departments);

            free(*deps_names);

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

            free(*deps_names);

            return st;
        }

        st = Map_insert(Departments, name, dep);
        if (st != ok) {
            str_pair_arr_free(&arr);
            free(*start_time);
            free(*finish_time);

            Department_free(dep);

            Map_free(Departments);

            free(*deps_names);

            return st;
        }
    }

    
    str_pair_arr_free(&arr);

    return ok;
}

int build_application(Unvalidated_application* source, Application** res, int max_priority, Logger* log) {
    if (source == NULL || res == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    int runtime_error = ok;

    int st = validate_application_data(source->arrival_time, source->id, source->priority, source->dep_id, source->text, &runtime_error);
    if (st != ok) {
        Unvalidated_application_free(source);
        return st;
    }

    if (runtime_error != ok) {
        st = Logger_add_and_init_message(log, source->arrival_time, RUNTIME_ERROR,
         runtime_error, 0, NULL, NULL, 0);
        //log_this
        //print_runtime_error(stdout, runtime_error);
        if (st != ok) {
            Unvalidated_application_free(source);
            return st;
        }
        Unvalidated_application_free(source);
        return ok;
    }

    int ipriority = atoi(source->priority);
    int iid = atoi(source->id);
    //free_all(source->priority, source->id);

    if (errno == ERANGE || ipriority > max_priority) {
        //log_this
        st = (errno == ERANGE) ? 
        Logger_add_and_init_message(log, source->arrival_time, RUNTIME_ERROR, OVERFLOW, 0, NULL, NULL, 0) 
        : Logger_add_and_init_message(log, source->arrival_time, runtime_error, INVALID_PRIORITY, 0, NULL, NULL, 0);

        //print_runtime_error(stdout, runtime_error);
        Unvalidated_application_free(source);

        return st;
    }

    remove_quotes(source->text);

    st = initiate_application(res, source->arrival_time,
     ipriority, source->text, iid, source->dep_id);

    if (st != ok) {
        Unvalidated_application_free(source);
        return st;
    }

    free_all(2, source->priority, source->id);

    Unvalidated_application_set_null(source);

    return ok;
}

int handle_file_with_applications(FILE* inp, Unvalidated_application_arr* arr) {
    if (inp == NULL || arr == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    int st = ok;
    int runtime_error = ok;
    Unvalidated_application a;
    
    while ((st = read_application_data(inp, &a)) == ok) {
        st = Unvalidated_application_arr_push_back(arr, a);
        if (st != ok) {
            Unvalidated_application_free(&a);
            return st;
        }
    }

    if (st == EOF) {
        return ok;
    }

    return st;
}

int handle_files_with_applications(int argc, char** argv, Unvalidated_application_arr* arr) {
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

    //Application_arr_sort(*arr);

    return ok;
}

int Department_handle_overload(Department* dep, Map* Departments,
 unsigned int deps_amount, char** deps_names, Application* a, Logger* log) {

    if (dep == NULL || Departments == NULL || deps_names == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    if (dep->load_coeff < dep->overload_coeff) {
        return ok;
    }

    int st = ok;

    Department* dep_min_load = dep;
    Department* dep_tmp = NULL;

    int need_transfer;
    st = Priority_queue_size(dep->applications, &need_transfer);
    if (st != ok) {
        return st;
    }

    //int res_load_coeff = dep->load_coeff;

    for (int i = 0; i < deps_amount; i++) {
        st = Map_find(Departments, deps_names[i], &dep_tmp);
        if (st != ok) {
            return st;
        }

        if (dep_tmp == dep_min_load) {
            continue;
        }

        int tmp_pq_size;
        st = Priority_queue_size(dep_tmp->applications, &tmp_pq_size);
        if (st != ok) {
            return st;
        }

        int res_load_coeff = (need_transfer + tmp_pq_size) / dep_tmp->operators_amount;
        if (res_load_coeff < dep_min_load->load_coeff) {
            dep_min_load = dep_tmp;
        }
    }

    if (dep_min_load == dep) {
        st = Logger_add_and_init_message(log, a->arrival_time, DEPARTMENT_OVERLOADED,
         0, a->id, NULL, NULL, 0);

        return st;
    }

    Priority_queue pq_tmp;


    st = Priority_queue_meld(dep_min_load->applications, dep_min_load->applications, dep->applications);
    if (st != ok) {
        return st;
    }

    st = Logger_add_and_init_message(log, a->arrival_time, DEPARTMENT_OVERLOADED,
     0, a->id, dep_min_load->name, NULL, 0);

    return st;
}

int Departments_handle_finish_tasks(Map* Departments, char* tmp_time,
 unsigned int deps_amount, char** deps_names, Logger* log) {

    if (Departments == NULL || deps_names == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    int st = ok;

    Department* tmp_dep = NULL;
    
    for (int i = 0; i < deps_amount; i++) {
        st = Map_find(Departments, deps_names[i], &tmp_dep);
        if (st != ok) {
            return st;
        }

        st = Department_handling_finishing(tmp_dep, tmp_time, log);
        if (st != ok) {
            return st;
        }
    }   

    return ok;

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

int Departments_post_handle(Map* Departments, unsigned int deps_amount,
 char** deps_names, char* final_time, Logger* log) {
    if (Departments == NULL || deps_names == NULL ||final_time == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    int st = ok;

    for (int i = 0; i < deps_amount; i++) {
        Department* tmp_dep;
        st = Map_find(Departments, deps_names[i], &tmp_dep);
        if (st != ok) {
            return st;
        }

        unsigned int tmp_pq_size;
        st = Priority_queue_size(tmp_dep->applications, &tmp_pq_size);
        if (st != ok) {
            return st;
        }

        while (tmp_pq_size > 0 && tmp_dep->free_operators_amount > 0) {
            st = st ? st : Department_handling_finishing(tmp_dep, final_time, log);

            st = st ? st : Department_give_work_to_free_operators(tmp_dep, final_time, log);

            st = st ? st : Priority_queue_size(tmp_dep->applications, &tmp_pq_size);

            if (st != ok) {
                return st;
            }
        }
        st = Department_handling_finishing(tmp_dep, final_time, log);
        if (st != ok) {
            return st;
        }
    }

    return ok;
}

int main(int argc, char** argv) {

    srand(time(NULL));

    argc = 4;

    argv[1] = "20";
    argv[2] = "config";
    argv[3] = "application2";
    argv[4] = "application1";

    FILE* config = fopen(argv[2], "r");
    if (config == NULL) {
        print_error(stdout, UNABLE_TO_OPEN_FILE);
        return -1;
    }

    int  max_priority = atoi(argv[1]);

    int st = ok;

    Map Departments;
    char* start_time = NULL;
    char* finish_time = NULL;

    unsigned int deps_amount;
    char** deps_names = NULL;

    st = setup_configuration(config, &Departments, &start_time, &finish_time, &deps_amount, &deps_names);
    if (st != ok) {
        fclose(config);
        print_error(stdout, st);
        return 1;
    }
    fclose(config);
    //return 0;

    Unvalidated_application_arr arr;

    st = Unvalidated_application_arr_init(&arr);
    if (st != ok) {
        print_error(stdout, st);
        
        Map_free(&Departments);
        free(deps_names);
        free(start_time);
        free(finish_time);
        return 1;
    }

    st = handle_files_with_applications(argc, argv, &arr);
    if (st != ok) {
        print_error(stdout, st);
        Unvalidated_application_arr_free(&arr);

        Map_free(&Departments);
        free(deps_names);
        free(start_time);
        free(finish_time);
        return 1;
    }

    FILE* log_file = fopen("log", "w+");
    if (log_file == NULL) {
        print_error(stdout, UNABLE_TO_OPEN_FILE);

        Unvalidated_application_arr_free(&arr);

        Map_free(&Departments);
        free(deps_names);
        free(start_time);
        free(finish_time);
        return 0;
    }

    Logger log;

    //st = Logger_init(&log, stdout);
    st = Logger_init(&log, log_file);
    if (st != ok) {
        print_error(stdout, st);

        Unvalidated_application_arr_free(&arr);

        Map_free(&Departments);
        free(deps_names);
        free(start_time);
        free(finish_time);
        fclose(log_file);
        return 1;
    } 


    // char* tmp_time = start_time;
    char* tmp_time = NULL;
    st = strcpy_with_malloc(&tmp_time, start_time);
    if (st != ok) {
        print_error(stdout, MEMORY_ISSUES);

        Unvalidated_application_arr_free(&arr);

        Map_free(&Departments);
        free(deps_names);
        free(start_time);
        free(finish_time);
        fclose(log_file);

        return 1;
    }
    int tmp_application_index = 0;
    while (tmp_application_index < arr.size && strcmp(arr.data[tmp_application_index].arrival_time, tmp_time) < 0) {
        tmp_application_index++;
    }

    while (!st && strcmp(tmp_time, finish_time) <= 0) {
        st = Departments_handle_finish_tasks(&Departments,
         tmp_time, deps_amount, deps_names, &log);
        if (st != ok) {
            return st;
        }

        while (tmp_application_index < arr.size && strcmp(arr.data[tmp_application_index].arrival_time, tmp_time) <= 0) {
            Application* tmp_a = NULL;
            st = build_application(&(arr.data[tmp_application_index]), &tmp_a,
             max_priority, &log);

            if (st != ok) {
                break;
            }

            if (tmp_a == NULL) {
               st = Logger_log(&log);
               tmp_application_index++;
               continue;
            }

            Department* tmp_dep = NULL;
            st = Map_find(&Departments, tmp_a->dep_id, &tmp_dep);
            if (st != ok) {
                free_application(tmp_a);
                break;
            }

            if (tmp_dep == NULL) {
                st = Logger_add_and_init_message(&log, tmp_time, RUNTIME_ERROR,
                INVALID_DEPARTMENT_NAME, 0, NULL, NULL, 0);

                free_application(tmp_a);

                if (st != ok) {
                    break;
                }

                st = Logger_log(&log);
                if (st != ok) {
                    break;
                }
                tmp_application_index++;
                continue;
            }

            st = Department_give_application(tmp_dep, tmp_a, tmp_time, &log);
            if (st != ok) {
                free_application(tmp_a);
                break;
            }

            st = Department_handle_overload(tmp_dep, &Departments, deps_amount, deps_names, tmp_a, &log);
            if (st != ok) {
                break;
            }

            st = Logger_log(&log);
            if (st != ok) {
                break;
            }
            tmp_application_index++;
        }

        for (int i = 0; i < deps_amount; i++) {
            Department* dep_tmp = NULL;
            st = Map_find(&Departments, deps_names[i], &dep_tmp);
            if (st != ok) {
                return st;
            }

            st = Department_give_work_to_free_operators(dep_tmp, tmp_time, &log);
            if (st != ok) {
                break;
            } 
        }

        if (st != ok) {
            break;
        }
        
        char* new_time = NULL;
        st = iso_time_add(tmp_time, 60, &new_time);
        if (st != ok) {
            break;
        }
        free(tmp_time);
        tmp_time = new_time;
    }

    st = st ? st : Logger_log(&log); 

    Unvalidated_application_arr_free(&arr);

    Map_free(&Departments);
    free(deps_names);
    free(start_time);
    start_time = NULL;
    free(tmp_time);
    free(finish_time);
    Logger_free(&log);

    if (st != ok) {
        print_error(stdout, st);
        return 1;
    }
    
    return 0;
}
