#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include "validators.h"
#include "memory.h"

int init_memory(Memory* memory) {
    memory->vars = (MemoryCell**)calloc(26, sizeof(MemoryCell*));
    if (memory == NULL) {
        return MEMORY_ISSUES;
    }
    return ok;
}

int init_memory_cell(MemoryCell** cell, const char name, const int val) {
    *cell = (MemoryCell*)malloc(sizeof(MemoryCell));
    if (*cell == NULL) {
        return MEMORY_ISSUES;
    }
    (*cell)->name = name;
    (*cell)->val = val;
    return ok;
}

int assign_memory_cell(Memory memory, char name, int value) {
    int st = ok;
    if (memory.vars[name - 'A'] == NULL) {
        MemoryCell* new = NULL;
        st = init_memory_cell(&new, name, value);
        if (st != ok) {
            return st;
        }
        memory.vars[name - 'A'] = new;
    }
    else {
        memory.vars[name - 'A']->val = value;
    }
    return ok;
}

MemoryCell* get_memory_cell_by_name(Memory memory, char name) {
    return (memory.vars[name - 'A']);
}

void free_memory_cell(MemoryCell* cell) {
    if (cell == NULL) {
        return;
    }
    free(cell);
}

void free_memory(Memory mem) {
    for (int i = 0; i < 26; i++) {
        free_memory_cell(mem.vars[i]);
    }
    free(mem.vars);
}

void print_memory_cell(FILE* outp, const MemoryCell* cell) {
    if (cell == NULL) {
        return;
    }
    fprintf(outp, "key: %c, value: %d\n", cell->name, cell->val);
}

void print_memory(FILE* outp, const Memory memory) {
    
    int flag = 0;
    for (int i = 0; i < 26; i++) {
        if (memory.vars[i] != NULL) {
            flag = 1;
            print_memory_cell(outp, memory.vars[i]);
        }
    }
    if (!flag) {
        fprintf(outp, "The memory is empty!\n");
    }
}

