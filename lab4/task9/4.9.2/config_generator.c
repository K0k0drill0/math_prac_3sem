#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include "../readers.h"
#include "../validators.h"
#include "../data_structs.h"
#include "../errors.h"

typedef enum gen_stage
{
    GEN_STAGE_PQ,
    GEN_STAGE_MAP,
    GEN_STAGE_TIME,
    GEN_STAGE_HANDLE_TIME,
    GEN_STAGE_DEP_CNT,
    GEN_STAGE_DEP_INF,
    GEN_STAGE_COEF,
    GEN_STAGE_EXIT,
    GEN_STAGE_FINISH
} gen_stage;


int main() {
    int st = ok;

    gen_stage stage = GEN_STAGE_PQ;

    char* input = NULL;
    char* input2 = NULL;

    char* pq_name = NULL; 
    char* map_name = NULL; 
    char* st_time = NULL;
    char* end_time = NULL;

    ull min_handle_time, max_handle_time, dep_cnt;
    unsigned int* staff_cnts = NULL;
    char** dep_names = NULL;
    double overload_coef;

    while(st == ok && stage != GEN_STAGE_EXIT && stage != GEN_STAGE_FINISH) {
        if (stage == GEN_STAGE_PQ) {
            char** pq_names = {"BinaryHeap", "BinomialHeap", "FibonacciHeap", "SkewHeap", "LeftistHeap", "Treap"};

            printf("Input type of pq:\n");
            printf("1) BinaryHeap\n");
            printf("2) BinomialHeap\n");
            printf("3) FibonacciHeap\n");
            printf("4) SkewHeap\n");
            printf("5) LeftistHeap\n");
            printf("6) Treap\n");
            printf("0) Exit\n\n");

            st = read_line(stdin, &input);
            if (st != ok) {
                break;
            }

            if (strlen(input) == 1 && input[0] - '0' > 0 && input[0] - '0' <= 6) {
                st = strcpy_with_malloc(&pq_name, pq_names[input[0] - '0' - 1]);
                if (st != ok) {
                    break;
                }
                stage = GEN_STAGE_MAP
            }
            else if (strlen(input) == 1 && input[0] - '0' == 0) {
                stage = GEN_STAGE_EXIT;
            }
            else {
                printf("Invalid priority queue type!\n");
            }
            free(input);
            input = NULL;
        }
        if (stage == GEN_STAGE_MAP) {
            char** map_types = {"HashSet", "DynamicArray", "BinarySearchTree", "Trie"};

            printf("Input map type.\n");

            printf("0) Exit.\n");
            printf("1) HashSet.\n");
            printf("2) DynamicArray.\n");
            printf("3) BinarySearchTree.\n");
            printf("4) Trie.\n");

            st = read_line(stdin, &input);
            if (st != ok) {
                break;
            }

            if (strlen(input) == 1 && input[0] - '0' > 0 && input[0] - '0' <= 4) {
                st = strcpy_with_malloc(&map_name, map_types[input[0] - '0' - 1]);
                if (st != ok) {
                    break;
                }
                stage = GEN_STAGE_TIME;
            }
            else if (strlen(input) == 1 && input[0] - '0' == 0) {
                stage = GEN_STAGE_EXIT;
            }
            else {
                printf("Invalid map type!\n");
            }
            free(input);
            input = NULL;
        }
        if (stage == GEN_STAGE_TIME) {
            printf("Input the date and time of the start and end of the simulation, separate both times with a line break.\nFormat: YYYY-MM-DDThh:mm:ssZ");
            printf("For exit enter 0.\n");

            st = st ? st : read_line(stdin, &input);
            if (!st && strcmp(input, "0") == 0) {
                stage = GEN_STAGE_EXIT;
                break;
            }

            st = st ? st : read_line(stdin, &input2);

            if (st != ok) {
                break;
            }

            if (is_valid_time_ISO_8601(input) && is_valid_time_ISO_8601(input2) && strcmp(input, input2) <= 0) {
                st = st ? st : strcpy_with_malloc(&st_time, input);
                st = st ? st : strcpy_with_malloc(&end_time, input2);

                if (st != ok) {
                    break;
                }
                stage = GEN_STAGE_HANDLE_TIME;
            }
            else {
                printf("Invalid time!\n");
            }
            free_all(2, input, input2);
            input = NULL;
            input2 = NULL;
        }
        if (stage == GEN_STAGE_HANDLE_TIME) {
            printf("Input min and max handling times, separate both times with a line break.\nFormat: YYYY-MM-DDThh:mm:ssZ");
            printf("For exit input 0.\n");

            st = st ? st : read_line(stdin, &input);
            if (!st && strcmp(input, "0") == 0) {
                stage = GEN_STAGE_EXIT;
                break;
            }

            st = st ? st : read_line(stdin, &input2);

            if (st != ok) {
                break;
            }

            if (is_valid_uint(input) && is_valid_uint(input2)) {
                min_handle_time = atoi(input);
                max_handle_time = atoi(input2);

                if (errno == ERANGE || min_handle_time > max_handle_time) {
                    printf("Numbers are invalid!\n");
                }
                else {
                    stage = GEN_STAGE_DEP_CNT;
                }
            }
            else {
                printf("Invalid numbers!\n");
            }
            free_all(2, input, input2);
            input = NULL;
            input2 = NULL;
        }
        if (stage = GEN_STAGE_DEP_CNT) {
            printf("Input the amount of departments.\n");

            st = st ? st : read_line(stdin, &input);

            if (st != ok) {
                break;
            }

            if (is_valid_uint(input)) {
                dep_cnt = atoi(input);

                if (errno == ERANGE || dep_cnt < 1 || dep_cnt > 100) {
                    printf("Amount is out of range! It should be in range [1, 100].\n");
                }
                else {
                    stage = GEN_STAGE_DEP_INF;

                    dep_names = (char**)calloc(dep_cnt, sizeof(char*));
                    staff_cnts = (unsigned int*)calloc(dep_cnt, sizeof(unsigned int));

                    if (dep_names == NULL || staff_cnts == NULL) {
                        free(dep_names);
                        free(staff_cnts);

                        dep_names = NULL;
                        staff_cnts = NULL;
                    }
                }
            }
            else {
                printf("Invalid numbers!\n");
            }
            free(input);
            input = NULL;
        }
        if (stage = GEN_STAGE_DEP_INF) {
            printf("Enter department staff size ([10..50]) and names\n");
            
            for (size_t i = 0; !st && i < dep_cnt; ++i) {
                printf("name-%llu (E to exit): ", (ull) i);
                st = st ? st : read_word_from_file(stdin, &dep_names[i]);
                st = st ? st : (strcmp(dep_names[i], "E") ? st : GEN_STAGE_EXIT);
                
                for (size_t j = 0; !st && j < i; ++j) {
                    if (!strcmp(dep_names[i], dep_names[j])) {
                        printf("This name has already been entered\n");
                        st = INVALID_INPUT;
                    }
                }
                
                if (st == INVALID_INPUT) {
                    st = ok;
                    free(dep_names[i]);
                    dep_names[i] = NULL;
                    --i;
                }
            }
            
            for (size_t i = 0; !st && i < dep_cnt; ++i) {
                ull staff_cnt_tmp = 0;
                printf("staff-%llu (E to exit): ", (ull) i);
                st = st ? st : read_word_from_file(stdin, &input);
                st = st ? st : (strcmp(input, "E") ? st : GEN_STAGE_EXIT);
                st = st ? st : parse_ullong(input, 10, &staff_cnt_tmp);
                staff_cnts[i] = (size_t) staff_cnt_tmp;
                free(input);
                input = NULL;
                
                if (st == INVALID_INPUT || staff_cnts[i] < 10 || staff_cnts[i] > 50) {
                    printf("Invalid input\n");
                    st = ok;
                    --i;
                }
            }
            
            if (!st) {
                stage = GEN_STAGE_COEF;
            }
        }
        if (stage == GEN_STAGE_COEF) {
            
        }
    }

    if (stage == GEN_STAGE_EXIT) {
        free_all(7, heap_type,
            map_type,
            start_time,
            finish_time,
            min_handling_time,
            max_handling_time,
            deps_amount);

        for (int i = 0; i < deps_amount; i++) {

        }
    }
}