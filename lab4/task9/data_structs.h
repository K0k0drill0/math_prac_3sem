#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

#include <stdio.h>

typedef long long ll;
typedef unsigned long long ull;

typedef struct {
    char* arrival_time; // format "YYYY-MM-DDTHH:MM:SSZ"
    int priority;
    char* text;

    int id; // откуда брать этот ебучий идентификатор я без понятия
    char* dep_id;
} Application;

typedef struct Application_arr {
    Application** data;
    int size;
    int max_size;
} Application_arr;

//___________________________________________

typedef struct str_pair {
    char* name, *op_am;
} str_pair;

typedef struct str_pair_arr {
    str_pair* data;
    size_t size;
    size_t max_size;
} str_pair_arr;

//___________________________________________

typedef struct Unvalidated_application {
    char* arrival_time;
    char* priority;
    char* text;

    char* id;
    char* dep_id;
} Unvalidated_application;

typedef struct Unvalidated_application_arr{
    Unvalidated_application* data;
    size_t size;
    size_t max_size;
} Unvalidated_application_arr;

//___________________________________________

int Unvalidated_application_init(Unvalidated_application* res, char* arrival_time, char* priority, char* text, char* id, char* dep_id);
void Unvalidated_application_free(Unvalidated_application* a);
int Unvalidated_application_set_null(Unvalidated_application* a);

int Unvalidated_application_arr_init(Unvalidated_application_arr* arr);
int Unvalidated_application_arr_push_back(Unvalidated_application_arr* arr, Unvalidated_application el);
//int Unvalidated_application_arr_pop_back(Unvalidated_application* arr, Unvalidated_application* el);
void Unvalidated_application_arr_free(Unvalidated_application_arr* arr);
void Unvalidated_application_arr_print(FILE* stream, Unvalidated_application_arr arr);


int initiate_application ( 
    Application** a,
    char* arrival_time, 
    int priority, 
    char* text, 
    int id,
    char* dep_id
    );

int Application_arr_init(Application_arr* arr);
int Application_arr_push_back(Application_arr* arr, Application* a) ; // Если память не выделилась, то функция ничего не освободит
int Application_arr_copy(Application_arr* dest, Application_arr source);
int Application_arr_concatenation(const Application_arr fir, const Application_arr sec, Application_arr* res);
void Application_arr_sort(Application_arr arr);


void Application_arr_free(Application_arr* arr);
int Application_arr_print(FILE* stream, Application_arr arr);

int str_pair_arr_init(str_pair_arr* arr);
int str_pair_arr_push_back(str_pair_arr* arr, str_pair pair);
int str_pair_arr_free(str_pair_arr* arr);

int strcpy_with_malloc(char** dest, const char* source);
void swap_ptr(void** a, void** b);

int copy_application(Application** dest, Application* source);

void free_application(Application* apl);
int compare_applications(const Application* a1, const Application* a2);
void print_application(FILE* stream, const Application* a);

void free_all(unsigned int amount, ...);

int ctoi(char ch);
int bpow_safely(ll base, ll pow, ll* res);
int mult_safely(ll arg_1, ll arg_2, ll* res);

int iso_time_add(char* time, ull add_s, char** res);
unsigned rand_32();

#endif