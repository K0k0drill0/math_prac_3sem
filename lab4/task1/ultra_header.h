#ifndef ULTRA_HEADER_H
#define ULTRA_HEADER_H

#include <math.h>

typedef unsigned long long ull;

typedef struct Node {
    char* key;
    char* value;
    struct Node* next;
} Node;

typedef struct HashTable {
    Node** table;
    ull* lists_lenghths;
    ull HashSize;
} HashTable;

typedef struct Cache {
    char* key;
    ull value;
    struct Cache* next; 
} Cache;

#define MOD_MAIN 288230376151711744 // 2^58

enum status_codes {
    ok, 
    UNABLE_TO_OPEN_A_FILE,
    INVALID_FILE,
    INVALID_ARGUMENTS,
    MEMORY_ISSUES,
    INVALID_INPUT,
    OVERFLOWED,
    NO_DEFINE, 
    INVALID_DEFINE
};

//list 

void print_error(int st);

int create_node(Node** nd, char* key, char* value, Node* next);

int add_node(Node** head, char* key, char* value);

int add_node_rp(Node** head, char** key, char* value, ull* sz);

Node* find_node_by_key(Node* head, const char* key);

void print_list(Node* head);

void delete_list(Node* head);

void delete_list_no_val(Node* head);

//cache

int create_cache_node(Cache** cd, char* key, ull value, Cache* next);

int add_cache_node_rp(Cache** head, char** key, ull value);

void print_cache_list(Cache* head);

void delete_cache_list(Cache* head);

// hash_table

ull pre_hash(const char* key);

ull hash(const char* key, const ull HASHSIZE);

int which_number(char c);

int is_rehash_needed(const HashTable* ht);

int rehashing(Cache* cache, HashTable** ht_old);

int create_hashtable(HashTable** ht, ull hash_size);

int insert_hashtable(HashTable** ht, Cache** cache, char* key,
 char* value);

char* find_value_hashtable(const HashTable* ht, const char* key);

void delete_hashtable(HashTable* ht);

void delete_hashtable_no_val(HashTable* ht);

void print_hashtable(const HashTable* ht);

#endif