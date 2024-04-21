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

int build_application(Unvalidated_application* source, Application** res, int max_priority) {
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
        //log_this
        print_runtime_error(stdout, runtime_error);
        Unvalidated_application_free(source);
        return ok;
    }

    int ipriority = atoi(source->priority);
    int iid = atoi(source->id);
    //free_all(source->priority, source->id);

    if (errno == ERANGE || ipriority > max_priority) {
        //log_this
        (errno == ERANGE) ? print_runtime_error(stdout, OVERFLOW) 
        : print_runtime_error(stdout, INVALID_PRIORITY);

        print_runtime_error(stdout, runtime_error);
        Unvalidated_application_free(source);
        return ok;
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
 unsigned int deps_amount, char** deps_names) {

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
        return ok;
    }

    st = Priority_queue_meld(dep_min_load->applications, dep_min_load->applications, dep->applications);
    if (st != ok) {
        return st;
    }

    return ok;

}

int Departments_handle_finish_tasks(Map* Departments, char* tmp_time,
 unsigned int deps_amount, char** deps_names) {

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

        st = Department_handling_finishing(tmp_dep, tmp_time);
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

int main(int argc, char** argv) {

    argc = 4;

    argv[1] = "20";
    argv[2] = "config";
    argv[3] = "application1";
    argv[4] = "application2";

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

    char* tmp_time = start_time;

    for (int i = 0; i < arr.size; i++) {
        Application* tmp_a = NULL;
        
        st = build_application(&(arr.data[i]), &tmp_a, max_priority);
        if (st != ok) {
            break;
        }

        if (tmp_a == NULL) {
            // probably_log
            continue;
        }

        tmp_time = tmp_a->arrival_time;
        if (strcmp(tmp_time, finish_time) > 0 || strcmp(tmp_time, start_time) < 0) {
            free_application(tmp_a);
            break;
        }

        st = Departments_handle_finish_tasks(&Departments, tmp_time,
         deps_amount, deps_names);
        if (st != ok) {
            continue;
        }

        Department* tmp_dep = NULL;
        st = Map_find(&Departments, tmp_a->dep_id, &tmp_dep);
        if (st != ok) {
            // free_tmp_a??
            break;
        }

        if (tmp_dep == NULL) {
            //log_this
            //free_applicaation
            continue;
        }

        st = Department_give_application(tmp_dep, tmp_a, tmp_time);
        if (st != ok) {
            break;
        }

        st = Department_handle_overload(tmp_dep, &Departments, deps_amount, deps_names);
        if (st != ok) {
            break;
        }
    }


    st = Departments_handle_finish_tasks(&Departments, finish_time,
         deps_amount, deps_names);

    Unvalidated_application_arr_free(&arr);

    Map_free(&Departments);
    free(deps_names);
    free(start_time);
    free(finish_time);

    if (st != ok) {
        print_error(stdout, st);
        return 1;
    }
    
    return 0;
}
