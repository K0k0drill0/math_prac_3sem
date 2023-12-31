#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include "ultra_header.h"

int is_separator(char c) {
    return (c == ' ' || c == '\t' || c == '\n');
}

int get_word_from_file(FILE* inp, FILE* outp, char** str_inp) {
    int arr_max_size = 16;
    *str_inp = (char*)malloc(sizeof(char) * arr_max_size);
    if (*str_inp == NULL) {
        return MEMORY_ISSUES;
    }

    char c;
    c = getc(inp);
    int ind = 0;
    while (is_separator(c) && c != EOF) {
        putc(c, outp);
        c = getc(inp);
    }

    if (c == EOF) {
        //free(*str_inp);
        return EOF;
    }

    while (!is_separator(c) && c != EOF) {
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
    
    if (is_separator(c)) {
        fseek(inp, -1, SEEK_CUR);
    }

    (*str_inp)[ind] = '\0';
    return ok;
}

int read_line(FILE* inp, FILE* outp, char** line) {
    int arr_max_size = 16;
    *line = (char*)malloc(sizeof(char) * arr_max_size);
    if (*line == NULL) {
        return MEMORY_ISSUES;
    }

    char c = getc(inp);

    int ind = 0;
    while (c != '\n' && c != EOF) {
        if (ind >= arr_max_size - 1) {
            arr_max_size *= 2;
            char *tmp = (char*)realloc(*line, arr_max_size * sizeof(char));
            if (tmp == NULL) {
                //free(str_inp);
                return MEMORY_ISSUES;
            }
            *line = tmp;
        }
        (*line)[ind] = c;
        ind++;
        c = getc(inp);
    }

    (*line)[ind] = '\0';

    if (c == EOF && ind == 0) {
        return EOF;
    }

    return ok;
}

int is_valid_key(char* str) {
    int ind = 0;
    while (str[ind] != '\0') {
        if (!isalpha(str[ind]) && !isdigit(str[ind])) {
            return 0;
        }
        ind++;
    }
    if (ind == 0) {
        return 0;
    }
    return 1;
}

int get_defines(FILE* inp, FILE* outp, char** key, char** value) {
    char* line = NULL;
    int st = read_line(inp, outp, &line);
    if (line != NULL) {
        int ind = 0;
        while (line[ind] != '\0') {
            putc(line[ind], outp);
            ind++;
        }
        putc('\n', outp);
    }
    if (st != ok) {
        free(line);
        return st;
    }

    int ind = 0;
    while (line[ind] == ' ' || line[ind] == '\t') {
        ind++;
    }

    if (line[ind] == '\0') {
        free(line);
        st = get_defines(inp, outp, key, value);
        return st;
    }

    int n = strlen(line);

    char* def = (char*)malloc(sizeof(char) * (n+1));
    if (def == NULL) {
        free(line);
        return MEMORY_ISSUES;
    }

    int def_ind = 0;
    while (line[ind] != ' ' && line[ind] != '\t' && line[ind] != '\0') {
        def[def_ind] = line[ind];
        ind++;
        def_ind++;
    }
    def[def_ind] = '\0';

    if (strcmp(def, "#define")) {
        free(line);
        free(def);
        fseek(outp, -def_ind, SEEK_CUR);
        fseek(inp, -def_ind, SEEK_CUR);
        return NO_DEFINE;
    }

    free(def);

    *key = (char*)malloc(sizeof(char) * (n+1));
    if (*key == NULL) {
        free(line);
        return MEMORY_ISSUES;
    }

    while (line[ind] == ' ' || line[ind] == '\t') {
        ind++;
    }

    int key_ind = 0;
    while (line[ind] != ' ' && line[ind] != '\t' && line[ind] != '\0') {
        (*key)[key_ind] = line[ind];
        ind++;
        key_ind++;
    }
    (*key)[key_ind] = '\0';
    if (!is_valid_key(*key)) {
        free(line);
        return INVALID_DEFINE;
    }


    *value = (char*)malloc(sizeof(char) * (n+1));
    if (*value == NULL) {
        free(line);
        return MEMORY_ISSUES;
    }

    while (line[ind] == ' ' || line[ind] == '\t') {
        ind++;
    }

    int val_ind = 0;
    while (line[ind] != ' ' && line[ind] != '\t' && line[ind] != '\0') {
        (*value)[val_ind] = line[ind];
        ind++;
        val_ind++;
    }
    (*value)[val_ind] = '\0';

    free(line);
    return ok;
}

int fill_hashtable(FILE* inp, FILE* outp, HashTable** ht, Cache** cache) {
    char* key = NULL, *value = NULL;

    int st = ok;
    while ((st = get_defines(inp, outp, &key, &value)) == ok) {
        st = insert_hashtable(ht, cache, key, value);
        if (st != ok) {
            free(key);
            free(value);
            return st;
        }
    }
    if (st == NO_DEFINE || st == EOF) {
        return ok;
    }
    else {
        free(key);
        return st;
    }

}

void write_word(FILE* outp, char* str) {
    int ind = 0;
    while (str[ind] != '\0') {
        putc(str[ind], outp);
        ind++;
    }
}

int proccess_file(FILE* inp, FILE* outp) { 
    HashTable* ht = NULL;
    Cache* cache = NULL;
    int st = create_hashtable(&ht, 128, hash);
    if (st != ok) {
        return st;
    }
    st = fill_hashtable(inp, outp, &ht, &cache);
    if (st != ok) {
        delete_hashtable(ht);
        delete_cache_list(cache);
        return st;
    }

    char* word = NULL;
    st = ok;
    while ((st = get_word_from_file(inp, outp, &word)) == ok) {
        if (is_valid_key(word)) {
            char* value = NULL;
            if ((value = find_value_hashtable(ht, word)) == NULL) {
                write_word(outp, word);
            }
            else {
                write_word(outp, value);
            }
        }
        else {
            write_word(outp, word);
        }
        free(word);
    }
    free(word);


    //print_hashtable(ht);
    //print_cache_list(cache);
    delete_hashtable(ht);
    delete_cache_list(cache);

    if (st != ok) {
        if (st != EOF) {
            return st;
        }
        else {
            return ok;
        }
    }
    return ok;
}

void copy_file(FILE* dest, FILE* source) {
    char c = getc(source);
    while (c != EOF) {
        putc(c, dest);
        c = getc(source);
    }
}


int main(int argc, char** argv) {
    argc = 2;
    argv[1] = "input.txt";
    if (argc != 2) {
        print_error(INVALID_ARGUMENTS);
        return 1;
    }

    FILE* inp = fopen(argv[1], "r");
    if (inp == NULL) {
        print_error(UNABLE_TO_OPEN_A_FILE);
        return 1;
    }
    FILE* outp = tmpfile();
    if (outp == NULL) {
        fclose(inp);
        print_error(UNABLE_TO_OPEN_A_FILE);
        return 1;
    }

    int st = proccess_file(inp, outp);
    if (st != ok) {
        fclose(inp);
        fclose(outp);
        print_error(st);
        return 1;
    }
    fclose(inp);

    inp = fopen(argv[1], "w+");
    if (inp == NULL) {
        fclose(outp);
        print_error(UNABLE_TO_OPEN_A_FILE);
        return 1;
    }
    fseek(outp, 0, SEEK_SET);
    copy_file(inp, outp);

    fclose(inp);
    fclose(outp);

    return 0;
}