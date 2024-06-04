#ifndef _OPERATION_SYNTAX_H_
#define _OPERATION_SYNTAX_H_

#include "errors.h"
#include "operation_type.h"
#include "Trie.h"

status_code convert_to_decimal_base(char * to_convert, int original_base, uint32_t * result);

status_code convert_to_base(uint32_t to_convert, int to_base, char ** result);

status_code get_value(Trie* trie, char * name, uint32_t * result, int original_base);

status_code not_operation(Trie* trie, uint32_t value, uint32_t * result);

status_code output_operation(Trie* trie, uint32_t value, int ouput_base);

status_code input_operation(Trie* trie, int input_base, uint32_t * result, char * name);

status_code add_operation(Trie* trie, uint32_t value_1, uint32_t value_2, uint32_t * result);

status_code mult_operation(Trie* trie, uint32_t value_1, uint32_t value_2, uint32_t * result);

status_code sub_operation(Trie* trie, uint32_t value_1, uint32_t value_2, uint32_t * result);

uint32_t fast_mod_pow(uint32_t base, uint32_t exponent, uint32_t modulus);

status_code pow_operation(Trie* trie, uint32_t value_1, uint32_t value_2, uint32_t * result);

status_code div_operation(Trie* trie, uint32_t value_1, uint32_t value_2, uint32_t * result);

status_code rem_operation(Trie* trie, uint32_t value_1, uint32_t value_2, uint32_t * result);

status_code xor_operation(Trie* trie, uint32_t value_1, uint32_t value_2, uint32_t * result);

status_code and_operation(Trie* trie, uint32_t value_1, uint32_t value_2, uint32_t * result);

status_code or_operation(Trie* trie, uint32_t value_1, uint32_t value_2, uint32_t * result);

#endif