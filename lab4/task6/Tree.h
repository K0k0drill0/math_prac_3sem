#ifndef TREE_H
#define TREE_H


enum status_code {
    ok, 
    UNABLE_TO_OPEN_FILE,
    MEMORY_ISSUES,
    OVERFLOWED,
    INVALID_FUNCTION_ARGUMENT,
    INVALID_AMOUNT_OF_ARGUMENTS,

    INVALID_CHARACTER,
    UNBALANCED_BRACKETS,
    INVALIND_OPERATOR_POSTURE,
    NO_VALUE_IN_BRACKETS, 
    EMPTY_EXPRESSION,
    INVALID_VAR_NAME,
    INVALID_EXPRESSION,
    INVALID_BRACKETS,

    DIVISION_BY_ZERO,
    ZERO_POW_ZERO,
    NEGATIVE_POWER
};

typedef struct Node {
    char data;
    struct Node* left;
    struct Node* right;
} Node;

int create_tree_node(Node** nd, const char value);
void print_tree(Node* root, int level);
void print_expr(FILE * file, Node* root);

int make_tree(Node** tree, const char expr[], int first, int last);
int calculate_tree(Node* root, const char* variables, const char* values);

void free_tree(Node* root);

#endif