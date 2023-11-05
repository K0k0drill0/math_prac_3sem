#include <stdio.h>
#include <ctype.h>
#include "Validations.h"

int is_valid_udouble(char* str) {
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

int is_valid_uint(char* str) {
    int ind = 0;
    while (str[ind] != '\0') {
        if (!isdigit(str[ind])) {
            return 0;
        }
        ind++;
    }
    return (ind == 0) ? 0 : 1;
}

int is_valid_recipient_ind(char* str) {
    int ind = 0;
    while (str[ind] != '\0') {
        if (!isdigit(str[ind])) {
            return 0;
        }
        ind++;
    }
    return (ind == 6) ? 1 : 0;
}

int is_valid_mail_id(char* str) {
    int ind = 0;
    while (str[ind] != '\0') {
        if (!isdigit(str[ind])) {
            return 0;
        }
        ind++;
    }
    return (ind == 14) ? 1 : 0;
}