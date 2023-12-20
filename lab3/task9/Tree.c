#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Tree.h"

int init_node(Node** nd, char* str) {
    if (str == NULL) {
        return INVALID_FUNCTION_PARAMETER;
    }

    *nd = (Node*)malloc(sizeof(Node));
    if (*nd == NULL) {
        return MEMORY_ISSUES;
    }

    (*nd)->data = str;
    (*nd)->amount = 1;
    (*nd)->left = NULL;
    (*nd)->right = NULL;
    (*nd)->parent = NULL;

    return ok;
}

int add_node(Node** root, char* str) {
    int st = ok;
    if (*root == NULL) {
        return init_node(root, str);
    }

    Node* parent = *root, *tmp = NULL;
    if (strcmp((*root)->data, str) == 0) {
        (*root)->amount++;
        free(str);
        return ok;
    }

    tmp = (strcmp(parent->data, str) > 0) ? parent->left : parent->right;

    while (tmp != NULL && strcmp(tmp->data, str) != 0) {
        parent = tmp;
        tmp = (strcmp(parent->data, str) > 0) ? parent->left : parent->right;
    }

    if (tmp != NULL) {
        tmp->amount++;
        free(str);
        return ok;
    }
    else {
        Node* new_node = NULL;
        st = init_node(&new_node, str);
        if (st != ok) {
            return st;
        }
        new_node->parent = parent;

        if (strcmp(parent->data, str) > 0) {
            parent->left = new_node;
        }
        else {
            parent->right = new_node;
        }
    }

    return ok;
}

Node* find_node_by_word(Node* root, const char* word) {
    if (root == NULL || strcmp(root->data, word) == 0) {
        return root;
    }

    return (strcmp(root->data, word) > 0) ? find_node_by_word(root->left, word)
     : find_node_by_word(root->right, word); 
}

void print_all_nodes_by_amount(Node* root, const int amount, int* N) {
    if (root == NULL) {
        return;
    }

    print_all_nodes_by_amount(root->left, amount, N);

    if (root->amount == amount && (*N)) {
        printf("%s - %d\n", root->data, root->amount);
        (*N)--;
    }

    print_all_nodes_by_amount(root->right, amount, N);
}

void get_amount_of_nodes_with_count(Node* root,
 const int amount, int* amount_of_nodes) {
    if (root == NULL) {
        return;
    }

    if (root->amount == amount) {
        (*amount_of_nodes)++;
    }

    get_amount_of_nodes_with_count(root->left, amount, amount_of_nodes);
    get_amount_of_nodes_with_count(root->right, amount, amount_of_nodes);
}

void find_longest_word(Node* root, int* max_len, char** ans) {
    if (root == NULL) {
        return;
    }

    find_longest_word(root->left, max_len, ans);

    int len = strlen(root->data);
    if (len > *max_len) {
        *max_len = len;
        *ans = root->data; 
    }

    find_longest_word(root->right, max_len, ans);
}

void find_shortest_word(Node* root, int* min_len, char** ans) {
    if (root == NULL) {
        return;
    }

    find_longest_word(root->left, min_len, ans);

    int len = strlen(root->data);
    if (len < *min_len) {
        *min_len = len;
        *ans = root->data; 
    }

    find_longest_word(root->right, min_len, ans);
}

int tree_depth(Node* root) {
    if (root == NULL) return 0;

    int Left = tree_depth(root->left);

    int Right = tree_depth(root->right);

    return ((Left > Right) ? Left : Right) + 1;
}

void tree_max_amount(Node* root, int* max_amount) {
    if (root == NULL) {
        return;
    }

    *max_amount = (root->amount > *max_amount) ? root->amount : *max_amount;
    tree_max_amount(root->left, max_amount);
    tree_max_amount(root->right, max_amount);
}

void tree_data_to_file(FILE* outp, Node* root) {
    if (root == NULL || outp == NULL) {
        return;
    }

    for (int i = 0; i < root->amount; i++) {
        fprintf(outp, "%s ", root->data);
    }

    tree_data_to_file(outp, root->left);
    tree_data_to_file(outp, root->right);
}

void print_tree(Node* root, int lvl) {
    if (root == NULL) {
        return;
    } 
    if (root->left) {
        print_tree(root->left, lvl + strlen(root->data));
    } 
    for (int i = 0; i < lvl; ++i) {
        printf(" ");
    } 

    printf("%s-%d\n", root->data, root->amount);
    if (root->right) {
        print_tree(root->right, lvl + strlen(root->data));
    } 
}

void free_node(Node* nd) {
    if (nd == NULL) {
        return;
    }

    free(nd->data);
    free(nd);
}

void free_tree(Node* root) {
    if (root == NULL) {
        return;
    }

    free_tree(root->left);
    free_tree(root->right);
    free_node(root);
}