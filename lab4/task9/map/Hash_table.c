#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>

#include "Hash_table.h"
#include "../errors.h"

#define DEFAULT_HASH_PARAM 37

typedef unsigned long long ull;

ull calc_default_str_hash(char* str)
{
    if (str == NULL)
    {
        return 0;
    }
    ull res = 0;
    for (ull i = 0; str[i]; ++i)
    {
        res *= DEFAULT_HASH_PARAM;
        res += ctoi(str[i]) + 1;
    }
    return res;
}

int Hash_set_set_null(Hash_set* hs) {
	if (hs == NULL) {
		return INVALID_FUNCTION_ARGUMENT;
	}
    
    hs->calc_hash = NULL;
	hs->hs_size = 0;
	hs->chain_sizes = NULL;
	hs->chains = NULL;
	hs->max_chain_size = 0;
	hs->max_chain_cnt = 0;
	hs->min_chain_size = 0;
	hs->min_chain_cnt = 0;
    
	return ok;
}

int Hash_set_init(Hash_set* hs) {
	if (hs == NULL) {
		return INVALID_FUNCTION_ARGUMENT;
	}
    
	hs->chains = (hs_node**) calloc(MIN_HS_SIZE, sizeof(hs_node*));
	hs->chain_sizes = (size_t*) calloc(MIN_HS_SIZE, sizeof(ull));
    
	if (hs->chains == NULL || hs->chain_sizes == NULL) {
		free_all(2, hs->chains, hs->chain_sizes);
        Hash_set_set_null(hs);
		return MEMORY_ISSUES;
	}
    
	hs->calc_hash = calc_default_str_hash;
	hs->hs_size = MIN_HS_SIZE;
	hs->max_chain_size = hs->min_chain_size = 0;
	hs->max_chain_cnt = hs->min_chain_cnt = MIN_HS_SIZE;
    
	return ok;
}

int Hash_set_free(Hash_set* hs) {
	if (hs == NULL) {
		return INVALID_FUNCTION_ARGUMENT;
	}
    
    for (size_t i = 0; i < hs->hs_size; ++i)
    {
        hs_node* node = hs->chains[i];
        while (node != NULL)
        {
            hs_node* nxt = node->next;
			if (node->key != node->dep->name) {
				free(node->key);
			}
            Department_free(node->dep);
			//node->dep = NULL;
			
            free(node);
            node = nxt;
        }
    }
    
	free_all(2, hs->chains, hs->chain_sizes);
    Hash_set_set_null(hs);
    
	return ok;
}


int Hash_set_rebuild(Hash_set* hs, ull param) {
	if (hs == NULL) {
		return INVALID_FUNCTION_ARGUMENT;
	}
    
	if (hs->hs_size > MAX_HS_SIZE / param) {
		return MEMORY_ISSUES;
	}
    
	size_t new_size = (hs->hs_size + 1) * param - 1;
	hs_node** new_chains = (hs_node**) calloc(new_size, sizeof(hs_node*));
	size_t* new_chain_sizes = (size_t*) calloc(new_size, sizeof(size_t));
    
	if (new_chains == NULL || new_chain_sizes == NULL) {
		free_all(2, new_chains, new_chain_sizes);
		return MEMORY_ISSUES;
	}
    
    for (size_t i = 0; i < hs->hs_size; ++i)
    {
        hs_node* node = hs->chains[i];
        while (node != NULL)
        {
            hs_node* nxt = node->next;
            
            ull hash = hs->calc_hash(node->key);
            size_t ind = hash % new_size;
            node->prev = NULL;
            node->next = new_chains[ind];
            new_chains[ind] = node;
            if (node->next != NULL)
            {
                node->next->prev = node;
            }
            
            new_chain_sizes[ind]++;
            node = nxt;
        }
    }
    
	size_t max_chain_size = 0;
	size_t max_chain_cnt = 0;
	size_t min_chain_size = ULLONG_MAX;
	size_t min_chain_cnt = 0;
    
	for (size_t i = 0; i < new_size; ++i) {
		if (new_chain_sizes[i] < min_chain_size)
		{
			min_chain_size = new_chain_sizes[i];
			min_chain_cnt = 1;
		}
		else if (new_chain_sizes[i] == min_chain_size)
		{
			min_chain_cnt++;
		}
		if (new_chain_sizes[i] > max_chain_size)
		{
			max_chain_size = new_chain_sizes[i];
            max_chain_cnt = 1;
		}
        else if (new_chain_sizes[i] == max_chain_size)
        {
            max_chain_cnt++;
        }
	}
    
	free_all(2, hs->chains, hs->chain_sizes);
	hs->hs_size = new_size;
	hs->chains = new_chains;
	hs->chain_sizes = new_chain_sizes;
	hs->max_chain_size = max_chain_size;
	hs->max_chain_cnt = max_chain_cnt;
	hs->min_chain_size = min_chain_size;
	hs->min_chain_cnt = min_chain_cnt;
    
    if (max_chain_size > 2 * (min_chain_size ? min_chain_size : 1)) {
		return Hash_set_rebuild(hs, 2);
	}
    
	return ok;
}

int Hash_set_search(Hash_set* hs, char* key, int create_flag, hs_node** node, size_t* chain_ind) {
	if (hs == NULL || key == NULL || node == NULL) {
		return INVALID_FUNCTION_ARGUMENT;
	}
    
	ull hash = hs->calc_hash(key);
	size_t ind = hash % hs->hs_size;
	hs_node* cur = hs->chains[ind];
    
    if (chain_ind != NULL)
    {
        *chain_ind = ind;
    }
    
	while (cur != NULL && strcmp(cur->key, key)) {
		cur = cur->next;
	}
	if (cur != NULL || !create_flag) {
		*node = cur;
		return ok;
	}
	
	hs_node* new_node = (hs_node*) malloc(sizeof(hs_node));
	char* key_copy = (char*) malloc(sizeof(char) * (strlen(key) + 1));
	if (new_node == NULL || key_copy == NULL) {
		free_all(2, new_node, key_copy);
		return MEMORY_ISSUES;
	}
    
	strcpy(key_copy, key);
	new_node->key = key_copy;
	new_node->dep = NULL;
    new_node->prev = NULL;
	new_node->next = hs->chains[ind]; // insert in head
    if (new_node->next != NULL)
    {
        new_node->next->prev = new_node;
    }
    
	hs->chains[ind] = new_node;
	*node = new_node;
	
	ull cur_size = hs->chain_sizes[ind];
	hs->chain_sizes[ind]++;
	// update data abount max chain
	if (hs->max_chain_size == cur_size) {
		hs->max_chain_size++;
        hs->max_chain_cnt = 1;
	}
	// update data about min chain
	if (hs->min_chain_size == cur_size) {
		hs->min_chain_cnt--;
		if (hs->min_chain_cnt == 0)
		{
			hs->min_chain_size = cur_size + 1;
			for (size_t i = 0; i < hs->hs_size; ++i)
			{
				if (hs->chain_sizes[i] == hs->min_chain_size)
				{
					hs->min_chain_cnt++;
				}
			}
		}
	}
    
	size_t mn = hs->min_chain_size ? hs->min_chain_size : 1;
	if (hs->max_chain_size > 2 * mn) {
		int code = Hash_set_rebuild(hs, 2);
		return code;
	}
    
    if (chain_ind != NULL)
    {
        *chain_ind = hash % hs->hs_size;
    }
    
	return ok;
}

int Hash_set_find(Hash_set* hs, char* key, Department** dep) {
	if (hs == NULL || key == NULL || dep == NULL) {
		return INVALID_FUNCTION_ARGUMENT;
	}
    
	hs_node* node = NULL;
	int err_code = Hash_set_search(hs, key, 0, &node, NULL);
	if (err_code) {
		return err_code;
	}
    
	if (node == NULL) {
        *dep = NULL;
	}
    else {
        *dep = node->dep;
    }
    
	return ok;
}

int Hash_set_insert(Hash_set* hs, char* key, Department* dep) {
	if (hs == NULL || key == NULL || dep == NULL) {
		return INVALID_FUNCTION_ARGUMENT;
	}
    
	hs_node* node = NULL;
	int err_code = Hash_set_search(hs, key, 1, &node, NULL);
	if (err_code) {
		return err_code;
	}
    
    if (node->dep != NULL)
    {
        return BAD_ACCESS;
    }
    
    node->dep = dep;
    
	return ok;
}