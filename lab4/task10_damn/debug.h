#ifndef _DEBUG_H_
#define _DEBUG_H_

#define _GNU_SOURCE

#include "Trie.h"
#include "operation_syntax.h"
#include <math.h>

status_code print_variables(Trie* trie);
void print_memory_dump(void * ptr, size_t size);
status_code print_variable_and_value(Trie* trie);
status_code change_variable_value(Trie* trie);
status_code insert_new_variable(Trie* trie, char ** var_name);
status_code zeckendorf_to_decimal(char* zeckendorf, uint32_t * res);
status_code insert_new_variable_zekendorf(Trie* trie, char * name);
status_code roman_to_decimal(char* roman, uint32_t * res);
status_code insert_new_variable_rim(Trie* trie, char * name);
status_code remove_variable(Trie* trie, char * variables[], size_t * size);
status_code debugger(Trie* trie, int * work_flag);
status_code add_to_names_array(char *** names, size_t * size, size_t * capacity, char * name);

#endif