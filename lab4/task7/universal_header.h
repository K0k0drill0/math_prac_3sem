#ifndef MEMORY_H
#define MEMORY_H

enum status_codes {
    ok, 
    UNABLE_TO_OPEN_A_FILE,
    INVALID_FILE,
    INVALID_ARGUMENTS,
    MEMORY_ISSUES,
    INVALID_INPUT,
    WRONG_AMOUNT_OF_ARGUMENTS,
    INVALID_ARGUMENT_OF_FUNCTION,
    INVALID_VARNAME,
    INVALID_VALUE,
    OVERFLOWED,

    NONE,
	ASSIGN,
	ADD,
	SUB,
	MULT,
	DIV,
	MOD,
	PRINT
};


typedef struct MemoryCell {
    char* name;
    int val;
} MemoryCell;

typedef struct
{
	int max_size;
	int tmp_size;
	MemoryCell** vars;
} Memory;

//memory
void print_error(const int st);
int is_arithmetic_operation(const char c);

int init_memory(Memory** memory);
int init_memory_cell(MemoryCell** cell, char* name, const int val);

int add_memory_cell(Memory** memory, MemoryCell* cell);

int find_memory_cell_by_name(const Memory* mem, const char* name, MemoryCell** ans);

void sort_memory(Memory* mem);

void free_memory_cell(MemoryCell* cell);
void free_memory(Memory* mem);

void print_memory_cell(const MemoryCell* cell);
void print_memory(const Memory* memory);

//calculations
int is_valid_varname(const char* str);
int is_digit(const char* str);

int op_print(const Memory* mem, const char* str);
int op_assign(Memory** mem, const char* str);

int find_value(Memory* mem, const char* str, int* ans);
int calculate_expression(Memory* mem, char* str, int* ans);

#endif