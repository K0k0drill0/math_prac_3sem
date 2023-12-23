#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <ctype.h>
#include "lists.h"

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
    case INVALID_STATE:
        printf("Invalid state!\n");
        break;
    case INVALID_COORDS:
        printf("Invalid coordinates!\n");
        break;
    case NO_SUCH_BUS:
        printf("No such bus!\n");
        break;
    default:
        break;
    }
}

int is_separator(char c) {
    return (c == ' ' || c == '\t' || c == '\n');
}

int is_valid_int(char* str) {
    int ind = 0;
    while (str[ind] != '\0') {
        if (!isdigit(str[ind])) {
            return 0;
        }
        ind++;
    }
    return 1;
}

int is_leap_year(const int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int is_valid_date(const char *dateString) {
    if (strlen(dateString) != 10) {
        return 0; 
    }

    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) {
            if (dateString[i] != '.') {
                return 0; 
            }
        } else {
            if (!isdigit(dateString[i])) {
                return 0; 
            }
        }
    }

    int day, month, year;
    sscanf(dateString, "%d.%d.%d", &day, &month, &year);

    if (year < 1000 || year > 9999 || month < 1 || month > 12 || day < 1 || day > 31) {
        return 0; 
    }

    int daysInMonth[] = {31, 28 + is_leap_year(year), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (day > daysInMonth[month - 1]) {
        return 0; 
    }

    return 1; 
}

int is_valid_time(const char *timeString) {
    if (strlen(timeString) != 8) {
        return 0; 
    }

    for (int i = 0; i < 8; i++) {
        if (i == 2 || i == 5) {
            if (timeString[i] != ':') {
                return 0; 
            }
        } else {
            if (!isdigit(timeString[i])) {
                return 0; 
            }
        }
    }

    int hour, minute, second;
    sscanf(timeString, "%d:%d:%d", &hour, &minute, &second);

    if (hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59) {
        return 0; 
    }

    return 1;
}

int is_valid_state(const char* state) {
    return (strlen(state) == 1 && (state[0] == 'S'
     || state[0] == 'F' || state[0] == 'M'));
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

void free_everything(int amount, ...) {
    va_list args;
    va_start(args, amount);
    for (int i = 0; i < amount; i++) {
        void* ptr = va_arg(args, void*);
        free(ptr);
    }
}

int read_stop_from_file(FILE* inp, char** bus_number, Stop** stop, Coords crd) {
    char* num = NULL, *date1 = NULL, *time1 = NULL, *date2 = NULL,
     *time2 = NULL, *state = NULL;

    int st = ok;
    if (((st = get_word_from_file(inp, &num)) != ok) || 
    ((st = get_word_from_file(inp, &date1)) != ok) ||
    ((st = get_word_from_file(inp, &time1)) != ok) ||
    ((st = get_word_from_file(inp, &date2)) != ok) ||
    ((st = get_word_from_file(inp, &time2)) != ok) ||
    ((st = get_word_from_file(inp, &state)) != ok)) {
        free_everything(6, num, date1, time1, date2, time2, state);
        return st;
    }

    *bus_number = num;

    if (!is_valid_date(date1) || !is_valid_time(time1) ||
     !is_valid_date(date2) || !is_valid_time(time2)) {
        free_everything(5, date1, time1, date2, time2, state);
        return INVALID_DATE_OR_TIME;
    }

    char* arrived = (char*)malloc(sizeof(char) * 20);
    if (arrived == NULL) {
        free_everything(5, date1, time1, date2, time2, state);
        return MEMORY_ISSUES;
    }
    sprintf(arrived, "%s %s", date1, time1);

    char* departed = (char*)malloc(sizeof(char) * 20);
    if (departed == NULL) {
        free_everything(5, date1, time1, date2, time2, state);
        return MEMORY_ISSUES;
    }
    sprintf(departed, "%s %s", date2, time2);
    free_everything(4, date1, time1, date2, time2);

    if (!is_valid_state(state)) {
        free_everything(3, arrived, departed, state);
        return INVALID_STATE;
    }

    st = init_stop(stop, arrived, departed, state[0], crd);
    free(state);
    return st;
}

int process_file(FILE* inp, Bus** data_base) {
    int st = ok;
    Stop* new_st = NULL;
    char* bus_number = NULL;
    Coords cd;

    char* x = NULL, *y = NULL;
    st = get_word_from_file(inp, &x);
    if (st != ok) {
        if (st == EOF) {
            return INVALID_COORDS;
        }
        return st;
    }
    st = get_word_from_file(inp, &y);
    if (st != ok) {
        free(x);
        if (st == EOF) {
            return INVALID_COORDS;
        }
        return st;
    }

    if (!is_valid_int(x) || !is_valid_int(y)) {
        free(x); free(y);
        return INVALID_COORDS;
    }

    // отвалидировать координаты, перевести в цифры и записать в структуру
    cd.x = atoi(x), cd.y = atoi(y);
    free(x); free(y);
    if (errno == ERANGE) {
        return OVERFLOWED;
    }

    while ((st = read_stop_from_file(inp, &bus_number, &new_st, cd)) == ok) {
        st = append_stop_in_bus_list(data_base, bus_number, new_st);
        if (st != ok) {
            free_stop(new_st);
            return st;
        }
    }

    if (st == EOF) {
        return ok;
    }
    free(bus_number);
    return st;
}

int create_data_base(Bus** data_base, int argc, char** argv) {
    *data_base = NULL;
    for (int i = 1; i < argc; i++) {
        FILE* inp = fopen(argv[i], "r");
        if (inp == NULL) {
            return UNABLE_TO_OPEN_A_FILE;
        }

        int st = process_file(inp, data_base);
        fclose(inp);
        if (st != ok) {
            return st;
        }
    }
    return ok;
}

void print_menu() {
    printf("\nEnter one of the suggested letters and the app will display a bus, which:\n");
    printf("A. Drove the biggest amount of all routes.\n");
    printf("B. Drove the smallest amount of all routes.\n");
    printf("C. Drove the longest path.\n");
    printf("D. Drove the shortest path.\n");
    printf("E. Drove the longest route.\n");
    printf("F. Drove the shortest route.\n");
    printf("G. Has the biggest downtime\n");
    printf("H. Has the smallest downtime\n");
    printf("I. Stayed at it`s stop the longest.\n");
    printf("J. Stayed at it`s stop the shortest.\n");
    printf("K. Exit the program\n\n");
}

int main(int argc, char** argv) {
    argc = 5;
    //argv[1] = "empty.txt";
    argv[1] = "stop1.txt";
    argv[2] = "stop2.txt";
    argv[3] = "stop3.txt";
    argv[4] = "stop4.txt";


    Bus* data_base = NULL;

    int st = create_data_base(&data_base, argc, argv);
    if (st != ok) {
        free_bus_list(data_base);
        print_error(st);
        return 1;
    }

    print_bus_list(data_base);

    print_menu();

    char* command = NULL;
    while ((st = get_word_from_file(stdin, &command)) == ok) {
        if (strlen(command) == 1 && command[0] >= 'A' && command[0] <= 'K') {
            char* bus_name = NULL;
            if (command[0] == 'A') {
                st = most_routes(data_base, &bus_name);
                if (st != ok && st != NO_SUCH_BUS) {
                    break;
                }
                if (bus_name == NULL) {
                    printf("No such bus!\n");
                }
                else {
                    printf("Bus with the biggest amount of all routes is: %s\n", bus_name);
                }
            }
            else if (command[0] == 'B') {
                st = least_routes(data_base, &bus_name);
                if (st != ok && st != NO_SUCH_BUS) {
                    break;
                }
                if (bus_name == NULL) {
                    printf("No such bus!\n");
                }
                else {
                    printf("Bus with the smallest amount of all routes is: %s\n", bus_name);
                }
            }
            else if (command[0] == 'C') {
                st = longest_path(data_base, &bus_name);
                if (st != ok && st != NO_SUCH_BUS) {
                    break;
                }
                if (bus_name == NULL) {
                    printf("No such bus!\n");
                }
                else {
                    printf("Bus with the longest path is: %s\n", bus_name);
                }
                
            }
            else if (command[0] == 'D') {
                st = shortest_path(data_base, &bus_name);
                if (st != ok && st != NO_SUCH_BUS) {
                    break;
                }
                if (bus_name == NULL) {
                    printf("No such bus!\n");
                }
                else {
                    printf("Bus with the shortest path is: %s\n", bus_name);
                }
                
            }
            else if (command[0] == 'E') {
                st = longest_route(data_base, &bus_name);
                if (st != ok && st != NO_SUCH_BUS) {
                    break;
                }
                if (bus_name == NULL) {
                    printf("No such bus!\n");
                }
                else {
                    printf("Bus with the longest route is: %s\n", bus_name);
                }
                
            }
            else if (command[0] == 'F') {
                st = shortest_route(data_base, &bus_name);
                if (st != ok && st != NO_SUCH_BUS) {
                    break;
                }
                if (bus_name == NULL) {
                    printf("No such bus!\n");
                }
                else {
                    printf("Bus with the shortest route is: %s\n", bus_name);
                }
                
            }
            else if (command[0] == 'G') {
                st = longest_stop(data_base, &bus_name);
                if (st != ok && st != NO_SUCH_BUS) {
                    break;
                }
                if (bus_name == NULL) {
                    printf("No such bus!\n");
                }
                else {
                    printf("Bus with the biggest downtime is: %s\n", bus_name);
                }
                
            }
            else if (command[0] == 'H') {
                st = shortest_stop(data_base, &bus_name);
                if (st != ok && st != NO_SUCH_BUS) {
                    break;
                }
                if (bus_name == NULL) {
                    printf("No such bus!\n");
                }
                else {
                    printf("Bus with the smallest downtime is: %s\n", bus_name);
                }
                
            }
            else if (command[0] == 'I') {
                st = longest_own_stop(data_base, &bus_name);
                if (st != ok && st != NO_SUCH_BUS) {
                    break;
                }
                if (bus_name == NULL) {
                    printf("No such bus!\n");
                }
                else {
                    printf("Bus, which stayed at it`s stop the longest: %s\n", bus_name);
                }
                
            }
            else if (command[0] == 'J') {
                st = shortest_own_stop(data_base, &bus_name);
                if (st != ok && st != NO_SUCH_BUS) {
                    break;
                }
                if (bus_name == NULL) {
                    printf("No such bus!\n");
                }
                else {
                    printf("Bus, which stayed at it`s stop the shortest: %s\n", bus_name);
                }
            }
            else if (command[0] == 'K') {
                break;
            }
        }
        else {
            printf("Incorrect command! Try again.\n");
        }
        print_menu();
        free(command);
    }

    free(command);
    

    free_bus_list(data_base);

    if (st != ok) {
        print_error(st);
    }
    
    return 0;    
}