#ifndef TREE_H
#define TREE_H

enum status_code {
    ok, 
    MEMORY_ISSUES,
    INVALID_FUNCTION_PARAMETER,
    INVALID_AMOUNT_OF_ARGUMENTS,
    UNABLE_TO_OPEN_FILE
};

typedef struct Node {
    char* data;
    int amount;
    struct Node* left;
    struct Node* right;
    struct Node* parent;
} Node;

int init_node(Node** nd, char* str);

int add_node(Node** root, char* str);

Node* find_node_by_word(Node* root, const char* word);
void print_all_nodes_by_amount(Node* root, const int amount, int* N);
void get_amount_of_nodes_with_count(Node* root,
 const int amount, int* amount_of_nodes);
void find_longest_word(Node* root, int* max_len, char** ans);
void find_shortest_word(Node* root, int* min_len, char** ans);
int tree_depth(Node* root);
void tree_max_amount(Node* root, int* max_amount);

void tree_data_to_file(FILE* outp, Node* root);
void print_tree(Node* root, int lvl);

void free_node(Node* nd);
void free_tree(Node* root);

#endif