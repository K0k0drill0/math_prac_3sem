#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include "priority_queue/priority_queue.h"
#include "data_structs.h"

//#include "map/map.h"

// Название (идентификатор)

/*Что известно о департаменте?
Его название(идентификатор)
его приоритетная очередь с заявками
Его количество операторов
Количество свободных операторов
Имена его операторов
*/

typedef struct Operator {
    char* name;
    Application* a;
    char* start_time;
    char* finish_time;

    unsigned int handling_time;
} Operator;

typedef struct Department {
    char* name;
    Priority_queue* applications;
    unsigned int operators_amount;
    Operator* staff;
    unsigned int free_operators_amount;
    double load_coeff;
    double overload_coeff;
    double eps;

    unsigned int min_handling_time;
    unsigned int max_handlind_time;
} Department;

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
    );

int Department_set_null(Department* d);
void Department_free(Department* dep);

void Department_print(FILE* stream, Department* dep);

int Department_handling_finishing(Department* dep, char* tmp_time);
int Department_give_application(Department* dep, Application* a, char* tmp_time);




#endif