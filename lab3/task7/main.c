#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "Liver.h"
#include "validators.h"
#include "Undo.h"

void print_error(int st) {
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
    case WRONG_AMOUNT_OF_ARGUMENTS:
        printf("Wrong amount of arguments!\n");
        break;
    case INVALID_FUNCTION_ARGUMENT:
        printf("Invalid function argument!\n");
        break;
    case OVERFLOWED:
        printf("Overflowed!\n");
        break;
    case INVALID_DATE_OR_TIME:
        printf("Invalid date or time!\n");
        break;
    case INVALID_DATA_IN_FILE:
        printf("Invalid data in file!\n");
        break;
    case INVALID_DATA_ABOUT_USER:
        printf("Invalid data about user!\n");
        break;
    case NO_SUCH_USER:
        printf("No such user!\n");
        break;
    case NO_UNDO_ACTION:
        printf("No undo action!\n");
        break;
    default:
        break;
    }
}

int is_separator(char c) {
    return (c == ' ' || c == '\t' || c == '\n'); 
}

int skip_separators(FILE* inp) {
    if (inp == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    char c;
    while (is_separator((c = getc(inp)))) ;
    fseek(inp, -1, SEEK_CUR);
}

int read_line(FILE* inp, char** line) {
    if (inp == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    int arr_max_size = 16;
    *line = (char*)malloc(sizeof(char) * arr_max_size);
    if (*line == NULL) {
        return MEMORY_ISSUES;
    }

    char c = getc(inp);
    while (c == ' ' || c == '\t') {
        c = getc(inp);
    }

    int ind = 0;
    while (c != '\n' && c != EOF) {
        if (ind >= arr_max_size - 3) {
            arr_max_size *= 2;
            char *tmp = (char*)realloc(*line, arr_max_size * sizeof(char));
            if (tmp == NULL) {
                return MEMORY_ISSUES;
            }
            *line = tmp;
        }
        (*line)[ind] = c;
        ind++;
        c = getc(inp);
    }

    (*line)[ind] = '\0';

    if (c == EOF && ind == 0) {
        return EOF;
    }

    return ok;
}

int cut(char* str) {
    if (str == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    int ind = 0;
    while (str[ind] != '\0') {
        ind++;
    }
    if (ind == 0) {
        return ok;
    }
    ind--;

    while (is_separator(str[ind])) {
        ind--;
    }
    ind++;
    str[ind] = '\0';
    return ok;
}

void free_everything(int amount, ...) {
    va_list args;
    va_start(args, amount);
    for (int i = 0; i < amount; i++) {
        void* ptr = va_arg(args, void*);
        free(ptr);
    }
}

int get_liver_from_file(FILE* inp, Liver** new) {
    if (inp == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    int st = ok;
    skip_separators(inp);

    char* surname = NULL, *name = NULL, *last_name = NULL, 
    *birthday = NULL, *gender = NULL, *wage = NULL;

    if ((st = read_line(inp, &surname)) != ok
     || (st = read_line(inp, &name)) != ok 
     || (st = read_line(inp, &last_name)) != ok
     || (st = read_line(inp, &birthday)) != ok 
     || (st = read_line(inp, &gender)) != ok
     || (st = read_line(inp, &wage)) != ok) {
        free_everything(6, surname, name, last_name, birthday, gender, wage);
        return st;
    }

    cut(name); cut(surname); cut(last_name); cut(birthday); cut(gender);
     cut(wage);

    if (!is_valid_name(name) || !is_valid_name(surname) ||
    !is_valid_last_name(last_name) || !is_valid_date(birthday) ||
    !is_valid_gender(gender) || !is_valid_udouble(wage)) {
        free_everything(6, surname, name, last_name, birthday, gender, wage);
        return INVALID_DATA_IN_FILE;
    }

    

    st = init_liver(new, name, surname, last_name, birthday, gender[0], 
    atof(wage));

    if (st != ok) {
        free_everything(6, surname, name, last_name, birthday, gender, wage);
        return st;
    }
    free_everything(2, gender, wage);
    return ok;
}

int process_file(FILE* inp, Liver** data_base) {
    int st = ok;
    Liver* new = NULL;
    while ((st = get_liver_from_file(inp, &new)) == ok) {
        st = insert_liver(data_base, new);
        if (st != ok) {
            return st;
        }
    }

    if (st == EOF) {
        return ok;
    }
    return st;
}

int get_liver_from_user(Liver** liver) {
    char* surname = NULL, *name = NULL, *last_name = NULL, 
    *birthday = NULL, *gender = NULL, *wage = NULL;
    int st = ok;
    
    printf("Input surname:\n");
    st = read_line(stdin, &surname);
    if (st != ok) {
        free_everything(6, surname, name, last_name, birthday, gender, wage);
        return st;
    }

    printf("Input name:\n");
    st = read_line(stdin, &name);
    if (st != ok) {
        free_everything(6, surname, name, last_name, birthday, gender, wage);
        return st;
    }

    printf("Input last name:\n");
    st = read_line(stdin, &last_name);
    if (st != ok) {
        free_everything(6, surname, name, last_name, birthday, gender, wage);
        return st;
    }

    printf("Input birthday:\n");
    st = read_line(stdin, &birthday);
    if (st != ok) {
        free_everything(6, surname, name, last_name, birthday, gender, wage);
        return st;
    }

    printf("Input gender:\n");
    st = read_line(stdin, &gender);
    if (st != ok) {
        free_everything(6, surname, name, last_name, birthday, gender, wage);
        return st;
    }

    printf("Input wage:\n");
    st = read_line(stdin, &wage);
    if (st != ok) {
        free_everything(6, surname, name, last_name, birthday, gender, wage);
        return st;
    }

    if (!is_valid_udouble(wage) && strlen(wage) != 0) {
        free_everything(6, surname, name, last_name, birthday, gender, wage);
        return INVALID_DATA_ABOUT_USER;
    }

    double wg = (strlen(wage) == 0) ? -1.0 : atof(wage);

    st = init_liver(liver, name, surname, last_name, birthday, gender[0],
     wg);

    if (st != ok) {
        free_everything(6, surname, name, last_name, birthday, gender, wage);
        return st;
    }
    free(gender); free(wage);
    return ok;
}

int find_liver_interaction(Liver* data_base, Liver** ans) {
    Liver* example = NULL;
    int st = get_liver_from_user(&example);
    if (st != ok) {
        free_liver_data(example);
        return st;
    }

    char* respond = NULL;
    Liver* found = data_base;
    while (1) {
        st = find_liver(found, example, &found);
        if (st != ok) {
            free_liver_data(example);
            return st;
        }

        if (found == NULL) {
            // printf("No such liver!\n");
            *ans = found;
            break;
        }
        else {
            printf("Were found:\n");
            print_liver(stdout, found);
            printf("Is that your liver? Enter \"Y\", if yes.\n");
            st = read_line(stdin, &respond);
            if (st != ok) {
                free_liver_data(example);
                return st;
            }
            if (strcmp(respond, "Y") == 0) {
                free(respond);
                //free_liver_data(example);
                break;
            }
            free(respond);
        }
        found = found->next;
    }
    free_liver_data(example);
    *ans = found;
    return ok;
}

int add_user_interaction(Liver** data_base, UndoDeque* dq) {
    int st = ok;
    Liver* new = NULL;

    st = get_liver_from_user(&new);
    if (st != ok) {
        return st;
    }

    if (!is_valid_liver(new)) {
        free_liver_data(new);
        return INVALID_DATA_ABOUT_USER;
    }

    st = insert_liver(data_base, new);
    if (st != ok) {
        free_liver_data(new);
    }

    // block for undo
    Action* for_dq = NULL;
    st = init_Action(&for_dq);
    if (st != ok) {
        free_liver_data(new);
        return st;
    }

    Liver* for_dq_lv = NULL;
    st = init_liver(&for_dq_lv, NULL, NULL, NULL, NULL, 'N', 0.0);
    if (st != ok) {
        free_action(for_dq);
        free_liver_data(new);
        return st;
    }

    st = copy_liver(for_dq_lv, new);
    if (st != ok) {
        free_liver_data(new);
        free_liver_data(for_dq_lv);
        free_action(for_dq);
        return st;
    }

    for_dq->now = for_dq_lv;

    st = add_action(dq, for_dq);
    if (st != ok) {
        free_liver_data(new);
        free_liver_data(for_dq_lv);
        free_action(for_dq);
        return st;
    }
    // end_of_block

    return st;
}

int modificate_user_interaction(Liver** data_base, UndoDeque* dq) {
    // Сначала запрашиваем жителя, чтобы найти того, которого нужно модифицировать
    // Начинаем запрашивать жителя с модификациями. Те поля, что останутся пустыми, не будут модифицированы
    // Те, что не пустые, меняем в найденном жителе.

    printf("We need to find your liver at first. Write info about him/her.\n");
    int st = ok;
    Liver* found = NULL;

    st = find_liver_interaction(*data_base, &found);
    if (st != ok) {
        return st;
    }
    if (found == NULL) {
        return NO_SUCH_USER;
    }

    printf("Input new info about user. If you don`t want to modificate field, just press enter.\n");

    Liver* new_liver_data = NULL;
    st = get_liver_from_user(&new_liver_data);
    if (st != ok) {
        free_liver_data(new_liver_data);
        return st;
    }

    if (!is_valid_liver_for_change(new_liver_data)) {
        free_liver_data(new_liver_data);
        return INVALID_DATA_ABOUT_USER;
    }

    Liver* new_liver = NULL;
    st = init_liver(&new_liver, NULL, NULL, NULL, NULL, 'N', 0.0);
    if (st != ok) {
        free_liver_data(new_liver_data);
        return st;
    }
    st = copy_liver(new_liver, found);
    if (st != ok) {
        free_liver_data(new_liver_data);
        free_liver_data(new_liver);
        return st;
    }

    st = change_liver(new_liver, new_liver_data);
    if (st != ok) {
        free_liver_data(new_liver_data);
        free_liver_data(new_liver);
        return st;
    }

    // block for undo
    Action* for_dq = NULL;
    st = init_Action(&for_dq);
    if (st != ok) {
        return st;
    }

    Liver* for_dq_lv = NULL;
    st = init_liver(&for_dq_lv, NULL, NULL, NULL, NULL, 'N', 0.0);
    if (st != ok) {
        free_action(for_dq);
        return st;
    }

    st = copy_liver(for_dq_lv, found);
    if (st != ok) {
        free_liver_data(for_dq_lv);
        free_action(for_dq);
        return st;
    }

    for_dq->then = for_dq_lv;

    Liver* for_dq_lv_now = NULL;
    st = init_liver(&for_dq_lv_now, NULL, NULL, NULL, NULL, 'N', 0.0);
    if (st != ok) {
        free_liver_data(for_dq_lv);
        free_action(for_dq);
        return st;
    }

    st = copy_liver(for_dq_lv_now, new_liver);
    if (st != ok) {
        free_liver_data(for_dq_lv);
        free_liver_data(for_dq_lv_now);
        free_action(for_dq);
        return st;
    }

    for_dq->now = for_dq_lv_now;

    st = add_action(dq, for_dq);
    if (st != ok) {
        free_liver_data(for_dq_lv);
        free_liver_data(for_dq_lv_now);
        free_action(for_dq);
        return st;
    }
    // end_of_block

    st = delete_liver(data_base, found);
    if (st != ok) {
        free_liver_data(new_liver_data);
        free_liver_data(new_liver);
        return st;
    }

    st = insert_liver(data_base, new_liver);
    if (st != ok) {
        free_liver_data(new_liver_data);
        free_liver_data(new_liver);
        return st;
    }

    return ok;
}

int delete_liver_interaction(Liver** data_base, UndoDeque* dq) {
    int st = ok;

    printf("For deleting we need to find a liver at first. Input the info about him/her.\n");

    Liver* found = NULL;
    st = find_liver_interaction(*data_base, &found);
    if (st != ok) {
        return st;
    }

    if (found == NULL) {
        return NO_SUCH_USER;
    }

    // block for undo
    Action* for_dq = NULL;
    st = init_Action(&for_dq);
    if (st != ok) {
        return st;
    }

    Liver* for_dq_lv = NULL;
    st = init_liver(&for_dq_lv, NULL, NULL, NULL, NULL, 'N', 0.0);
    if (st != ok) {
        free_action(for_dq);
        return st;
    }

    st = copy_liver(for_dq_lv, found);
    if (st != ok) {
        free_liver_data(for_dq_lv);
        free_action(for_dq);
        return st;
    }

    for_dq->then = for_dq_lv;

    st = add_action(dq, for_dq);
    if (st != ok) {
        free_liver_data(for_dq_lv);
        free_action(for_dq);
        return st;
    }
    // end_of_block

    return delete_liver(data_base, found);
}

void print_menu() {
    printf("Chose the action:\n");
    printf("F: find liver.\n");
    printf("M: modificate liver.\n");
    printf("A: add liver.\n");
    printf("D: delete liver.\n");
    printf("P: print user in file.\n");
    printf("U: undo.\n");
    printf("E: exit.\n");
}

int user_interaction(Liver** data_base) {
    int st = ok;
    UndoDeque dq;
    st = init_UndoDeque(&dq);
    if (st != ok) {
        return st;
    }

    printf("Welcome to the app!\n");
    print_menu();

    int undodeque_needs_to_be_freed = 0;

    char* request = NULL;
    while ((st = read_line(stdin, &request) == ok)) {
        cut(request);

        char q;
        if (strlen(request) != 1 || (request[0] != 'A' &&
         request[0] != 'D' && request[0] != 'F' &&
         request[0] != 'M' && request[0] != 'P') &&
         request[0] != 'E' && request[0] != 'U') {
            free(request);
            printf("Invalid command! Try again.\n");
            print_menu();
            continue;
        }
        q = request[0];
        free(request);

        if (q != 'U' && q != 'E') {
            if (undodeque_needs_to_be_freed) {
                free_UndoDeque(&dq);
                undodeque_needs_to_be_freed = 0;
            }
        }

        if (q == 'F') {
            Liver* found = NULL;
            st = find_liver_interaction(*data_base, &found);
            if (st != ok && st != INVALID_DATA_ABOUT_USER) {
                free_UndoDeque(&dq);
                return st;
            }
            if (st == INVALID_DATA_ABOUT_USER) {
                print_error(st);
                printf("Try again.\n");
            }
            else if (st == ok && found == NULL) {
                print_error(NO_SUCH_USER);
                printf("Try again.\n");
            }
        }
        else if (q == 'M') {
            st = modificate_user_interaction(data_base, &dq);
            if (st == NO_SUCH_USER || st == INVALID_DATA_ABOUT_USER) {
                print_error(st);
                printf("Try again.\n");
            }
            else if (st != NO_SUCH_USER && st != INVALID_DATA_ABOUT_USER && st != ok) {
                free_UndoDeque(&dq);
                return st;
            }
        }
        else if (q == 'A') {
            st = add_user_interaction(data_base, &dq);
            if (st != ok) {
                free_UndoDeque(&dq);
                return st;
            }
        }
        else if (q == 'D') {
            st = delete_liver_interaction(data_base, &dq);
            if (st == NO_SUCH_USER || st == INVALID_DATA_ABOUT_USER) {
                print_error(st);
                printf("Try again.\n");
            }
            else if (st != NO_SUCH_USER && st != INVALID_DATA_ABOUT_USER && st != ok) {
                free_UndoDeque(&dq);
                return st;
            }
        }
        else if (q == 'P') {
            printf("Input filename:\n");

            char* filename = NULL;
            st = read_line(stdin, &filename);
            if (st != ok) {
                free(filename);
                free_UndoDeque(&dq);
                return st;
            }

            FILE* outp = fopen(filename, "a");
            free(filename);

            if (outp == NULL) {
                print_error(UNABLE_TO_OPEN_A_FILE);
                printf("Try again!\n");
                print_menu();
                continue;
            }

            fseek(outp, 0, SEEK_END);
            fprintf(outp, "________________________________________________________________________\n");
            print_liver_list(outp, *data_base);
            fclose(outp);
        }
        else if (q == 'U') {
            undodeque_needs_to_be_freed = 1;
            st = undo(&dq, data_base);
            if (st != ok && st != NO_UNDO_ACTION) {
                free_UndoDeque(&dq);
                return st;
            }
            if (st == NO_UNDO_ACTION) {
                printf("No undo action!\n");
            }
        }
        else if (q == 'E') {
            free_UndoDeque(&dq);
            return ok;
        }
        print_menu();
    }
}
/*
Модификация жителя
Добавление жителя
Удаление жителя

Поиск жителя с заданными параметрами
Размещение жителя в файл
*/



int main() {

    FILE* inp = fopen("input.txt", "r");
    if (inp == NULL) {
        print_error(UNABLE_TO_OPEN_A_FILE);
        return -1;
    }

    Liver* data_base = NULL;

    int st = process_file(inp, &data_base);
    if (st != ok) {
        print_error(st);
        free_liver_list(data_base);
        fclose(inp);
        return 1;
    }
    fclose(inp);

    print_liver_list(stdout, data_base); 

    st = user_interaction(&data_base);
    if (st != ok) {
        print_error(st);
    }

    free_liver_list(data_base);
    return 0;

}