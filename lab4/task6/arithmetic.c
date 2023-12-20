#include <stdio.h>
#include <ctype.h>
#include "arithmetic.h"

int is_operator(const char c) {
    char * operators = "&|~-+<=!?>";
    int ind = 0;
    while (operators[ind]) {
        if (operators[ind] == c) {
            return 1;
        }
        ind++;
    }
    return 0;
}

int is_separator(const char c) {
    return (c == ' ' || c == '\t' || c == '\n');
}

int is_unar(const char c) {
    return (c == '~');
}

int priority(const char c) {
    switch (c)
    {
    case '~':
        return 3;
    case '?':
        return 2;
    case '!':
        return 2;
    case '+':
        return 2;
    case '&':
        return 2;
    case '|':
        return 1;
    case '-':
        return 1;
    case '<':
        return 1;
    case '=':
        return 1;
    default:
        return 100;
    }
}

int is_allowed_char(const char c) {
    return (isupper(c) || c == '0' || c == '1');
}