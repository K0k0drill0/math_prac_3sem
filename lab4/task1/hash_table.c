#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <errno.h>
#include "ultra_header.h"

int which_number(char c) {
    if (isdigit(c)) return c - '0';
    if (isupper(c)) return c - 'A' + 10;
    if (islower(c)) return c - 'a' + 20;
    return -1;
}

ull pre_hash(const char* key) {
    ull ans = which_number(key[0]);
    int ind = 1;

    while (key[ind] != '\0') {
        ans *= 62;
        ans %= MOD_MAIN;
        ans += which_number(key[ind]);
        ans %= MOD_MAIN;

        ind++;
    }

    return ans;
}

ull hash(const char* key, const ull HASHSIZE) {
    return pre_hash(key) % HASHSIZE;
}

int is_rehash_needed(const HashTable* ht) {
    ull min_val, max_val;
    min_val = ht->lists_lenghths[0], max_val = ht->lists_lenghths[0];

    for (int i = 0; i < ht->HashSize; i++) {
        if (ht->lists_lenghths[i] < min_val) {
            min_val = ht->lists_lenghths[i];
        }
        if (ht->lists_lenghths[i] > max_val) {
            max_val = ht->lists_lenghths[i];
        }
    }

    return (max_val - min_val >= 2);
}

int rehashing(Cache* cache, HashTable** ht_old) {
    ull new_hashsize = (*ht_old)->HashSize * 2;
    if (errno == ERANGE) {
        return OVERFLOWED;
    }

    HashTable* ht_new;
    int st = create_hashtable(&ht_new, new_hashsize);
    if (st != ok) {
        return st;
    }
    ht_new->HashSize = new_hashsize;

    Cache* tmp = cache;
    while(tmp != NULL) {
        ull hsh = tmp->value;
        char* key_in_new = tmp->key;
        char* value_in_new = find_node_by_key((*ht_old)->table[hsh%((*ht_old)->HashSize)], key_in_new)->value;
        st = add_node_rp(&(ht_new->table[hsh % new_hashsize]), &key_in_new, value_in_new,
         &(ht_new->lists_lenghths[hsh%new_hashsize]));
        if (st != ok) {
            delete_hashtable_no_val(ht_new);
            return st;
        }

        tmp = tmp->next;
    }

    delete_hashtable_no_val(*ht_old);

    *ht_old = ht_new;

    return ok;

}

int create_hashtable(HashTable** ht, ull hash_size) {
    *ht = (HashTable*)malloc(sizeof(HashTable));
    if (*ht == NULL) {
        return MEMORY_ISSUES;
    }

    (*ht)->HashSize = hash_size;
    (*ht)->lists_lenghths = NULL;
    (*ht)->table = NULL;

    (*ht)->table = (Node**)calloc(hash_size ,sizeof(Node*));
    if ((*ht)->table == NULL) { 
        free(*ht);
        return MEMORY_ISSUES;
    }

    (*ht)->lists_lenghths = (ull*)calloc(hash_size, sizeof(ull));
    if ((*ht)->lists_lenghths == NULL) {
        free((*ht)->table);
        free(*ht);
        return MEMORY_ISSUES;
    }

    return ok;
}

int insert_hashtable(HashTable** ht, Cache** cache, char* key,
 char* value) {
    ull hs = pre_hash(key);
    int st = ok;
    st = add_cache_node_rp(cache, &key, hs);
    if (st != ok) {
        return st;
    }
    hs %= (*ht)->HashSize;

    st = add_node_rp(&((*ht)->table[hs]), &key, value, &((*ht)->lists_lenghths[hs]));
    if (st != ok) {
        return st;
    }

    while (is_rehash_needed(*ht)) {
        st = rehashing(*cache, ht);
        if (st != ok) {
            break;
        }
    }

    if (st != OVERFLOWED) {
        return st;
    }


    return ok;
}

char* find_value_hashtable(const HashTable* ht, const char* key) {
    ull hs = hash(key, ht->HashSize);
    Node* ans = find_node_by_key(ht->table[hs], key);
    return (ans == NULL) ? NULL : ans->value;
}

void delete_hashtable(HashTable* ht) {
    for (int i = 0; i < ht->HashSize; i++) {
        delete_list(ht->table[i]);
    }
    free(ht->table);
    free(ht->lists_lenghths);
    free(ht);
}

void delete_hashtable_no_val(HashTable* ht) {
    for (int i = 0; i < ht->HashSize; i++) {
        delete_list_no_val(ht->table[i]);
    }
    free(ht->table);
    free(ht->lists_lenghths);
    free(ht);
}

void print_hashtable(const HashTable* ht) {
    printf("Hashtable for hashsize %llu:\n", ht->HashSize);
    for (ull i = 0; i < ht->HashSize; i++) {
        Node* cur = ht->table[i];
        if (cur != NULL) printf("Hash value %llu; size %llu: ", i, ht->lists_lenghths[i]);
        while (cur != NULL) {
            printf("(%s, %s) ", cur->key, cur->value);
            cur = cur->next;
        }
        if (ht->table[i] != NULL) {
            printf("\n");
        }
    }
}
