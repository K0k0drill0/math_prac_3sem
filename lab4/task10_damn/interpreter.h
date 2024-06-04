#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_

#define _GNU_SOURCE
#include "operation_syntax.h"
#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONFIG_FILE "../__new_config.txt"

status_code  add_to_expression(char ** expression, char * to_add);
status_code  make_unary_expression(Trie* trie, char * variable_name, operation operation_name, uint32_t value, uint32_t * result, int input_base, int output_base, int assign_base);
status_code  make_binary_expression(Trie* trie, operation operation_name, uint32_t value_1, uint32_t value_2, uint32_t * result);
status_code  solve_expression(Current_settings* settings, Trie* trie, char * expression, uint32_t * result, int input_base, int output_base, int assign_base);
status_code  scan_buffer(Current_settings* settings, Trie* trie, char * buffer, int input_base, int output_base, int assign_base);

status_code  free_from_delims(char ** string);
status_code  my_strtok(char ** result, char ** st_string, const char * delim);
void shift_string(char ** string, int index);
status_code  is_delim(char character, const char * delim);

status_code  interpretate(FILE * file, Current_settings* settings, int input_base, int assign_base, int output_base);

#endif