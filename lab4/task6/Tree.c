#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Tree.h"
#include "arithmetic.h"

int create_tree_node(Node** nd, const char value) {
    *nd = (Node*)malloc(sizeof(Node));
    if (*nd == NULL) {
        return MEMORY_ISSUES;
    }

    (*nd)->data = value;
    (*nd)->left = NULL;
    (*nd)->right = NULL;
    
    return ok;
}

void print_tree(Node* root, int n) {
    if (root != NULL) {
        print_tree(root->right, n + 1);
        for (int i = 0; i < n; i++) printf("\t");
        printf("%c\n", root->data);
        print_tree(root->left, n + 1);
    }
}


void print_expr(FILE * file, Node* root) {
    if (!root) {
        return;
    }
    if (is_operator(root->data)) {
        fprintf(file, "(");
    }
    print_expr(file, root->left);
    fprintf(file, "%c", root->data);
    if (root -> data == '-' || root -> data == '+' || root -> data == '<') {
        fprintf(file, ">");
    }
    print_expr(file, root -> right);
    if (is_operator(root->data)) {
        fprintf(file, ")");
    }
}

int make_tree(Node** tree, const char expr[], int first, int last) {
    int min_priority, k, prior, nest = 0;
    *tree = (Node*)malloc(sizeof(Node));
    if (*tree == NULL) {
        return MEMORY_ISSUES;
    }
    (*tree)->left = NULL; (*tree)->right = NULL;
    min_priority = 100;
    for (int i = first; i <= last; i++) {
        if (expr[i] == '(') {
            nest++;
        }
        else if (expr[i] == ')') {
            nest--;
        }
        if (nest > 0) {
            continue;
        }
        prior = priority(expr[i]);
        if (prior <= min_priority) {
            min_priority = prior;
            k = i;
        }
    }
    if (nest != 0) {
        free_tree(*tree);
        return UNBALANCED_BRACKETS;
    }
    if (min_priority == 100) {
        if (expr[first] == '(' && expr[last] == ')') {
            free_tree(*tree);
            return make_tree(tree, expr, first + 1, last - 1);
        }
        else {
            (*tree)->data = expr[first];
            return ok;
        }
    }
    (*tree)->data = expr[k];
    //(*tree)->data[1] = '\0';
    int st = ok;
    if (is_unar(expr[k])) {
        st = make_tree(&((*tree)->right), expr, k+1, last);
        if (st != ok) {
            free_tree(*tree);
        } 
        return ok;
    }
    st = make_tree(&((*tree)->left), expr, first, k-1);
    if (st != ok) {
        free_tree(*tree);
        return st;
    }
    st = make_tree(&((*tree)->right), expr, k+1, last);
    if (st != ok) {
        free_tree(*tree);
        return st;
    }

    return ok;
}

int calculate_tree(Node* root, const char* variables, const char* values) {
    if (!root) {
        return 0;
    } 
    int left = calculate_tree(root -> left, variables, values);
    int right = calculate_tree(root -> right, variables, values);
    if (isalpha(root->data)) {
        int ind = 0;
        while (variables[ind] != '\0') {
            if (root->data == variables[ind]) {
                return values[ind] - '0';
            }
            ind++;
        }
    }
    if (root->data == '0' || root->data == '1') {
        return root->data - '0';
    } 

    // если оператор
    switch (root -> data)
    {
        case '&': return left & right;
        case '|': return left | right;
        case '~': return ~right;
        case '-': return left <= right;
        case '+': return ~(~left | right);
        case '<': return left != right;
        case '=': return left == right;
        case '!': return ~(left & right);
        case '?': return ~(left | right);
        default: return INVALID_FUNCTION_ARGUMENT;
    }
}

void free_tree(Node* root) {
    if (root == NULL) {
        return;
    }
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}