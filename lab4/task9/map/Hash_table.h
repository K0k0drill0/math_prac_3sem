#ifndef _HASH_SET_H_
#define _HASH_SET_H_

#include "../Department.h"

#define HASH_SET_ALPHABET_SIZE 36
#define MIN_HS_SIZE 127
#define MAX_HS_SIZE (1ULL << 58)

typedef unsigned long long ull;

typedef struct hs_node
{
	char* key;
	Department* dep;
	struct hs_node* prev;
	struct hs_node* next;
} hs_node;

typedef struct Hash_set
{
	ull (*calc_hash)(char*);
	size_t hs_size;
	size_t* chain_sizes;
	hs_node** chains;
	size_t max_chain_size;
	size_t max_chain_cnt;
	size_t min_chain_size;
	size_t min_chain_cnt;
} Hash_set, Hash_set;

int Hash_set_set_null(Hash_set* hs);
int Hash_set_init(Hash_set* hs);
int Hash_set_free(Hash_set* hs);

//int Hash_set_contains(Hash_set* hs, char* key, int* is_contained);
int Hash_set_find(Hash_set* hs, char* key, Department** dep);
int Hash_set_insert(Hash_set* hs, char* key, Department* dep);
int Hash_set_erase(Hash_set* hs, char* key);

#endif // _HASH_SET_H_