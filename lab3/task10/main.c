#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    struct Node* parent;
    struct Node* brother;
    struct Node* son;
    char key;
} Node;

typedef struct {
    Node* root;
}Tree;

enum status_codes {
    ok, 
    UNABLE_TO_OPEN_A_FILE,
    INVALID_FILE,
    INVALID_ARGUMENTS,
    MEMORY_ISSUES,
    INVALID_INPUT,
    WRONG_AMOUNT_OF_ARGUMENTS
};

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
    case WRONG_AMOUNT_OF_ARGUMENTS:
        printf("Wrong amount of arguments!\n");
        break;
    default:
        break;
    }
}

int create_node(Node** node, const char key) {
    *node = (Node*)malloc(sizeof(Node));
    if (*node == NULL) {
        return MEMORY_ISSUES;
    }
    (*node)->parent = NULL;
    (*node)->brother = NULL;
    (*node)->son = NULL;
    (*node)->key = key;
    return ok;
}

int create_child(Node* node, const char key, Node** created_node) {
    if (node == NULL) {
        return INVALID_INPUT;
    }

    Node* new_node;
    int st = create_node(&new_node, key);
    if (st != ok) {
        return st;
    }

    new_node->parent = node;
    if (node->son == NULL) {
        node->son = new_node; 
    }
    else {
        Node* tmp = node->son;
        while (tmp->brother != NULL) {
            tmp = tmp->brother;
        }
        tmp->brother = new_node;
    }
    if (*created_node != NULL) {
        *created_node = new_node;
    }
    return ok;
}

void delete_tree_from_node(Node* node) {
    if (node == NULL) {
        return;
    }
    if (node->son != NULL) {
        delete_tree_from_node(node->son);
    }
    if (node->brother != NULL) {
        delete_tree_from_node(node->brother);
    }
    if (!node->son) {
        free(node->son);
    }
    if (!node->brother) {
        free(node->brother);
    }
    if (!node) {
        free(node);
    }
}

int create_tree(const char* str, Tree* tree) {
    if (str == NULL || tree == NULL) {
        return INVALID_INPUT;
    }

    int st = ok;
    Node* parent = NULL;
    Node* cur_node = NULL;
    Node* help_node = NULL;
    int key_waiting_flag = 1;
    int ind = 0;
    while (str[ind] != '\0' && st == ok) {
        while (str[ind] == ' ' || str[ind] == '\t') {
            ind++;
        }

        if (key_waiting_flag) {
            key_waiting_flag = 0;
            if (cur_node == NULL) {
                st = create_node(&(tree->root), str[ind]);
                cur_node = tree->root;
                help_node = tree->root;
            }
            else {
                st = create_child(cur_node, str[ind], &help_node);
            }
        }
        else {
            key_waiting_flag = 1;
            if (str[ind] == '(') {
                cur_node = help_node;
            }
            else if (str[ind] == ')') {
                cur_node = cur_node->parent;
                key_waiting_flag = 0;  
            }
        }
        ind++;
    }
    return st;
}

void fprint_tree_node(FILE* file, const Node* node, const int offset)
{
	if (node == NULL || file == NULL)
	{
		return;
	}
	for (int i = 0; i < offset; ++i)
	{
		fprintf(file, "  ");
	}
	fprintf(file, "%c\n", node->key);
	fprint_tree_node(file, node->son, offset + 1);
	fprint_tree_node(file, node->brother, offset);
}

void fprint_tree(FILE* file, const Tree tree)
{
	if (file == NULL)
	{
		return;
	}
	fprint_tree_node(file, tree.root, 0);
}

int is_separator(const char c) {
    return (c == ' ' || c == '\t' || c == '\n');
}

int get_word_from_file(FILE* inp, char** str_inp) {
    int arr_max_size = 16;
    *str_inp = (char*)malloc(sizeof(char) * arr_max_size);
    if (*str_inp == NULL) {
        return MEMORY_ISSUES;
    }

    char c;
    c = getc(inp);
    int ind = 0;
    while (is_separator(c) && c != EOF) {
        c = getc(inp);
    }

    if (c == EOF) {
        //free(*str_inp);
        return EOF;
    }

    while (c != '\n' && c != EOF) {
        if (ind >= arr_max_size - 1) {
            arr_max_size *= 2;
            char *tmp = (char*)realloc(*str_inp, arr_max_size * sizeof(char));
            if (tmp == NULL) {
                //free(str_inp);
                return MEMORY_ISSUES;
            }
            *str_inp = tmp;
        }
        (*str_inp)[ind] = c;
        ind++;
        c = getc(inp);
    }
    
    (*str_inp)[ind] = '\0';
    return ok;
}

int main(int argc, char** argv) {
    argc = 3;
    argv[1] = "input.txt";
    argv[2] = "output.txt";

    if (argc != 3) {
        print_error(WRONG_AMOUNT_OF_ARGUMENTS);
        return 1;
    }

    FILE* inp = fopen(argv[1], "r");
    if (!inp) {
        print_error(UNABLE_TO_OPEN_A_FILE);
        return -1;
    }
    FILE* outp = fopen(argv[2], "w+");
    if (!outp) {
        fclose(inp);
        print_error(UNABLE_TO_OPEN_A_FILE);
        return -1;
    }

    char* expr = NULL;
    int st;

    while ((st = get_word_from_file(inp, &expr)) == ok) {
        Tree tree;
        st = create_tree(expr, &tree);
        if (st != ok) {
            free(expr);
            delete_tree_from_node(tree.root);
            break;
        }
        fprint_tree(outp, tree);
        fprintf(outp, "\n");
        delete_tree_from_node(tree.root);
        free(expr);
    }

    free(expr);
    fclose(inp);
    fclose(outp);
}


