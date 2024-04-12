#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>

#include "Trie.h"
#include "../errors.h"

typedef unsigned long long ull;

int char_to_int(char ch) {
    if (isdigit(ch)) {
        return ch - '0';
    }
    if (isalpha(ch)) {
        return toupper(ch) - 'A' + 10;
    }
    return -1;
}

int char_to_ind(char ch) {
    return char_to_int(ch);
}

char ind_to_char(int ind) {
    if (ind >= 0 && ind <= 9) {
        return '0' + ind;
    }
    if (ind > 9 && ind < 36) {
        return 'a' + ind - 10;
    }
    return '\0';
}


int Trie_node_init(Trie_node** node) {
    if (node == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    *node = (Trie_node*) malloc(sizeof(Trie_node));
    if (*node == NULL) {
        return MEMORY_ISSUES;
    }
    for (ull i = 0; i < TRIE_ALPABET_SIZE; ++i) {
        (*node)->children[i] = NULL;
    }
    (*node)->parent = NULL;
    (*node)->dep = NULL;
    return ok;
}

int Trie_node_free(Trie_node** node) {
    if (node == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    if (*node == NULL) {
        return ok;
    }
    
    for (ull i = 0; i < TRIE_ALPABET_SIZE; ++i) {
        Trie_node_free(&(*node)->children[i]);
    }
    Department_free((*node)->dep);
    //free((*node)->dep);
    free(*node);
    //free_all(2, (*node)->dep, *node);
    *node = NULL;
    return ok;
}

int Trie_set_null(Trie* Trie) {
    if (Trie == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    Trie->root = NULL;
    return ok;
}

int Trie_init(Trie* Trie) {
    if (Trie == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    return Trie_node_init(&Trie->root);
}

int Trie_free(Trie* Trie) {
    if (Trie == NULL) {
        return ok;
    }
    int err_code = ok;
    if (Trie->root != NULL) {
        err_code = Trie_node_free(&Trie->root);
    }
    Trie->root = NULL;
    return err_code;
}


int Trie_search_node (Trie* Trie, char* key, Trie_node** node) {
    if (Trie == NULL || key == NULL || node == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    if (Trie->root == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    *node = NULL;
    Trie_node* cur = Trie->root;
    for (ull i = 0; key[i]; ++i) {
        if (cur == NULL) {
            *node = NULL;
            return ok;
        }
        int ind = char_to_ind(key[i]);
        if (ind == -1) {
            return INVALID_INPUT;
        }
        cur = cur->children[ind];
    }
    *node = cur;
    return ok;
}

int Trie_create_node (Trie* Trie, char* key, Trie_node** node) {
    if (Trie == NULL || key == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    if (Trie->root == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    int err_code = ok;
    Trie_node* cur = Trie->root;
    Trie_node* first_new = NULL;
    for (ull i = 0; key[i] && !err_code; ++i) {
        int ind = char_to_ind(key[i]);
        if (ind == -1) {
            err_code = INVALID_FUNCTION_ARGUMENT;
        }
        if (cur->children[ind] == NULL) {
            err_code = Trie_node_init(&cur->children[ind]);
            first_new = first_new == NULL ? cur->children[ind] : first_new;
            if (!err_code)
            {
                cur->children[ind]->parent = cur;
            }
        }
        if (!err_code) {
            cur = cur->children[ind];
        }
    }
    if (err_code) {
        Trie_node_free(&first_new);
        cur = NULL;
    }
    if (node != NULL) {
        *node = cur;
    }
    return err_code;
}

int Trie_clean_branch (Trie* Trie, Trie_node* node, Trie_node* cleaned_child_node) {
    if (Trie == NULL || node == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    int child_flag = 0;
    for (ull i = 0; i < TRIE_ALPABET_SIZE; ++i) {
        if (cleaned_child_node != NULL && node->children[i] == cleaned_child_node) {
            node->children[i] = NULL;
        }
        if (node->children[i] != NULL) {
            child_flag = 1;
        }
    }
    if (child_flag || Trie->root == node || node->dep != NULL) {
        return ok;
    }
    Trie_node* parent = node->parent;
    Trie_node* tmp = node;
    Trie_node_free(&node);
    return Trie_clean_branch(Trie, parent, tmp);
}

int Trie_find (Trie* Trie, char* key, Department** dep) {
    if (Trie == NULL || key == NULL || dep == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    Trie_node* node = NULL;
    int err_code = Trie_search_node(Trie, key, &node);
    if (err_code) {
        return err_code;
    }
    // if (node == NULL) {
    //     return BAD_ACCESS;
    // }

    if (node == NULL) {
        *dep = NULL;
        return ok;
    }
    *dep = node->dep;
    return ok;
}

int Trie_insert(Trie* Trie, char* key, Department* dep) {
    if (Trie == NULL || key == NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    Trie_node* node = NULL;
    int err_code = Trie_create_node(Trie, key, &node);
    if (err_code) {
        return err_code;
    }
    if (node->dep != NULL) {
        return INVALID_FUNCTION_ARGUMENT;
    }
    node->dep = dep;
    if (node->dep == NULL) {
        return Trie_clean_branch(Trie, node, NULL);
    }
    return ok;
}
