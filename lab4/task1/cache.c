#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ultra_header.h"

int create_cache_node(Cache** cd, char* key, ull value, Cache* next) {
    *cd = (Cache*)malloc(sizeof(Cache));
    if (*cd == NULL) {
        return MEMORY_ISSUES;
    }

    (*cd)->key = key;
    (*cd)->value = value;
    (*cd)->next = next;
    return ok;
}

int add_cache_node_rp(Cache** head, char** key, ull value) {
    Cache* new_node;
    int st = create_cache_node(&new_node, *key, value, NULL);
    if (st != ok) {
        return st;
    }

    if (*head == NULL) {
        *head = new_node;
        return ok;
    } 
    if (!strcmp((*head)->key, *key)) {
        (*head)->value = value;
        free(new_node->key);   
        new_node->key = (*head)->key;
        free(new_node);

        *key = (*head)->key;
        return ok;
    }

    Cache* tmp = *head;

    while (tmp->next != NULL && strcmp(tmp->key, new_node->key)) {
        tmp = tmp->next;
    }

    if (!strcmp(tmp->key, new_node->key)) {
        tmp->value = new_node->value;
        free(new_node->key);   
        new_node->key = tmp->key;
        free(new_node);

        *key = tmp->key;
    }
    else {
        tmp->next = new_node;
    }

    return ok;
}

void print_cache_list(Cache* head) {
    Cache* tmp = head;
    while (tmp != NULL) {
        printf("(%s, %llu) ", tmp->key, tmp->value);
        tmp = tmp->next;
    }
    printf("\n");
}

void delete_cache_list(Cache* head) {
    if (head == NULL) {
        return;
    }
    Cache* tmp = head;
    Cache* next_ptr = head->next;
    while(next_ptr != NULL) {
        //free(tmp->key);
        free(tmp);
        tmp = next_ptr;
        next_ptr = tmp->next;
    }
    //free(tmp->key);
    free(tmp);
}
