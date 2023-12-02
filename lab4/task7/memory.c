#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include "universal_header.h"

void print_error(const int st) {
    switch (st)
    {
    case UNABLE_TO_OPEN_A_FILE:
        printf("Unable to open a file!\n");
        break;
    case INVALID_FILE:
        printf("Invalid file!\n");
        break;
    case INVALID_ARGUMENTS:
        printf("Invalid argument(s)!\n");
        break;
    case MEMORY_ISSUES:
        printf("Unable to allocate the memory!\n");
        break;
    case INVALID_INPUT:
        printf("Invalid input!\n");
        break;
    case WRONG_AMOUNT_OF_ARGUMENTS:
        printf("Wrong amount of arguments!\n");
        break;
    case INVALID_ARGUMENT_OF_FUNCTION:
        printf("Invalid parameter of function!\n");
        break;
    case NONE:
        printf("No such element(s).\n");
        break;
    case INVALID_VARNAME:
        printf("Invalid varname!\n");
        break;
    case INVALID_VALUE:
        printf("Invalid value!\n");
        break;
    case OVERFLOWED:
        printf("Overflowed!\n\n");
        break;
    default:
        break;
    }
}

int is_arithmetic_operation(const char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '%');
}

int which_operation(const char c) {
    switch(c) {
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

int init_memory(Memory** memory) {
    *memory = (Memory*)malloc(sizeof(Memory));
    if (*memory == NULL) {
        return MEMORY_ISSUES;
    }

    (*memory)->max_size = 16;
    (*memory)->tmp_size = 0;

    (*memory)->vars = (MemoryCell**)malloc(sizeof(MemoryCell*) * (*memory)->max_size);
    if ((*memory)->vars == NULL) {
        free(*memory);
        return MEMORY_ISSUES;
    }
    return ok;
}

int init_memory_cell(MemoryCell** cell, char* name, const int val) {
    if (name == NULL) {
        return INVALID_ARGUMENT_OF_FUNCTION;
    }
    *cell = (MemoryCell*)malloc(sizeof(MemoryCell));
    if (*cell == NULL) {
        return MEMORY_ISSUES;
    }
    (*cell)->name = name;
    (*cell)->val = val;
    return ok;
}

int add_memory_cell(Memory** memory, MemoryCell* cell) {
    if (*memory == NULL || cell == NULL) {
        return INVALID_ARGUMENT_OF_FUNCTION;
    }

    if ((*memory)->tmp_size == (*memory)->max_size) {
        (*memory)->max_size *= 2;
        Memory* tmp = (Memory*)realloc(*memory, (*memory)->max_size); 
        if (tmp == NULL) {
            return MEMORY_ISSUES;
        }
        (*memory) = tmp;
    }

    (*memory)->vars[(*memory)->tmp_size] = cell;
    (*memory)->tmp_size++;

    sort_memory(*memory);
    return ok;
}

// int find_memory_cell_by_name(const Memory* mem, const char* name, MemoryCell** ans) {
//     if (mem == NULL || name == NULL) {
//         return INVALID_ARGUMENT_OF_FUNCTION;
//     }

//     for (int i = 0; i < mem->tmp_size; i++) {
//         if (!strcmp(mem->vars[i]->name, name)) {
//             *ans = mem->vars[i];
//             return ok;
//         }
//     }
//     *ans = NULL;
//     return ok;
// }

int find_memory_cell_by_name(const Memory* mem, const char* name, MemoryCell** ans) {
    if (mem == NULL || name == NULL) {
        return INVALID_ARGUMENT_OF_FUNCTION;
    }

    //print_memory(mem);
    //printf("Trying to find: %s\n\n", name);

    int low, high, middle;
    low = 0;
    high = mem->tmp_size - 1;
    while (low <= high) {
        middle = low + (high - low) / 2;
        if (strcmp(mem->vars[middle]->name, name) > 0) {
            high = middle - 1;
        }
        else if (strcmp(mem->vars[middle]->name, name) < 0) {
            low = middle + 1;
        }
        else {
            *ans = mem->vars[middle];
            return ok;
        }
    }
    *ans = NULL;
    return ok;
}

int sort_memory_comparator (const void* v1, const void* v2) {
    const MemoryCell *memCellA = *((const MemoryCell **)v1);
    const MemoryCell *memCellB = *((const MemoryCell **)v2);
    return strcmp(memCellA->name, memCellB->name);
}

void sort_memory(Memory* mem) {
    qsort(mem->vars, mem->tmp_size, sizeof(MemoryCell*), sort_memory_comparator);
}

void free_memory_cell(MemoryCell* cell) {
    if (cell == NULL) {
        return;
    }
    free(cell->name);
    free(cell);
}

void free_memory(Memory* mem) {
    if (mem == NULL) {
        return;
    }
    for (int i = 0; i < mem->tmp_size; i++) {
        free_memory_cell(mem->vars[i]);
    }
    free(mem->vars);
    free(mem);
}

void print_memory_cell(const MemoryCell* cell) {
    if (cell == NULL) {
        return;
    }
    printf("key: %s, value: %d\n", cell->name, cell->val);
}

void print_memory(const Memory* memory) {
    if (memory == NULL) {
        return;
    }
    if (memory->tmp_size == 0) {
        printf("The memory is empty!\n");
        return;
    }
    for (int i = 0; i < memory->tmp_size; i++) {
        print_memory_cell(memory->vars[i]);
    }
}

