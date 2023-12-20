#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Liver.h"
#include "validators.h"

int init_liver(Liver** liver, char* name, char* surname, 
char* last_name, char* birthday, char gender, double wage) {
    *liver = (Liver*)malloc(sizeof(Liver));
    if (*liver == NULL) {
        return MEMORY_ISSUES;
    }

    (*liver)->name = name;
    (*liver)->surname = surname;
    (*liver)->last_name = last_name;
    (*liver)->birthday = birthday;
    (*liver)->gender = gender;
    (*liver)->wage = wage;

    (*liver)->next = NULL;

    return ok;
}

int is_valid_liver(const Liver* liv) {
    if (liv == NULL) {
        return 0;
    }

    return (is_valid_name(liv->name) && is_valid_name(liv->surname) &&
    is_valid_last_name(liv->last_name) && is_valid_date(liv->birthday) &&
    (liv->gender == 'M' || liv->gender == 'W') && liv->wage >= 0.0);
}

int is_valid_liver_for_change(const Liver* liv) {
    if (liv == NULL) {
        return 0;
    }

    return ((is_valid_name(liv->name) || strlen(liv->name) == 0) &&
    (is_valid_name(liv->surname) || strlen(liv->surname) == 0) &&
    is_valid_last_name(liv->last_name) &&
    (is_valid_date(liv->birthday) || strlen(liv->birthday) == 0));
}

int copy_liver(Liver* dest, Liver* source) {
    int ind = 0;
    while (source->surname[ind] != '\0') {
        ind++;
    }
    dest->surname = (char*)malloc(sizeof(char) * (ind + 1));
    if (dest->surname == NULL) {
        return MEMORY_ISSUES;
    }
    strcpy(dest->surname, source->surname);

    ind = 0;
    while (source->name[ind] != '\0') {
        ind++;
    }
    dest->name = (char*)malloc(sizeof(char) * (ind + 1));
    if (dest->name == NULL) {
        free(dest->surname);
        return MEMORY_ISSUES;
    }
    strcpy(dest->name, source->name);

    ind = 0;
    while (source->last_name[ind] != '\0') {
        ind++;
    }
    dest->last_name = (char*)malloc(sizeof(char) * (ind + 1));
    if (dest->last_name == NULL) {
        free(dest->surname);
        free(dest->name);
        return MEMORY_ISSUES;
    }
    strcpy(dest->last_name, source->last_name);

    ind = 0;
    while (source->birthday[ind] != '\0') {
        ind++;
    }
    dest->birthday = (char*)malloc(sizeof(char) * (ind + 1));
    if (dest->birthday == NULL) {
        free(dest->surname);
        free(dest->name);
        free(dest->last_name);
        return MEMORY_ISSUES;
    }
    strcpy(dest->birthday, source->birthday);

    dest->gender = source->gender;
    dest->wage = source->wage;

    return ok;
}

int datecmp(const char* s1, const char* s2) {
    if (s1 == NULL || s2 == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    for (int i = 6; i <= 9; i++) {
        if (s1[i] > s2[i]) {
            return 1;
        }
        else if (s1[i] < s2[i]) {
            return -1;
        }
    }

    for (int i = 3; i <= 4; i++) {
        if (s1[i] > s2[i]) {
            return 1;
        }
        else if (s1[i] < s2[i]) {
            return -1;
        }
    }

    for (int i = 0; i <= 1; i++) {
        if (s1[i] > s2[i]) {
            return 1;
        }
        else if (s1[i] < s2[i]) {
            return -1;
        }
    }

    return 0;
}

int insert_liver(Liver** head, Liver* new) {
    if (new == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    if (*head == NULL) {
        *head = new;
        return ok;
    }
    if (datecmp((*head)->birthday, new->birthday) < 1) {
        new->next = *head;
        *head = new;
        return ok;
    }

    Liver* prev = *head; 
    Liver* tmp = prev->next;
    while (tmp != NULL && datecmp(tmp->birthday, new->birthday) == 1) {
        prev = tmp;
        tmp = tmp->next;
    }

    if (tmp == NULL) {
        prev->next = new;
    }
    else {
        prev->next = new;
        new->next = tmp;
    }
    return ok;
}

int are_equal_livers(const Liver* l, const Liver* example) {
    if (strlen(example->surname) != 0 &&
     strcmp(example->surname, l->surname) != 0) {
        return 0;
    }

    if (strlen(example->name) != 0 &&
     strcmp(example->name, l->name) != 0) {
        return 0;
    }
    if (strlen(example->last_name) != 0 &&
     strcmp(example->last_name, l->last_name) != 0) {
        return 0;
    }
    if (strlen(example->birthday) != 0 &&
     strcmp(example->birthday, l->birthday) != 0) {
        return 0;
    }

    if (example->gender != '\0' && example->gender != l->gender) {
        return 0;
    }

    double eps = 0.000000000000000001;
    if (example->wage > 0.0 && fabs(example->wage - l->wage) > eps) {
        return 0;
    }

    return 1;
}

int find_liver(Liver* data_base, const Liver* needed, Liver** found) {
    if (needed == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    Liver* tmp = data_base;

    while (tmp != NULL && !are_equal_livers(tmp, needed)) {
        tmp = tmp->next;
    }

    *found = tmp;
    return ok;
}

int change_liver(Liver* liv_in_db, Liver* example) {
    if (liv_in_db == NULL || example == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    if (strlen(example->surname) != 0) {
        free(liv_in_db->surname);
        liv_in_db->surname = example->surname;
    }
    else {
        free(example->surname);
    }   

    if (strlen(example->name) != 0) {
        free(liv_in_db->name);
        liv_in_db->name = example->name;
    }
    else {
        free(example->name);
    }  

    if (strlen(example->last_name) != 0) {
        free(liv_in_db->last_name);
        liv_in_db->last_name = example->last_name;
    }
    else {
        free(example->last_name);
    }  

    if (strlen(example->birthday) != 0) {
        free(liv_in_db->birthday);
        liv_in_db->birthday = example->birthday;
    }
    else {
        free(example->birthday);
    }  

    if (example->gender == 'W' || example->gender == 'M') {
        liv_in_db->gender = example->gender;
    }

    if (example->wage >= 0.0) {
        liv_in_db->wage = example->wage;
    }

    free(example);
    return ok;
}

int delete_liver(Liver** data_base, Liver* will_be_deleted) {
    if (*data_base == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    if (*data_base == will_be_deleted) {
        *data_base = will_be_deleted->next;
        free_liver_data(will_be_deleted);
        return ok;
    }

    Liver* prev = *data_base;
    Liver* tmp = prev->next;

    while (tmp != will_be_deleted) {
        prev = tmp;
        tmp = tmp->next;
    }

    prev->next = tmp->next;
    free_liver_data(tmp);
    return ok; 
}

int print_liver(FILE* stream, Liver* liv) {
    if (liv == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    fprintf(stream, "Surname: %s\n", liv->surname);
    fprintf(stream, "Name: %s\n", liv->name);
    fprintf(stream, "Last name: %s\n", liv->last_name);
    fprintf(stream, "Birthday: %s\n", liv->birthday);
    fprintf(stream, "Gender: %c\n", liv->gender);
    fprintf(stream, "Wage: %lf\n", liv->wage);
    return ok;
}

int print_liver_list(FILE* stream, Liver* head) {
    Liver* tmp = head;
    while (tmp != NULL) {
        print_liver(stream, tmp);
        fprintf(stream, "\n");
        tmp = tmp->next;
    }
}

void free_liver_data(Liver* liver) {
    if (liver == NULL) {
        return;
    }
    free(liver->name);
    free(liver->surname);
    free(liver->last_name);
    free(liver->birthday);
    free(liver);
}

void free_liver_list(Liver* liver) {
    if (liver == NULL) {
        return;
    }

    Liver* prev = liver;
    Liver* tmp = prev->next;

    while (tmp != NULL) {
        free_liver_data(prev);
        prev = tmp;
        tmp = tmp->next;
    }

    free_liver_data(prev);
}