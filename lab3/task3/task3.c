#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    unsigned int id;
    char* name;
    char* second_name;
    double wage;
} Employee;


enum status_code{
    ok, 
    INVALID_AMOUNT_ARGUMENTS,
    UNABLE_TO_OPEN_FILE,
    INVALID_FLAG,
    MEMORY_ISSUES,
    INVALID_FILE
};

void print_error(const int st) {
    switch (st)
    {
    case INVALID_AMOUNT_ARGUMENTS:
        printf("Invalid amount of arguments!\n");
        break;
    case UNABLE_TO_OPEN_FILE:
        printf("Unable to open a file!\n");
        break;
    case INVALID_FLAG:
        printf("Invalid flag(s)!\n");
        break;
    case MEMORY_ISSUES:
        printf("Unable to allocate the memory!\n");
        break;
    case INVALID_FILE:
        printf("Invalid file!\n");
        break;
    default:
        break;
    }
}

int is_valid_int(const char* num) {
    int ind = 0;
    while (num[ind] != '\0') {
        if (!isdigit(num[ind])) {
            return 0;
        }
        ind++;
    }
    return 1;
}

int is_valid_double(const char* num) {
    int dots_counter = 0;
    int ind = 0;
    while (num[ind] != '\0') {
        if (!isdigit(num[ind])) {
            if (num[ind] == '.' && dots_counter == 0) {
                dots_counter++;
                ind++;
                continue;
            }
            return 0;
        }
        ind++;
    }
    return 1;
}

int is_valid_name(const char* name) {
    int ind = 0;
    while (name[ind] != '\0') {
        if (!isalpha(name[ind])) {
            return 0;
        }
        ind++;
    }
    return 1;
}

void free_everything(const unsigned int amount, ...) {
    va_list args;
    va_start(args, amount);
    for (int i = 0; i < amount; i++) {
        void* ptr = va_arg(args, void*);
        free(ptr);
    }
    va_end(args);
}

void free_arr_employee(Employee* worker, int size) {
    for (int i = 0; i < size; i++) {
        free(worker[i].name);
        free(worker[i].second_name);
    }
    free(worker);
}

int get_string(FILE* inp, char** str) {
    int str_max_size = 16;
    int str_tmp_size = 0;
    *str = (char*)malloc(sizeof(char) * str_max_size);
    if (*str == NULL) {
        return MEMORY_ISSUES;
    }

    char c;
    while ((c = getc(inp)) != '\n' && c != EOF) {
        if (str_tmp_size + 1 == str_max_size) {
            str_max_size *= 2;
            char* tmp = (char*)realloc(*str, sizeof(char) * str_max_size);
            if (tmp == NULL) {
                free(*str);
                return MEMORY_ISSUES;
            }
            *str = tmp;
        }
        (*str)[str_tmp_size] = c;
        str_tmp_size++;
    }
    (*str)[str_tmp_size] = '\0';
    if (c == EOF && strlen(*str) == 0) {
        return EOF;
    }
    return ok;
}

int separate_words(char* line, const unsigned int amount, ...) {
    va_list args;
    va_start(args, amount);

    int ind = 0;
    for (int i = 0; i < amount; i++) {
        int str_max_size = 16;
        int str_tmp_size = 0;

        char** str_tmp = va_arg(args, char**);
        *str_tmp = (char*)malloc(sizeof(char) * str_max_size);
        if (*str_tmp == NULL) {
            return MEMORY_ISSUES;
        } 
        
        while (line[ind] == ' ' || line[ind] == '\t') {
            ind++;
        } 
        if (line[ind] == '\0') {
            return INVALID_FILE;
        }

        while (line[ind] != ' ' && line[ind] != '\t' && line[ind] != '\0') {
            if (str_tmp_size + 1 == str_max_size) {
                str_max_size *= 2;
                char *tmp = (char*)realloc(*str_tmp, sizeof(char) * str_max_size);
                if (tmp == NULL) {
                    free(str_tmp);
                    return MEMORY_ISSUES;
                }
                *str_tmp = tmp;
            }
            (*str_tmp)[str_tmp_size] = line[ind];
            ind++;
            str_tmp_size++;
        }
        (*str_tmp)[str_tmp_size] = '\0';
    }

    va_end(args);
    return ok;
}

int file_reader(FILE* inp, Employee** ans_arr, int* arr_size) {
    int arr_max_size = 16;
    int arr_tmp_size = 0;
    *ans_arr = (Employee*)malloc(sizeof(Employee) * arr_max_size);
    if (*ans_arr == NULL) {
        return MEMORY_ISSUES;
    }

    char* line;
    int st;
    while ((st = get_string(inp, &line)) == ok) {
        char *id, *name, *second_name, *wage;
        st = separate_words(line, 4, &id, &name, &second_name, &wage);
        if (st != ok) {
            free_everything(5, line, id, name, second_name, wage);
            return st;
        }

        if (!is_valid_int(id) || !is_valid_double(wage) || !is_valid_name(name) || !is_valid_name(second_name)) {
            free_everything(5, line, id, name, second_name, wage);
            return INVALID_FILE;
        }

        if (arr_tmp_size > arr_max_size) {
            arr_max_size *= 2;
            Employee* tmp = (Employee*)realloc(*ans_arr, arr_max_size);
            if (tmp == NULL) {
                free_everything(6, line, id, name, second_name, wage, *ans_arr);
                return MEMORY_ISSUES;
            }
            *ans_arr = tmp;
        }
        (*ans_arr)[arr_tmp_size].id = atoi(id);
        (*ans_arr)[arr_tmp_size].name = name;
        (*ans_arr)[arr_tmp_size].second_name = second_name;
        (*ans_arr)[arr_tmp_size].wage = atof(wage);
        arr_tmp_size++;
        free(line);
    }

    if (st == EOF) {
        *arr_size = arr_tmp_size;
        return ok;
    }
    else {
        free(*ans_arr);
        return st;
    }
}

int check_valid_args(int argc, char** argv) {
    if (argc != 4) {
        return INVALID_AMOUNT_ARGUMENTS;
    }
    char* flag = argv[2];
    if ((flag[0] != '-' && flag[0] != '/') || (flag[1] != 'a' && flag[1] != 'd')) {
        return INVALID_FLAG;
    }
    return ok;
}

int compare_employees(const void* a, const void* b) {
    Employee* emp1 = (Employee*)a;
    Employee* emp2 = (Employee*)b;

    // Сортировка по возрастанию заработной платы
    if (emp1->wage < emp2->wage) {
        return -1;
    } else if (emp1->wage > emp2->wage) {
        return 1;
    }

    // Если зарплаты равны, сортировка по фамилии
    int second_name_cmp = strcmp(emp1->second_name, emp2->second_name);
    if (second_name_cmp != 0) {
        return second_name_cmp;
    }

    // Если фамилии равны, сортировка по имени
    int name_cmp = strcmp(emp1->name, emp2->name);
    if (name_cmp != 0) {
        return name_cmp;
    }

    // Если имена равны, сортировка по id
    return emp1->id - emp2->id;
}

void print_employee_arr(FILE* outp, const Employee* arr, const unsigned int size) {
    for (int i = 0; i < size; i++) {
        fprintf(outp, "%d %s %s %lf\n", arr[i].id, arr[i].name, arr[i].second_name, arr[i].wage);
    }
}

int main(int argc, char** argv) {

    // argc = 4;
    // argv[1] = "input.txt";
    // argv[2] = "-d";
    // argv[3] = "output.txt";

    int st;
    st = check_valid_args(argc, argv);
    if (st != ok) {
        print_error(st);
        printf("Usage: <file_inp> flag <flag_outp>\n");
        return 1;
    }

    FILE* inp = fopen(argv[1], "r");
    FILE* outp = fopen(argv[3], "w+");
    if (!inp || !outp) {
        fclose(inp);
        fclose(outp);
        print_error(UNABLE_TO_OPEN_FILE);
        return -1;
    }

    Employee* data_base;
    int data_base_size;
    st = file_reader(inp, &data_base, &data_base_size);
    if (st != ok) {
        free_arr_employee(data_base, data_base_size);
        print_error(st);
        return 1;
    }

    if (argv[2][1] == 'a') {
        qsort(data_base, data_base_size, sizeof(Employee), compare_employees);
    } else if (argv[2][1] == 'd') {
        qsort(data_base, data_base_size, sizeof(Employee), compare_employees);
        for (int i = 0, j = data_base_size - 1; i < j; i++, j--) {
            Employee temp = data_base[i];
            data_base[i] = data_base[j];
            data_base[j] = temp;
        }
    }
    print_employee_arr(outp, data_base, data_base_size);
    free_arr_employee(data_base, data_base_size);
    fclose(inp);
    fclose(outp);
    return 0;
}