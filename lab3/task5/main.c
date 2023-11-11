#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    unsigned int id;
    char* name;
    char* surname;
    char* group;
    unsigned char* grades;
} Student;

enum status_codes {
    ok, 
    UNABLE_TO_OPEN_A_FILE,
    INVALID_FILE,
    INVALID_ARGUMENTS,
    MEMORY_ISSUES,
    INVALID_INPUT
};

void print_error(const int st);
void student_create(Student* stud, unsigned int id, char* name, char* surname,
 char* group, unsigned char* grades);
void student_free(Student* stud);
int get_student_from_file(FILE* inp, Student* stud);
int is_separator(const char c);
int get_word_from_file(FILE* inp, char** str_inp);
void free_everything(const unsigned int amount, ...);
int is_valid_uint(const char* str);
int is_valid_grades(const char* grades);
int is_valid_name(const char* name);
int create_students_arr(FILE* inp, Student** students_arr,
 int* students_arr_tmp_size, int* students_arr_max_size);
void print_students_arr(FILE* trass, const Student* arr, const unsigned int size, void (*printer)(FILE*, Student));
void print_student(FILE* trass, const Student stud);
void free_students_arr(Student* arr, unsigned int size);
int are_equal_id(const Student stud, const void* id);
int are_equal_name(const Student stud, const void* name);
int are_equal_surname(const Student stud, const void* surname);
int are_equal_group(const Student stud, const void* group);
int find_by_param(const Student* students_arr, const unsigned int students_arr_tmp_size,
Student** result, int* res_size, int* res_max_size, const void* info, int (*comp)(const Student, const void*));
int find_from_user(FILE* trass, const Student* students_arr, const unsigned int students_arr_tmp_size);
int student_copy(Student* s1, const Student* s2);
int compare_by_id(const void* s1, const void* s2);
int compare_by_name(const void* s1, const void* s2);
int compare_by_surname(const void* s1, const void* s2);
int compare_by_group(const void* s1, const void* s2);
int sort_from_user(FILE* trass, Student* students_arr, const unsigned int students_arr_tmp_size);
void print_menu();
void print_students_with_good_grades(FILE* trass, const Student* students_arr, const unsigned int students_arr_tmp_size);
void print_for_find(FILE* trass, const Student stud);
void print_for_good_stud(FILE* trass, const Student stud);


int main(int argc, char** argv) {
    if (argc != 3) {
        print_error(INVALID_ARGUMENTS);
        return 1;
    }

    FILE* inp = fopen(argv[1], "r");
    if (!inp) {
        print_error(UNABLE_TO_OPEN_A_FILE);
        return -1;
    }
    FILE* outp = fopen(argv[2], "w+");
    if (!outp) {
        fclose(inp);
        print_error(UNABLE_TO_OPEN_A_FILE);
        return -1;
    }

    Student* students_arr;
    int students_arr_tmp_size = 0;
    int students_arr_max_size = 16;
    students_arr = (Student*)malloc(sizeof(Student) * students_arr_max_size);
    if (students_arr == NULL) {
        print_error(MEMORY_ISSUES);
        return 1;
    }

    int st = create_students_arr(inp, &students_arr, &students_arr_tmp_size,
     &students_arr_max_size);
    if (st != ok) {
        print_error(st);
        free_students_arr(students_arr, students_arr_tmp_size);
        fclose(inp);
        return 1;
    }

    char* action = NULL;
    //print_menu();
    int flag = 1;
    st = ok;
    while (st == ok || st == INVALID_INPUT) {
        print_menu();
        free(action);
        st = get_word_from_file(stdin, &action);
        if (st != ok) break;
        // if (!flag) {
        //     print_menu();
        // }
        // flag = 0;

        if (strlen(action) != 1) {
            print_error(INVALID_INPUT);
            continue;
        }

        if (action[0] == '1') {
            fprintf(outp, "Searching...\n");
            st = find_from_user(outp, students_arr, students_arr_tmp_size);
            if (st != ok) {
                if (st == INVALID_INPUT) {
                    print_error(INVALID_INPUT);
                    continue;
                }
                break;
            }
        }
        else if (action[0] == '2') {
            fprintf(outp, "Sorting...\n");
            st = sort_from_user(outp, students_arr, students_arr_tmp_size);
            if (st != ok) {
                if (st == INVALID_INPUT) {
                    print_error(INVALID_INPUT);
                    continue;
                }
                break;
            }
        }
        else if (action[0] == '3') {
            fprintf(outp, "Printing...\n");
            print_students_with_good_grades(outp, students_arr, students_arr_tmp_size);
        }
        else if (action[0] == '0') {
            break;
        }
    }

    if (st != ok) {
        print_error(st);
        free(action);
        free_students_arr(students_arr, students_arr_tmp_size);
        fclose(inp);
        fclose(outp);
        return 1;
    }

    free(action);
    free_students_arr(students_arr, students_arr_tmp_size);
    fclose(inp);
    fclose(outp);

    return 0;
}



void print_menu() {
    printf("Options:\n");
    printf("1. Find student by parameter.\n"); //Находит студента. Далее спрашиваем, хочет
    // ли пользователь получить вывод в трассировочный файл. Если да, то выводим
    printf("2. Sort students by parameter.\n"); // Не спрашивая, выводим отсортированный массив
    // в файлик
    printf("3. Print in file students, that average grade is better, than average grade among all students.\n");
    printf("0. Exit.\n");
    printf("\n");
    // printf("\n");
}

void print_students_with_good_grades(FILE* trass, const Student* students_arr, const unsigned int students_arr_tmp_size) {
    if (students_arr_tmp_size == 0) {
        fprintf(trass, "The array is empty!\n");
        return;
    }
    double grades_sum = 0.0;
    for (int i = 0; i < students_arr_tmp_size; i++) {
        for (int j = 0; j < 5; j++) {
            grades_sum += students_arr[i].grades[j] - '0';
        }
    }
    grades_sum /= ((double)students_arr_tmp_size * 5.0);

    for (int i = 0; i < students_arr_tmp_size; i++) {
        double tmp_aver = 0.0;
        for (int j = 0; j < 5; j++) {
            tmp_aver += students_arr[i].grades[j] - '0';
        }
        tmp_aver /= 5;
        if (tmp_aver > grades_sum) {
            print_student(trass, students_arr[i]);
        }
    }
}

int sort_from_user(FILE* trass, Student* students_arr, const unsigned int students_arr_tmp_size) {
    printf("Input needed parameter!\n");
    char* q = NULL;
    int st = get_word_from_file(stdin, &q);
    if (st != ok) {
        free(q);
        return st;
    }

    if (!strcmp(q, "id")) {
        qsort(students_arr, students_arr_tmp_size, sizeof(Student), compare_by_id);
    }
    else if (!strcmp(q, "name")) {
        qsort(students_arr, students_arr_tmp_size, sizeof(Student), compare_by_name);
    }
    else if (!strcmp(q, "surname")) {
        qsort(students_arr, students_arr_tmp_size, sizeof(Student), compare_by_surname);
    }
    else if (!strcmp(q, "group")) {
        qsort(students_arr, students_arr_tmp_size, sizeof(Student), compare_by_group);
    }
    else {
        free(q);
        return INVALID_INPUT;
    }

    print_students_arr(trass, students_arr, students_arr_tmp_size, print_student);
    free(q);
    return ok;
}

int find_from_user(FILE* trass, const Student* students_arr, const unsigned int students_arr_tmp_size) {
    printf("Input needed parameter!\n");
    char* q = NULL;
    int st = get_word_from_file(stdin, &q);
    if (st != ok) {
        free(q);
        return st;
    }

    Student* all_found = NULL;
    int all_found_tmp_size = 0;
    int all_found_max_size = 16;
    all_found = (Student*)malloc(sizeof(Student) * all_found_max_size);
    if (all_found == NULL) {
        free(q);
        return MEMORY_ISSUES;
    }

    printf("Input the key!\n");
    char* key = NULL, *end;
    st = get_word_from_file(stdin, &key);
    if (st != ok) {
        free(q);
        free(key);
        free_students_arr(all_found, all_found_tmp_size);
        return st;
    }

    if (!strcmp(q, "id")) {
        if (!is_valid_uint(key)) {
            free(q);
            free(key);
            free_students_arr(all_found, all_found_tmp_size);
            return INVALID_INPUT;
        }
        unsigned int n = strtol(key, &end, 10);
        st = find_by_param(students_arr, students_arr_tmp_size, &all_found, &all_found_tmp_size, &all_found_max_size, &n, are_equal_id);
        if (st != ok) {
            free(q);
            free(key);
            free_students_arr(all_found, all_found_tmp_size);
            return st;
        }
    }
    else if (!strcmp(q, "name")) {
        st = find_by_param(students_arr, students_arr_tmp_size, &all_found, &all_found_tmp_size, &all_found_max_size, key, are_equal_name);
        if (st != ok) {
            free(q);
            free(key);
            free_students_arr(all_found, all_found_tmp_size);
            return st;
        }
    }
    else if (!strcmp(q, "surname")) {
        st = find_by_param(students_arr, students_arr_tmp_size, &all_found, &all_found_tmp_size, &all_found_max_size, key, are_equal_surname);
        if (st != ok) {
            free(q);
            free(key);
            free_students_arr(all_found, all_found_tmp_size);
            return st;
        }
    }
    else if (!strcmp(q, "group")) {
        st = find_by_param(students_arr, students_arr_tmp_size, &all_found, &all_found_tmp_size, &all_found_max_size, key, are_equal_group);
        if (st != ok) {
            free(q);
            free(key);
            free_students_arr(all_found, all_found_tmp_size);
            return st;
        }
    }
    else {
        free(q);
        free(key);
        free_students_arr(all_found, all_found_tmp_size);
        return INVALID_INPUT;
    }

    printf("All students were found! Do you want to print them in file?\n");
    printf("Input Y, if yes.\n");
    char* ans = NULL;
    st = get_word_from_file(stdin, &ans);
    if (st != ok) {
        free(q);
        free(key);
        free(ans);
        free_students_arr(all_found, all_found_tmp_size);
        return st;
    }
    if (strcmp(ans, "Y") == 0) {
        //print_for_find(trass, )
        print_students_arr(trass, all_found, all_found_tmp_size, print_for_find);
    }
    
    free(q);
    free(key);
    free(ans);
    free_students_arr(all_found, all_found_tmp_size);
    return ok;
}

int find_by_param(const Student* students_arr, const unsigned int students_arr_tmp_size,
Student** result, int* res_size, int* res_max_size, const void* info, int (*comp)(const Student, const void*)) {
    for (int i = 0; i < students_arr_tmp_size; i++) {
        if (comp(students_arr[i], info)) {
            if (*res_size == *res_max_size) {
                *res_max_size *= 2;
                Student* tmp = (Student*)realloc(*result, sizeof(Student) * *res_max_size);
                if (tmp == NULL) {
                    return MEMORY_ISSUES;
                }
                *result = tmp;
            }
            int st = student_copy(&((*result)[*res_size]), &students_arr[i]);
            if (st != ok) {
                return MEMORY_ISSUES;
            }
            //memcpy(&((*result)[*res_size]), &students_arr[i], sizeof(Student));
            //(*result)[*res_size] = students_arr[i];
            (*res_size)++;
        }
    }
    return ok;
}

int are_equal_id(const Student stud, const void* id) {
    unsigned int *el = (unsigned int*)id;
    return (stud.id == *el) ? 1 : 0;
}

int are_equal_name(const Student stud, const void* name) {
    char* name_str = (char*)name;
    return (strcmp(name_str, stud.name) == 0) ? 1 : 0;
}

int are_equal_surname(const Student stud, const void* surname) {
    char* name_str = (char*)surname;
    return (strcmp(name_str, stud.surname) == 0) ? 1 : 0;
}

int are_equal_group(const Student stud, const void* group) {
    char* name_str = (char*)group;
    return (strcmp(name_str, stud.group) == 0) ? 1 : 0;
}
 
int compare_by_id(const void* s1, const void* s2) {
    return ((Student*)s1)->id - ((Student*)s2)->id;
}

int compare_by_surname(const void* s1, const void* s2) {
    return strcmp(((Student*)s1)->surname, ((Student*)s2)->surname);
}

int compare_by_name(const void* s1, const void* s2) {
    return strcmp(((Student*)s1)->name, ((Student*)s2)->name);
}

int compare_by_group(const void* s1, const void* s2) {
    return strcmp(((Student*)s1)->group, ((Student*)s2)->group);
}

int is_separator(const char c) {
    return (c == ' ' || c == '\t' || c == '\n'); 
}

int get_word_from_file(FILE* inp, char** str_inp) {
    int arr_max_size = 16;
    *str_inp = (char*)malloc(sizeof(char) * arr_max_size);
    if (*str_inp == NULL) {
        return MEMORY_ISSUES;
    }

    char c;
    c = getc(inp);
    int ind = 0;
    while (is_separator(c) && c != EOF) {
        c = getc(inp);
    }

    if (c == EOF) {
        //free(*str_inp);
        return EOF;
    }

    while (!is_separator(c) && c != EOF) {
        if (ind >= arr_max_size - 1) {
            arr_max_size *= 2;
            char *tmp = (char*)realloc(*str_inp, arr_max_size * sizeof(char));
            if (tmp == NULL) {
                //free(str_inp);
                return MEMORY_ISSUES;
            }
            *str_inp = tmp;
        }
        (*str_inp)[ind] = c;
        ind++;
        c = getc(inp);
    }
    
    (*str_inp)[ind] = '\0';
    return ok;
}

int get_student_from_file(FILE* inp, Student* stud) {
    char* id = NULL, *name = NULL, *surname = NULL, *group = NULL, *grades = NULL, *end;

    int st = get_word_from_file(inp, &id);
    if (st != ok) {
        free_everything(5, id, name, surname, group, grades);
        return st;
    }

    st = get_word_from_file(inp, &name);
    if (st != ok) {
        free_everything(5, id, name, surname, group, grades);
        if (st == EOF) {
            return INVALID_FILE;
        }
        return st;
    }

    st = get_word_from_file(inp, &surname);
    if (st != ok) {
        free_everything(5, id, name, surname, group, grades);
        if (st == EOF) {
            return INVALID_FILE;
        }
        return st;
    }

    st = get_word_from_file(inp, &group);
    if (st != ok) {
        free_everything(5, id, name, surname, group, grades);
        if (st == EOF) {
            return INVALID_FILE;
        }
        return st;
    }

    st = get_word_from_file(inp, &grades); // оценки должны быть поданы слитно, без разделителей
    if (st != ok) {
        free_everything(5, id, name, surname, group, grades);
        if (st == EOF) {
            return INVALID_FILE;
        }
        return st;
    }

    if (!is_valid_uint(id) || !is_valid_grades(grades) || !is_valid_name(name)
     || !is_valid_name(surname)) {
        free_everything(5, id, name, surname, group, grades);
        return INVALID_FILE;
    }

    unsigned int n = strtoul(id, &end, 10);
    free(id);
    student_create(stud, n, name, surname, group, grades);
    return ok;
}

int create_students_arr(FILE* inp, Student** students_arr,
 int* students_arr_tmp_size, int* students_arr_max_size) {
    //char c = getc(inp);
    int st = ok;
    Student stud_tmp;
    student_create(&stud_tmp, 0, NULL, NULL, NULL, NULL);
    while ((st = get_student_from_file(inp, &stud_tmp)) == ok) {
        if (*students_arr_tmp_size == *students_arr_max_size) {
            *students_arr_max_size *= 2;
            Student* tmp = (Student*)realloc(*students_arr, (*students_arr_max_size * sizeof(Student)));
            if (tmp == NULL) {
                student_free(&stud_tmp);
                return MEMORY_ISSUES;
            }
            *students_arr = tmp;
        }
        (*students_arr)[*students_arr_tmp_size] = stud_tmp;
        (*students_arr_tmp_size)++;
        student_create(&stud_tmp, 0, NULL, NULL, NULL, NULL);
    }
    if (st != EOF) {
        student_free(&stud_tmp);
        return st;
    }
    student_free(&stud_tmp);
    return ok;
}

void student_create(Student* stud, unsigned int id, char* name, char* surname,
 char* group, unsigned char* grades) {
    stud->id = id;
    stud->name = name;
    stud->surname = surname;
    stud->group = group;
    stud->grades = grades;
}

int student_copy(Student* s1, const Student* s2) {
    s1->id = s2->id;
    s1->name = (char*)malloc(sizeof(char) * (strlen(s2->name) + 1));
    if (s1->name == NULL) {
        return MEMORY_ISSUES;
    }

    s1->surname = (char*)malloc(sizeof(char) * (strlen(s2->surname) + 1));
    if (s1->surname == NULL) {
        free(s1->name);
        return MEMORY_ISSUES;
    }

    s1->group = (char*)malloc(sizeof(char) * (strlen(s2->group) + 1));
    if (s1->group == NULL) {
        free(s1->name);
        free(s1->surname);
        return MEMORY_ISSUES;
    }

    s1->grades = (char*)malloc(sizeof(char) * (strlen(s2->grades) + 1));
    if (s1->grades == NULL) {
        free(s1->name);
        free(s1->surname);
        free(s1->group);
        return MEMORY_ISSUES;
    }

    s1->name = strcpy(s1->name, s2->name);
    s1->surname = strcpy(s1->surname, s2->surname);
    s1->group = strcpy(s1->group, s2->group);
    s1->grades = strcpy(s1->grades, s2->grades);
    return ok;
}

void student_free(Student* stud) {
    free(stud->name);
    free(stud->surname);
    free(stud->group);
    free(stud->grades);
}

void free_everything(const unsigned amount, ...) {
    va_list args;
    va_start(args, amount);
    for (int i = 0; i < amount; i++) {
        void* ptr = va_arg(args, void*);
        free(ptr);
    }
}

void free_students_arr(Student* arr, unsigned int size) {
    for (int i = 0; i < size; i++) {
        student_free(&(arr[i]));
    }
    free(arr);
}

void print_error(const int st) {
    switch (st)
    {
    case UNABLE_TO_OPEN_A_FILE:
        printf("Unable to open a file!\n");
        break;
    case INVALID_FILE:
        printf("Invalid file!\n");
        break;
    case INVALID_ARGUMENTS:
        printf("Invalid argument(s)!\n");
        break;
    case MEMORY_ISSUES:
        printf("Unable to allocate the memory!\n");
        break;
    case INVALID_INPUT:
        printf("Invalid input!\n");
        break;
    default:
        break;
    }
}

void print_students_arr(FILE* trass, const Student* arr, const unsigned int size, void (*printer)(FILE*, Student)) {
    if (size == 0) {
        fprintf(trass, "The array is empty!\n");
    }
    for (int i = 0; i < size; i++) {
        printer(trass, arr[i]);
    }
}

void print_student(FILE* trass, const Student stud) {
    fprintf(trass, "Students characteristic:\n");
    fprintf(trass, "id: %u\n", stud.id);
    fprintf(trass, "name: %s\n", stud.name);
    fprintf(trass, "surname: %s\n", stud.surname);
    fprintf(trass, "group: %s\n", stud.group);
    fprintf(trass, "grades: ");
    for (int i = 0; i < 5; i++) {
        fprintf(trass, "%c", stud.grades[i]);
    }
    fprintf(trass, "\n\n");
}

void print_for_find(FILE* trass, const Student stud) {
    fprintf(trass, "name: %s\n", stud.name);
    fprintf(trass, "surname: %s\n", stud.surname);
    fprintf(trass, "group: %s\n", stud.group);
    double av_gr = 0.0;
    for (int i = 0; i < 5; i++) {
        av_gr += stud.grades[i] - '0';
    }
    av_gr /= 5.0;
    fprintf(trass, "average grade: %lf\n", av_gr);
}

void print_for_good_stud(FILE* trass, const Student stud) {
    printf("name: %s\n", stud.name);
    printf("surname: %s\n", stud.surname);
}

int is_valid_uint(const char* str) {
    int ind = 0;
    while (str[ind] != '\0') {
        if (!isdigit(str[ind])) {
            return 0;
        }
        ind++;
    }
    return (ind == 0) ? 0 : 1;
}

int is_valid_grades(const char* grades) {
    int ind = 0;
    while (grades[ind] != '\0') {
        if (!isdigit(grades[ind]) || grades[ind] < '2' || grades[ind] > '5') {
            return 0;
        }
        ind++;
    }
    return (ind == 5) ? 1 : 0;
}

int is_valid_name(const char* name) {
    int ind = 0;
    while (name[ind] != '\0') {
        if (!isalpha(name[ind])) {
            return 0;
        }
        ind++;
    }
    return (ind == 0) ? 0 : 1;
}
