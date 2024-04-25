#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <limits.h>
#include <time.h>
#include <errno.h>
//#include <algorithm>

#include "errors.h"
#include "data_structs.h"

typedef long long ll;
typedef unsigned long long ull;

int Unvalidated_application_init(
    Unvalidated_application* res,
    char* arrival_time,
    char* priority,
    char* text,
    char* id,
    char* dep_id) {
    
    if (res == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    res->arrival_time = arrival_time;
    res->priority = priority;
    res->text = text;
    res->id = id;
    res->dep_id;

    return ok;
}

void Unvalidated_application_free(Unvalidated_application* a) {
    if (a == NULL) {
        return;
    }

    free_all(5, a->arrival_time, a->dep_id, a->id, a->priority, a->text);
    a->arrival_time = NULL;
    a->dep_id = NULL;
    a->id = NULL;
    a->priority = NULL;
    a->text = NULL; 
}

int Unvalidated_application_set_null(Unvalidated_application* a) {
    if (a == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    a->arrival_time = NULL;
    a->dep_id = NULL;
    a->id = NULL;
    a->priority = NULL;
    a->text = NULL;

    return ok;
}

void Unvalidated_application_print(FILE* stream, Unvalidated_application a) {
    if (stream == NULL) {
        return;
    }

    fprintf(stream, "Arrival time: %s\n", a.arrival_time);
    fprintf(stream, "Priority: %s\n", a.priority);
    fprintf(stream, "Text: %s\n", a.text);
    fprintf(stream, "Id: %s\n", a.id);
}

int Unvalidated_application_arr_init(Unvalidated_application_arr* arr) {
    if (arr == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    arr->size = 0;
    arr->max_size = 16;

    arr->data = (Unvalidated_application*)malloc(sizeof(Unvalidated_application) * arr->max_size);
    if (arr->data == NULL) {
        free(arr);
        return MEMORY_ISSUES;
    }

    return ok;
}

int Unvalidated_application_arr_push_back(Unvalidated_application_arr* arr, Unvalidated_application el) {
    if (arr == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }


    if (arr->data == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    if (arr->size == arr->max_size) {
        arr->max_size *= 2;
        Unvalidated_application* tmp = (Unvalidated_application*)realloc(arr->data,
         sizeof(Unvalidated_application) * arr->max_size);

        if (tmp == NULL) {
            return MEMORY_ISSUES;
        }
        
        arr->data = tmp;
    }

    arr->data[arr->size] = el;
    (arr->size)++;

    return ok;
}

void Unvalidated_application_arr_free(Unvalidated_application_arr* arr) {
    if (arr == NULL) {
        return;
    }

    for (int i = 0; i < arr->size; i++) {
        Unvalidated_application_free(&(arr->data[i]));
    }
    free(arr->data);
    arr->data = NULL;
    arr->size = 0;
    arr->max_size = 0;
}

void Unvalidated_application_arr_print(FILE* stream, Unvalidated_application_arr arr) {
    for (int i = 0; i < arr.size; i++) {
        Unvalidated_application_print(stream, arr.data[i]);
        fprintf(stream, "\n");   
    }
    fprintf(stream, "\n");   
}


int Application_arr_init(Application_arr* arr) {
    arr->size = 0;
    arr->max_size = 16;

    arr->data = (Application**)malloc(sizeof(Application*) * arr->max_size);
    if (arr->data == NULL) {
        free(arr);
        return MEMORY_ISSUES;
    }

    return ok;
}

int Application_arr_push_back(Application_arr* arr, Application* a) { // Если память не выделилась, то функция ничего не освободит
    if (arr == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    if (arr->data == NULL || a == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    if (arr->size == arr->max_size) {
        arr->max_size *= 2;
        Application** tmp = (Application**)realloc(arr->data,
         sizeof(Application*) * arr->max_size);

        if (tmp == NULL) {
            return MEMORY_ISSUES;
        }

        arr->data = tmp;
    }

    arr->data[arr->size] = a;
    (arr->size)++;

    return ok;
}

int Application_arr_copy(Application_arr* dest, Application_arr source) {
    if (source.data == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    dest->size = source.size;
    dest->max_size = source.max_size;

    int st = ok;

    dest->data = (Application**)malloc(sizeof(Application*) * dest->max_size);
    if (dest->data == NULL) {
        return MEMORY_ISSUES;
    }

    for (int i = 0; i < source.size; i++) {
        Application* new_el = NULL;

        st = copy_application(&new_el ,source.data[i]);
        if (st != ok) {
            for (int j = 0; j < i; j++) {
                free_application(dest->data[j]);
            }

            free(dest->data);
            return MEMORY_ISSUES;
        }
        dest->data[i] = new_el;
    }

    return ok;
}

int Application_arr_concatenation(const Application_arr fir, const Application_arr sec, Application_arr* res) {
    if (fir.data == NULL || sec.data == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    int st = ok;

    st = Application_arr_copy(res, fir);
    if (st != ok) {
        return st;
    }

    for (int i = 0; i < sec.size; i++) {
        Application* new_el = NULL;
        st = copy_application(&new_el, sec.data[i]);
        if (st != ok) {
            Application_arr_free(res);
            return st;
        }

        st = Application_arr_push_back(res, new_el);
        if (st != ok) {
            free_application(new_el);
            Application_arr_free(res);
            return st;
        }
    }

    return ok;
}

void Application_arr_free(Application_arr* arr) {
    if (arr == NULL) {
        return;
    }
    for (int i = 0; i < arr->size; i++) {
        free_application(arr->data[i]);
    }

    arr->size = 0;
    arr->max_size = 0;

    free(arr->data);
}

int Application_arr_print(FILE* stream, Application_arr arr) {
    if (arr.data == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    for (int i = 0; i < arr.size; i++) {
        print_application(stream, arr.data[i]);
        fprintf(stream, "\n");
    }

    return ok;
}

int cmp_for_arr_sort(const void* a1, const void* a2) {
    Application* pa1 = (Application*)a1;
    Application* pa2 = (Application*)a2;
    return (strcmp(pa2->arrival_time, pa1->arrival_time));
}

void Application_arr_sort(Application_arr arr) {
    if (arr.size == 0 || arr.size == 1 || arr.data == NULL) {
        return;
    }
    qsort(arr.data, arr.size, sizeof(Application*), cmp_for_arr_sort);
}

int str_pair_arr_init(str_pair_arr* arr) {
    if (arr == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    arr->size = 0;
    arr->max_size = 16;

    arr->data = (str_pair*)malloc(sizeof(str_pair) * arr->max_size);
    if (arr->data == NULL) {
        return MEMORY_ISSUES;
    }

    return ok;
}

int str_pair_arr_push_back(str_pair_arr* arr, str_pair pair) {
    if (arr == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    if (arr->size == arr->max_size) {
        arr->max_size *= 2;
        str_pair* tmp = (str_pair*)realloc(arr->data, sizeof(str_pair) * arr->max_size);
        if (tmp == NULL) {
            arr->max_size /= 2;
            return MEMORY_ISSUES;
        }
        arr->data = tmp;
    }

    arr->data[arr->size] = pair;

    arr->size += 1;

    return ok; 
}

int str_pair_arr_free(str_pair_arr* arr) {
    if (arr == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    for (int i = 0; i < arr->size; i++) {
        free(arr->data[i].name);
        free(arr->data[i].op_am);
    }
    free(arr->data);
    arr->data = NULL;
    arr->max_size = 0;
    arr->size = 0;
}

int strcpy_with_malloc(char** dest, const char* source) {
    int l = strlen(source);
    *dest = (char*)malloc(sizeof(char) * (l+1));

    if (*dest == NULL) {
        return MEMORY_ISSUES;
    }

    strcpy(*dest, source);

    return ok;
}

void swap_ptr(void** a, void** b) {
    void* c = *a;
    *a = *b;
    *b = c;
}

int initiate_application ( 
    Application** a,
    char* arrival_time, 
    int priority, 
    char* text, 
    int id,
    char* dep_id
    ) {

    if (arrival_time == NULL || text == NULL || dep_id == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    *a = (Application*)malloc(sizeof(Application));
    if (*a == NULL) {
        return MEMORY_ISSUES;
    }

    (*a)->arrival_time = arrival_time;
    (*a)->priority = priority;
    (*a)->text = text;
    (*a)->id = id;
    (*a)->dep_id = dep_id;
    
    return ok;
}

int copy_application(Application** dest, Application* source) {
    char* new_arrival_time = NULL, *new_text = NULL, *new_dep_id = NULL;

    int st = ok;

    st = strcpy_with_malloc(&new_arrival_time, source->arrival_time);
    if (st != ok) {
        return st;
    }

    st = strcpy_with_malloc(&new_text, source->text);
    if (st != ok) {
        free(new_arrival_time);
        return st;
    }

    st = strcpy_with_malloc(&new_dep_id, source->dep_id);
    if (st != ok) {
        free(new_arrival_time);
        free(new_text);
        return st;
    }

    return initiate_application(dest, new_arrival_time, source->priority, new_text, source->id, new_dep_id);
}

void free_application(Application* apl) {
    if (apl == NULL) {
        return;
    }
    free(apl->arrival_time);
    free(apl->text);
    free(apl->dep_id);
    free(apl);
}

int compare_applications(const Application* a1, const Application* a2) {
    if (a1->priority != a2->priority) {
        if (a1->priority > a2->priority) {
            return 1;
        }
        else {
            return -1;
        }
    }

    int ans = strcmp(a1->arrival_time, a2->arrival_time);
    if (ans == 0) {
        if (a1->id == a2->id) {
            return 0;
        }
        else {
            return (a1->id > a2->id) ? 1 : -1;
        }
    }
    else {
        return (ans > 0) ? 1 : -1;
    }
}

void print_application(FILE* stream, const Application* a) {
    if (a == NULL) {
        fprintf(stream, "Ptr is NULL!\n");
        return;
    }
    fprintf(stream, "Arrival time: %s\n", a->arrival_time);
    fprintf(stream, "Priority: %d\n", a->priority);
    fprintf(stream, "Text: %s\n", a->text);
    fprintf(stream, "Id: %d\n", a->id);
}

void free_all(unsigned int amount, ...) {
    va_list l;
    va_start(l, amount);

    for (int i = 0; i < amount; i++) {
        void* el = va_arg(l, void*);
        free(el);
    }

    va_end(l);
}

int ctoi(char ch) {
    if (isdigit(ch)) {
        return ch - '0';
    }
    else if (isalpha(ch)) {
        return toupper(ch) - 'A' + 10;
    }
    return -1;
}


int mult_safely(ll arg_1, ll arg_2, ll* res)
{
    if (res == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    if ((arg_1 == LLONG_MIN && arg_2 != 1) || (arg_2 == LLONG_MIN && arg_1 != 1)) {
        return OVERFLOW;
    }
    if (llabs(arg_1) > LLONG_MAX / llabs(arg_2)) {
        return OVERFLOW;
    }
    *res = arg_1 * arg_2;
    return ok;
}

int bpow_safely(ll base, ll pow, ll* res) {
    if (res == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    if (pow < 0) {
        return INVALID_INPUT;
    }
    if (base == 0 && pow == 0) {
        return ZERO_POWERED_ZERO;
    }
    ll res_tmp = 1;
    ll mult = base;
    while (pow > 0) {
        if (pow & 1) {
            int err_code = mult_safely(res_tmp, mult, &res_tmp);
            if (err_code) {
                return err_code;
            }
        }
        mult *= mult;
        pow >>= 1;
    }
    *res = res_tmp;
    return ok;
}

int iso_time_add(char* time, ull add_s, char** res) {
    if (time == NULL || res == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    char time_tmp[21];
    strcpy(time_tmp, time);

    *res = (char*)malloc(sizeof(char) * 21);
    if (*res == NULL) {
        return MEMORY_ISSUES;
    }

    time_tmp[4] = time_tmp[7] = time_tmp[10] = time_tmp[13] = time_tmp[16] = time_tmp[19] = '\0';
    struct tm t = { 0 };
    t.tm_sec = atoi(time_tmp + 17);
    t.tm_min = atoi(time_tmp + 14);
    t.tm_hour = atoi(time_tmp + 11);
    t.tm_mday = atoi(time_tmp + 8);
    t.tm_mon = atoi(time_tmp + 5) - 1;
    t.tm_year = atoi(time_tmp) - 1900;
    
    t.tm_sec += add_s;
    mktime(&t);
    
    (*res)[0] = '0' + ((t.tm_year + 1900) / 1000);
    (*res)[1] = '0' + ((t.tm_year + 1900) / 100 % 10);
    (*res)[2] = '0' + ((t.tm_year + 1900) / 10 % 10);
    (*res)[3] = '0' + ((t.tm_year + 1900) / 1 % 10);
    
    (*res)[5] = '0' + ((t.tm_mon + 1) / 10);
    (*res)[6] = '0' + ((t.tm_mon + 1) % 10);
    
    (*res)[8] = '0' + (t.tm_mday / 10);
    (*res)[9] = '0' + (t.tm_mday % 10);
    
    (*res)[11] = '0' + (t.tm_hour / 10);
    (*res)[12] = '0' + (t.tm_hour % 10);
    
    (*res)[14] = '0' + (t.tm_min / 10);
    (*res)[15] = '0' + (t.tm_min % 10);
    
    (*res)[17] = '0' + (t.tm_sec / 10);
    (*res)[18] = '0' + (t.tm_sec % 10);
    
    (*res)[4] = (*res)[7] = '-';
    (*res)[10] = 'T';
    (*res)[13] = (*res)[16] = ':';
    (*res)[19] = 'Z';
    (*res)[20] = '\0';
    

    return ok;
}


unsigned rand_32()
{
	unsigned x = rand() & 255;
	x |= (rand() & 255) << 8;
	x |= (rand() & 255) << 16;
	x |= (rand() & 255) << 24;
	return x;
}


int parse_ullong(const char* src, int base, ull* number)
{
	if (src == NULL || number == NULL)
	{
		return INVALID_FUNCTION_ARGUMENT;
	}
	if (base < 0 || base == 1 || base > 36)
	{
		return INVALID_BASE;
	}
	if (src[0] == '\0')
	{
		return INVALID_INPUT;
	}
	errno = 0;
	char* ptr;
	*number = strtoull(src, &ptr, base);
	if (*ptr != '\0')
	{
		return INVALID_INPUT;
	}
	if (errno == ERANGE)
	{
		return OVERFLOW;
	}
	return ok;
}