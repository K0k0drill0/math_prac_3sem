#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "validators.h"

int is_valid_udouble(const char* str) {
    int dot_counter = 0;
    int ind = 0;
    while (str[ind] != '\0') {
        if (!isdigit(str[ind])) {
            if (str[ind] == '.' && dot_counter == 0) {
                dot_counter++;
            }
            else {
                return 0;
            }
        }
        ind++;
    }
    return (ind == 0) ? 0 : 1;
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

int is_valid_last_name(const char* name) {
    int ind = 0;
    while (name[ind] != '\0') {
        if (!isalpha(name[ind])) {
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

int is_valid_gender(const char* str) {
    return (strlen(str) == 1 && (str[0] == 'M' || str[0] == 'W'));
}