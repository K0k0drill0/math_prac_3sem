#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ultra_header.h"

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
    case OVERFLOWED:
        printf("Overflowed!\n");
        break;
    case INVALID_DEFINE:
        printf("Invalid define!\n");
        break;
    default:
        break;
    }
}

int create_node(Node** nd, char* key, char* value, Node* next) {
    *nd = (Node*)malloc(sizeof(Node));
    if (*nd == NULL) {
        return MEMORY_ISSUES;
    }
    (*nd)->key = key;
    (*nd)->value = value;
    (*nd)->next = next;
    return ok;
}

int add_node(Node** head, char* key, char* value) {
    Node* new_node;
    int st = create_node(&new_node, key, value, NULL);
    if (st != ok) {
        return st;
    }

    if (*head == NULL) {
        *head = new_node;
        return ok;
    }

    Node* tmp = *head;
    
    while (tmp->next != NULL) {
        tmp = tmp->next;
    }
    tmp->next = new_node;
    return ok;
}

int add_node_rp(Node** head, char** key, char* value, ull* sz) {
    Node* new_node;
    int st = create_node(&new_node, *key, value, NULL);
    if (st != ok) {
        return st;
    }

    if (*head == NULL) {
        *head = new_node;
        *sz = 1;
        return ok;
    } 
    if (!strcmp((*head)->key, *key)) {
        free((*head)->value);
        (*head)->value = value;
        (*head)->key = *key;
        free(new_node);
        return ok;
    }

    Node* tmp = *head;

    while (tmp->next != NULL && strcmp(tmp->key, new_node->key)) {
        tmp = tmp->next;
    }

    if (!strcmp(tmp->key, new_node->key)) {
        free(tmp->value);
        tmp->value = new_node->value;
        tmp->key = new_node->key;
        free(new_node);
    }
    else {
        tmp->next = new_node;
        (*sz)++;
    }

    return ok;
}

Node* find_node_by_key(Node* head, const char* key) {
    Node* tmp = head;
    while (tmp != NULL) {
        if (!strcmp(tmp->key, key)) {
            break;
        }
        tmp = tmp->next;
    }
    return tmp;
}

void print_list(Node* head) {
    Node* tmp = head;
    while (tmp != NULL) {
        printf("(%s, %s) ", tmp->key, tmp->value);
        tmp = tmp->next;
    }
    printf("\n");
}

void delete_list(Node* head) {
    if (head == NULL) {
        return;
    }
    Node* tmp = head;
    Node* next_ptr = head->next;
    while(next_ptr != NULL) {
        free(tmp->key);
        free(tmp->value);
        free(tmp);
        tmp = next_ptr;
        next_ptr = tmp->next;
    }
    free(tmp->key);
    free(tmp->value);
    free(tmp);
}

void delete_list_no_val(Node* head) {
    if (head == NULL) {
        return;
    }
    Node* tmp = head;
    Node* next_ptr = head->next;
    while(next_ptr != NULL) {
        free(tmp);
        tmp = next_ptr;
        next_ptr = tmp->next;
    }
    free(tmp);
}