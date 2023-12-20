#ifndef ARITHMETIC_H
#define ARITHMETIC_H

int is_operator(const char c);
int priority(const char c);
int is_separator(const char c);
int is_unar(const char c);
int is_allowed_char(const char c);

#endif