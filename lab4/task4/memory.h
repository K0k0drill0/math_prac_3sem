#ifndef MEMORY_H
#define MEMORY_H

typedef struct MemoryCell {
    char name;
    int val;
} MemoryCell;

typedef struct
{
	MemoryCell** vars;
} Memory;

//memory

int init_memory(Memory* memory);
int init_memory_cell(MemoryCell** cell, const char name, const int val);

int assign_memory_cell(Memory memory, char name, int value);

MemoryCell* get_memory_cell_by_name(Memory memory, char name);

void free_memory_cell(MemoryCell* cell);
void free_memory(Memory mem);

void print_memory_cell(FILE* outp, const MemoryCell* cell);
void print_memory(FILE* outp, const Memory memory);


#endif