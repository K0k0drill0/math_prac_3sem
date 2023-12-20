#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include "Tree.h"

void print_error(const char c) {
    switch (c)
    {
    case MEMORY_ISSUES:
        printf("Unable to allocate the memory!\n");
        break;
    case INVALID_FUNCTION_PARAMETER:
        printf("Invalid function parameter!\n");
        break;
    case INVALID_AMOUNT_OF_ARGUMENTS:
        printf("Invalid amount of arguments!\n");
        break;
    case UNABLE_TO_OPEN_FILE:
        printf("Unable to open a file!\n");
        break;
    default:
        break;
    }
}

int is_separator(const char c) {
    return (c == ' ' || c == '\t' || c == '\n');
}

int is_valid_uint(const char* str) {
    int ind = 0;
    while (str[ind] != '\0') {
        if (!isdigit(str[ind])) {
            return 0;
        }
        ind++;
    }
    return (ind == 0) ? 0 : 1;
}

int read_word_from_file(FILE* inp, char** str_inp) {
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
        return EOF;
    }

    while (!is_separator(c) && c != EOF) {
        if (ind >= arr_max_size - 1) {
            arr_max_size *= 2;
            char *tmp = (char*)realloc(*str_inp, arr_max_size * sizeof(char));
            if (tmp == NULL) {
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

int build_tree(FILE* inp, Node** root) {
    int st = ok;
    char* data = NULL;
    while ((st = read_word_from_file(inp, &data)) == ok) {
        st = add_node(root, data);
        if (st != ok) {
            free(data);
            return st;
        }
    }
    free(data);

    if (st == EOF) {
        return ok;
    }
    return st;
}

void print_menu() {
    printf("Chose the action:\n");
    printf("1. How much a word was met in the text.\n");
    printf("2. Most often N-word.\n");
    printf("3. Shortest word.\n");
    printf("4. Longest word.\n");
    printf("5. Depth of the tree.\n");
    printf("6. Tree to file and back\n");
    printf("7. Exit.\n");
}

int interaction_with_user(Node** root) {
    int st = ok;
    print_menu();

    char* req = NULL;
    while ((st = read_word_from_file(stdin, &req)) == ok) {
        fflush(stdin);
        if (strlen(req) != 1 || req[0] < '0' || req[0] > '7') {
            free(req);
            printf("Invalid input! Try again\n");
            print_menu();
            continue;
        }

        char q = req[0];
        free(req);

        if (q == '1') {
            char* word_tryna_find = NULL;
            st = read_word_from_file(stdin, &word_tryna_find);
            if (st != ok) {
                return st;
            }
            fflush(stdin);

            Node* found = find_node_by_word(*root, word_tryna_find);
            if (found == NULL) {
                printf("No such word! Try again.\n");
            }
            else {
                printf("Was found: %d\n", found->amount);
            }
            free(word_tryna_find);
        }
        else if (q == '2') {
            char* N_word = NULL;
            st = read_word_from_file(stdin, &N_word);
            if (st != ok) {
                return st;
            }
            if (!is_valid_uint(N_word)) {
                printf("Invalid integer! Try again.\n");
                print_menu();
                free(N_word);
                continue;
            } 
            int N = atoi(N_word);
            free(N_word);

            int max_amount = 0;
            tree_max_amount(*root, &max_amount);
            if (max_amount == 0) {
                printf("No words in tree!\n");
            }
            else {
                int am_nodes = 0;
                get_amount_of_nodes_with_count(*root, max_amount, &am_nodes);
                while (N != 0 && max_amount != 0) {
                    for (int i = 0; i < am_nodes; i++) {
                        print_all_nodes_by_amount(*root, max_amount, &N);
                    }
                    max_amount--;
                }
                printf("\n");
            }
        }
        else if (q == '3') {
            char* shortest = NULL;
            int len = INT_MAX;
            find_shortest_word(*root, &len, &shortest);
            if (shortest == NULL) {
                printf("No words in tree!\n");
            }
            else {
                printf("Shortest word in the tree: %s\n", shortest);
            }
        }
        else if (q == '4') {
            char* longest = NULL;
            int len = -1;
            find_longest_word(*root, &len, &longest);
            if (longest == NULL) {
                printf("No words in tree!\n");
            }
            else {
                printf("Longest word in the tree: %s\n", longest);
            }
        }
        else if (q == '5') {
            printf("Depth of the tree: %d\n", tree_depth(*root));
        }
        else if (q == '6') {
            FILE* outp = tmpfile();
            if (outp == NULL) {
                return UNABLE_TO_OPEN_FILE;
            }

            tree_data_to_file(outp, *root);
            fseek(outp, 0, SEEK_SET);
            free_tree(*root);
            *root = NULL;

            st = build_tree(outp, root);
            if (st != ok) {
                fclose(outp);
                return st;
            }
            fclose(outp);
        }
        else if (q == '7') {
            return ok;
        }

        print_menu();
    }
}

int main(int argc, char** argv) {
    argc = 2;
    argv[1] = "input.txt";

    if (argc != 2) {
        print_error(INVALID_AMOUNT_OF_ARGUMENTS);
        return 1;
    }

    int st = ok;

    FILE* inp = fopen(argv[1], "r");
    if (inp == NULL) {
        print_error(UNABLE_TO_OPEN_FILE);
        return 1;
    }

    Node* root = NULL;
    st = build_tree(inp, &root);
    if (st != ok) {
        fclose(inp);
        free_tree(root);
        print_error(st);
        return 1;
    }

    st = interaction_with_user(&root);
    if (st != ok) {
        fclose(inp);
        free_tree(root);
        print_error(st);
        return 1;
    }

    //print_tree(root, 0);
    fclose(inp);
    free_tree(root);

    return 0;
}