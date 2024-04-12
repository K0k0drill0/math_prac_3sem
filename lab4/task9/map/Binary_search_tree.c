#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Binary_search_tree.h"
#include "../errors.h"

int Bst_node_init(Bst_node** node, char* key, Department* dep) {
    if (node == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    *node = (Bst_node*) malloc(sizeof(Bst_node));
    if (*node == NULL) {
        return MEMORY_ISSUES;
    }
    
    (*node)->key = (char*) malloc(sizeof(char) * (strlen(key) + 1));
    if ((*node)->key == NULL) {
        free(*node);
        return MEMORY_ISSUES;
    }
    
    strcpy((*node)->key, key);
    (*node)->left = NULL;
    (*node)->right = NULL;
    (*node)->dep = dep;
    
    return ok;
}

int Bst_node_free(Bst_node* node) {
    if (node == NULL) {
        return ok;
    }
    
    Bst_node_free(node->left);
    Bst_node_free(node->right);
    
    if (node->key != node->dep->name) {
        free(node->key);
    }

    Department_free(node->dep);
    free(node);
    //free_all(3, node->key, node->dep, node);
    
    return ok;
}


int Bst_set_null(Bst* tree) {
    if (tree == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }

    tree->root = NULL;

    return ok;
}

int Bst_init(Bst* tree) {
    if (tree == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    tree->root = NULL;
    
    return ok;
}

int Bst_free(Bst* tree) {
    if (tree == NULL) {
        return ok;
    }
    
    int err_code = ok;
    if (tree->root != NULL) {
        err_code = Bst_node_free(tree->root);
    }
    
    tree->root = NULL;
    
    return err_code;
}


int Bst_search_node(Bst* tree, char* key, Bst_node** node) {
    if (tree == NULL || key == NULL || node == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    if (tree->root == NULL) {
        *node = NULL;
        return ok;
    }
    
    Bst_node* cur = tree->root;
    int comp = 0;
    
    while (cur != NULL) {
        comp = strcmp(key, cur->key);
        if (comp == 0)
        {
            *node = cur;
            return ok;
        }
        else if (comp < 0)
        {
            cur = cur->left;
        }
        else
        {
            cur = cur->right;
        }
    }
    
    *node = NULL;
    return ok;
}

int Bst_find(Bst* tree, char* key, Department** dep) {
    if (tree == NULL || key == NULL || dep == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    Bst_node* node = NULL;
    Bst_search_node(tree, key, &node);
    
    // if (node == NULL) {
    //     return BAD_ACCESS;
    // }
    
    if (node == NULL) {
        *dep = NULL;
    }
    else {
        *dep = node->dep;
    }
    
    return ok;
}

int Bst_insert(Bst* tree, char* key, Department* dep) {
    if (tree == NULL || key == NULL || dep == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    
    if (tree->root == NULL) {
        return Bst_node_init(&tree->root, key, dep);
    }
    
    Bst_node* cur = tree->root;
    Bst_node* parent = NULL;
    int comp = 0;
    
    while (cur != NULL) {
        parent = cur;
        comp = strcmp(key, cur->key);
        
        if (comp == 0)
        {
            return BAD_ACCESS;
        }
        
        cur = comp < 0 ? cur->left : cur->right;
    }
    
    return comp < 0 ? Bst_node_init(&parent->left, key, dep)
                    : Bst_node_init(&parent->right, key, dep);
}

int Bst_del_node(Bst_node** del_node) {   
    Bst_node* node = *del_node;

    if (node == NULL) {
        return ok;
    }

    Bst_node* new_node;
    if (node->left == NULL && node->right == NULL) {
        new_node = NULL;
    }
    else if (node->left == NULL) {
        new_node = node->right;
    }
    else if (node->right == NULL) {
        new_node = node->left;
    }
    else {
        Bst_node* prev = node;
        new_node = node->right;
        while (new_node->left != NULL)
        {
            prev = new_node;
            new_node = new_node->left;
        }
        
        new_node->left = node->left;
        prev->left = new_node->right;
        
        if (node->right != new_node)
        {
            new_node->right = node->right;
        }
    }
    
    free(node->key);
    free(node);
    *del_node = new_node;

    return ok;
}

void print_tree(Bst_node *node, int tab)  {
    if (node == NULL)  {
        return;
    }

    print_tree(node->right, 1 + tab);

    for (int i = 0; i < tab; i++)  {
        printf("  ");
    }
    printf("%s\n", node->key);

    print_tree(node->left, 1 + tab);
}