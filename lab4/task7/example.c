#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <string.h>
#include <math.h>
#include <errno.h>

typedef long long ll;
typedef unsigned long long ull;

typedef enum
{
	OK,
	INVALID_ARG,
	INVALID_INPUT,
	INVALID_FLAG,
	INVALID_NUMBER,
	FILE_OPENING_ERROR,
	FILE_CONTENT_ERROR,
	OVERFLOW,
	UNINITIALIZED_USAGE,
	DIVISION_BY_ZERO,
	BAD_ALLOC,
	CORRUPTED_MEMORY
} status_codes;

void print_error(status_codes code)
{
	switch (code)
	{
		case OK:
			return;
		case INVALID_ARG:
			printf("Invalid function argument\n");
			return;
		case INVALID_INPUT:
			printf("Invalid input\n");
			return;
		case INVALID_FLAG:
			printf("Invalid flag\n");
			return;
		case INVALID_NUMBER:
			printf("Invalid number\n");
			return;
		case FILE_OPENING_ERROR:
			printf("File cannot be opened\n");
			return;
		case FILE_CONTENT_ERROR:
			printf("Invalid content of file\n");
			return;
		case OVERFLOW:
			printf("An overflow occurred\n");
			return;
		case UNINITIALIZED_USAGE:
			printf("Uninitialized variable was used\n");
			return;
		case DIVISION_BY_ZERO:
			printf("Division by zero occurred\n");
			return;
		case BAD_ALLOC:
			printf("Memory lack error occurred\n");
			return;
		case CORRUPTED_MEMORY:
			printf("Memory was corrupted\n");
			return;
		default:
			printf("Unexpected error occurred\n");
			return;
	}
}

typedef struct
{
	char* name;
	ll val;
} Memory_cell;

typedef struct
{
	ull size;
	ull cnt;
	Memory_cell* vars;
} Memory;

status_codes memory_set_null(Memory* memory);
status_codes memory_destruct(Memory* memory);
status_codes memory_insert(Memory* memory, const char* name, ll val);
status_codes memory_search(Memory memory, const char* name, ll* pos);
status_codes memory_get_var(Memory memory, const char* name, ll* val);
status_codes memory_set_var(Memory* memory, const char* name, ll val);
status_codes memory_print_var(Memory memory, const char* name);
status_codes memory_print_all(Memory memory);

status_codes get_arg_value(Memory memory, const char* arg, ll* val);

typedef enum
{
	NONE,
	ASSIGN,
	ADD,
	SUB,
	MULT,
	DIV,
	MOD,
	PRINT
} operation;

status_codes sread_until(const char* src, const char* delims, const char** end_ptr, char** str);
status_codes fread_expr(FILE* file, char** str);
status_codes parse_expr(const char* src, operation* op, ull* arg_cnt, char** arg1, char** arg2, char** arg3);

int main(int argc, char** argv)
{
	if (argc == 1)
	{
		printf("Usage: cmd_path <input>\n");
		return OK;
	}
	if (argc != 2)
	{
		return INVALID_INPUT;
	}
	
	FILE* input = fopen(argv[1], "r");
	FILE* input = fopen("lab4/t07_input", "r");
	if (input == NULL)
	{
		return FILE_OPENING_ERROR;
	}
	
	Memory memory;
	memory_set_null(&memory);
	status_codes err_code = OK;
	
	while(!err_code && !feof(input))
	{
		char* expr = NULL;
		err_code = fread_expr(input, &expr);
		
		operation op;
		ull arg_cnt;
		char* arg1 = NULL;
		char* arg2 = NULL;
		char* arg3 = NULL;
		err_code = err_code ? err_code : parse_expr(expr, &op, &arg_cnt, &arg1, &arg2, &arg3);
		free(expr);
		
		ll val1;
		ll val2;
		switch (op)
		{
			case ASSIGN:
			{
				err_code = err_code ? err_code : get_arg_value(memory, arg2, &val1);
				err_code = err_code ? err_code : memory_set_var(&memory, arg1, val1);
				break;
			}
			case ADD:
			{
				err_code = err_code ? err_code : get_arg_value(memory, arg2, &val1);
				err_code = err_code ? err_code : get_arg_value(memory, arg3, &val2);
				err_code = err_code ? err_code : memory_set_var(&memory, arg1, val1 + val2);
				break;
			}
			case SUB:
			{
				err_code = err_code ? err_code : get_arg_value(memory, arg2, &val1);
				err_code = err_code ? err_code : get_arg_value(memory, arg3, &val2);
				err_code = err_code ? err_code : memory_set_var(&memory, arg1, val1 - val2);
				break;
			}
			case MULT:
			{
				err_code = err_code ? err_code : get_arg_value(memory, arg2, &val1);
				err_code = err_code ? err_code : get_arg_value(memory, arg3, &val2);
				err_code = err_code ? err_code : memory_set_var(&memory, arg1, val1 * val2);
				break;
			}
			case DIV:
			{
				err_code = err_code ? err_code : get_arg_value(memory, arg2, &val1);
				err_code = err_code ? err_code : get_arg_value(memory, arg3, &val2);
				err_code = err_code ? err_code : memory_set_var(&memory, arg1, val1 / val2);
				break;
			}
			case MOD:
			{
				err_code = err_code ? err_code : get_arg_value(memory, arg2, &val1);
				err_code = err_code ? err_code : get_arg_value(memory, arg3, &val2);
				err_code = err_code ? err_code : memory_set_var(&memory, arg1, val1 % val2);
				break;
			}
			case PRINT:
			{
				if (arg_cnt == 1)
				{
					err_code = err_code ? err_code : memory_print_all(memory);
				}
				else
				{
					err_code = err_code ? err_code : memory_print_var(memory, arg2);
				}
			}
			default:
				break;
		}
		free(arg1);
		free(arg2);
		free(arg3);
	}
	fclose(input);
	memory_destruct(&memory);
	if (err_code)
	{
		print_error(err_code);
		return err_code;
	}
}

int is_word(const char* str)
{
	if (str == NULL || str[0] == '\0')
	{
		return 0;
	}
	for (ull i = 0; str[i]; ++i)
	{
		if (!isalpha(str[i]))
		{
			return 0;
		}
	}
	return 1;
}

int is_number(const char* str)
{
	if (str == NULL || str[0] == '\0')
	{
		return 0;
	}
	for (ull i = 0; str[i]; ++i)
	{
		if (!isdigit(str[i]))
		{
			return 0;
		}
	}
	return 1;
}

status_codes memory_set_null(Memory* memory)
{
	if (memory == NULL)
	{
		return INVALID_ARG;
	}
	memory->size = 0;
	memory->cnt = 0;
	memory->vars = NULL;
	return OK;
}

status_codes memory_destruct(Memory* memory)
{
	if (memory == NULL)
	{
		return INVALID_ARG;
	}
	for (ull i = 0; i < memory->cnt; ++i)
	{
		free(memory->vars[i].name);
	}
	free(memory->vars);
	memory->size = 0;
	memory->cnt = 0;
	memory->vars = NULL;
	return OK;
}

int memory_comp(const void* l, const void* r)
{
	Memory_cell* lcell = (Memory_cell*) l;
	Memory_cell* rcell = (Memory_cell*) r;
	return strcmp(lcell->name, rcell->name);
}

status_codes memory_insert(Memory* memory, const char* name, ll val)
{
	if (memory == NULL)
	{
		return INVALID_ARG;
	}
	if (memory->vars == NULL)
	{
		memory->vars = (Memory_cell*) malloc(sizeof(Memory_cell) * 2);
		if (memory->vars == NULL)
		{
			return BAD_ALLOC;
		}
		memory->size = 2;
		memory->cnt = 0;
	}
	if (memory->cnt == memory->size)
	{
		Memory_cell* tmp = (Memory_cell*) realloc(memory->vars, sizeof(Memory_cell) * memory->size * 2);
		if (tmp == NULL)
		{
			return BAD_ALLOC;
		}
		memory->vars = tmp;
		memory->size *= 2;
	}
	Memory_cell cell;
	cell.name = (char*) malloc(sizeof(char) * (strlen(name) + 1));
	if (cell.name == NULL)
	{
		return BAD_ALLOC;
	}
	strcpy(cell.name, name);
	cell.val = val;
	memory->vars[memory->cnt++] = cell;
	qsort(memory->vars, memory->cnt, sizeof(Memory_cell), memory_comp);
	return OK;
}

status_codes memory_search(Memory memory, const char* name, ll* pos)
{
	if (name == NULL || pos == NULL)
	{
		return INVALID_ARG;
	}
	if (memory.cnt == 0)
	{
		*pos = -1;
		return OK;
	}
	ull left = 0;
	ull right = memory.cnt;
	while (left < right)
	{
		ull mid = (left + right) / 2;
		int cmp = strcmp(name, memory.vars[mid].name);
		if (cmp > 0)
		{
			left = mid + 1;
		}
		else if (cmp < 0)
		{
			right = mid;
		}
		else
		{
			*pos = mid;
			return OK;
		}
	}
	*pos = -1;
	return OK;
}

status_codes memory_get_var(Memory memory, const char* name, ll* val)
{
	if (name == NULL || val == NULL)
	{
		return INVALID_ARG;
	}
	ll pos;
	status_codes err_code = memory_search(memory, name, &pos);
	if (err_code)
	{
		return err_code;
	}
	if (pos == -1)
	{
		return UNINITIALIZED_USAGE;
	}
	*val = memory.vars[pos].val;
	return OK;
}

status_codes memory_set_var(Memory* memory, const char* name, ll val)
{
	if (memory == NULL || name == NULL)
	{
		return INVALID_ARG;
	}
	ll pos;
	status_codes err_code = memory_search(*memory, name, &pos);
	if (err_code)
	{
		return err_code;
	}
	if (pos == -1)
	{
		err_code = memory_insert(memory, name, val);
		return err_code;
	}
	memory->vars[pos].val = val;
	return OK;
}

status_codes memory_print_var(Memory memory, const char* name)
{
	if (name == NULL)
	{
		return INVALID_ARG;
	}
	if (!is_word(name))
	{
		return INVALID_INPUT;
	}
	ll pos;
	status_codes err_code = memory_search(memory, name, &pos);
	if (err_code)
	{
		return err_code;
	}
	if (pos == -1)
	{
		return UNINITIALIZED_USAGE;
	}
	printf("%s = %lld\n", memory.vars[pos].name, memory.vars[pos].val);
	return OK;
}

status_codes memory_print_all(Memory memory)
{
	printf("Memory dump:\n");
	if (memory.cnt == 0)
	{
		printf("No variables is unitialized\n");
	}
	for (ull i = 0; i < memory.cnt; ++i)
	{
		printf("| %-30s | %18lld |\n", memory.vars[i].name, memory.vars[i].val);
	}
	return OK;
}

status_codes get_arg_value(Memory memory, const char* arg, ll* val)
{
	if (arg == NULL || val == NULL)
	{
		return INVALID_INPUT;
	}
	if (is_word(arg))
	{
		return memory_get_var(memory, arg, val);
	}
	if (is_number(arg))
	{
		errno = 0;
		ll tmp = strtoll(arg, NULL, 10);
		if (errno == ERANGE)
		{
			*val = LLONG_MAX;
			return OVERFLOW;
		}
		*val = tmp;
		return OK;
	}
	return INVALID_INPUT;
}

status_codes sread_until(const char* src, const char* delims, const char** end_ptr, char** str)
{
	if (src == NULL || str == NULL)
	{
		return INVALID_ARG;
	}
	ull cnt = 0;
	ull size = 2;
	*str = (char*) malloc(sizeof(char) * size);
	if (*str == NULL)
	{
		return BAD_ALLOC;
	}
	
	char flags[256];
	for (ull i = 0; i < 256; ++i)
	{
		flags[i] = 1;
	}
	for (ull i = 0; delims[i]; ++i)
	{
		flags[(int) delims[i]] = 0;
	}
	const char* ptr = src;
	
	while (flags[(int) *ptr] && *ptr != '\0')
	{
		if (cnt + 1 == size)
		{
			size *= 2;
			char* temp_word = realloc(*str, sizeof(char) * size);
			if (temp_word == NULL)
			{
				free(*str);
				return BAD_ALLOC;
			}
			*str = temp_word;
		}
		(*str)[cnt++] = *ptr;
		++ptr;
	}
	(*str)[cnt] = '\0';
	*end_ptr = ptr;
	return OK;
}

status_codes fread_expr(FILE* file, char** str)
{
	if (file == NULL || str == NULL)
	{
		return INVALID_ARG;
	}
	ull cnt = 0;
	ull size = 2;
	*str = (char*) malloc(sizeof(char) * size);
	if (*str == NULL)
	{
		return BAD_ALLOC;
	}
	
	char ch = getc(file);
	int skip_flag = 1;
	while (skip_flag)
	{
		while (ch == ' ' || ch == '\t' || ch == '\n')
		{
			ch = getc(file);
		}
		skip_flag = 0;
		
		if (ch == '%')
		{
			while (ch != '\n' && !feof(file))
			{
				ch = getc(file);
			}
			ch = getc(file);
			skip_flag = 1;
		}
		else if (ch == '[')
		{
			while (ch != ']' && !feof(file))
			{
				ch = getc(file);
			}
			if (ch != ']')
			{
				free(*str);
				return FILE_CONTENT_ERROR;
			}
			ch = getc(file);
			skip_flag = 1;
		}
	}
	
	while (ch != ';' && !feof(file))
	{
		if (cnt + 2 == size)
		{
			size *= 2;
			char* temp_word = realloc(*str, sizeof(char) * size);
			if (temp_word == NULL)
			{
				free(*str);
				return BAD_ALLOC;
			}
			*str = temp_word;
		}
		(*str)[cnt++] = ch;
		ch = getc(file);
	}
	if (ch == ';')
	{
		(*str)[cnt++] = ch;
	}
	(*str)[cnt] = '\0';
	return OK;
}

int is_arithmetic_operation(char ch)
{
	return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%';
}

operation char_to_operation(char ch)
{
	switch (ch)
	{
		case '=':
			return ASSIGN;
		case '+':
			return ADD;
		case '-':
			return SUB;
		case '*':
			return MULT;
		case '/':
			return DIV;
		case '%':
			return MOD;
		default:
			return NONE;
	}
}

status_codes parse_expr(const char* src, operation* op, ull* arg_cnt, char** arg1, char** arg2, char** arg3)
{
	if (src == NULL || op == NULL || arg_cnt == NULL || arg1 == NULL || arg2 == NULL || arg3 == NULL)
	{
		return INVALID_ARG;
	}
	
	if (src[0] == '\0')
	{
		*op = NONE;
		*arg_cnt = 0;
		*arg1 = NULL;
		*arg2 = NULL;
		*arg3 = NULL;
		return OK;
	}
	
	status_codes err_code = OK;
	const char* ptr = src;
	ull arg_cnt_tmp = 0;
	operation op_tmp;
	char* arg1_tmp = NULL;
	char* arg2_tmp = NULL;
	char* arg3_tmp = NULL;
	
	err_code = sread_until(ptr, " =;", &ptr, &arg1_tmp);
	++arg_cnt_tmp;
	
	if (!err_code && !strcmp(arg1_tmp, "print"))
	{
		op_tmp = PRINT;
		if (*ptr == ' ')
		{
			err_code = sread_until(++ptr, ";", &ptr, &arg2_tmp);
			++arg_cnt_tmp;
		}
	}
	else
	{
		err_code = *ptr == '=' ? OK : INVALID_INPUT;
		op_tmp = ASSIGN;
		err_code = err_code ? err_code : sread_until(++ptr, "+-*/%;", &ptr, &arg2_tmp);
		++arg_cnt_tmp;
		if (!err_code && is_arithmetic_operation(*ptr))
		{
			op_tmp = char_to_operation(*ptr);
			err_code = sread_until(++ptr, ";", &ptr, &arg3_tmp);
			++arg_cnt_tmp;
		}
	}
	
	err_code = err_code ? err_code : (*ptr == ';' ? OK : INVALID_INPUT);
	++ptr;
	err_code = err_code ? err_code : (*ptr == '\0' ? OK : INVALID_INPUT);
	
	if (!err_code && arg1_tmp != NULL)
	{
		err_code = is_word(arg1_tmp) ? OK : INVALID_INPUT;
	}
	if (!err_code && arg2_tmp != NULL)
	{
		err_code = (is_word(arg2_tmp) || is_number(arg2_tmp)) ? OK : INVALID_INPUT;
	}
	if (!err_code && arg3_tmp != NULL)
	{
		err_code = (is_word(arg3_tmp) || is_number(arg3_tmp)) ? OK : INVALID_INPUT;
	}
	
	if (err_code)
	{
		free(arg1_tmp);
		free(arg2_tmp);
		free(arg3_tmp);
		return err_code;
	}
	*op = op_tmp;
	*arg_cnt = arg_cnt_tmp;
	*arg1 = arg1_tmp;
	*arg2 = arg2_tmp;
	*arg3 = arg3_tmp;
	return OK;
}