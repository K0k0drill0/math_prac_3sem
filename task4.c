#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void func_d(FILE* inp, FILE* outp) {
    char c = fgetc(inp);
    while (c != EOF) {
        if (!isdigit(c)) {
            fputc(c, outp);
        }
        c = fgetc(inp);
    }
}

void func_i(FILE* inp, FILE* outp) {
    char c = fgetc(inp);
    int counter = 0;
    char pr_am[50];
    while (c != EOF) {
        if (c == '\n') {
            sprintf(pr_am, " (amount of letters = %d)\n", counter);
            fputs(pr_am, outp);
            c = fgetc(inp);
            counter = 0;
            continue;
        }
        if (isalpha(c)) {
            counter++;
        }
        fputc(c, outp);
        c = getc(inp);
    }
    sprintf(pr_am, " (amount of letters = %d)\n", counter);
    fputs(pr_am, outp);
}

void func_s(FILE* inp, FILE* outp) {
    char c = fgetc(inp);
    int counter = 0;
    char pr_am[100];
    while (c != EOF) {
        if (c == '\n') {
            sprintf(pr_am, " (amount of symbols, that are not a number, letter or space = %d)\n", counter);
            fputs(pr_am, outp);
            c = fgetc(inp);
            counter = 0;
            continue;
        }
        if (!isalpha(c) && !isdigit(c) && c != ' ') {
            counter++;
        }
        fputc(c, outp);
        c = getc(inp);
    }
    sprintf(pr_am, " (amount of symbols, that are not a number, letter or space = %d)\n", counter);
    fputs(pr_am, outp);
}

void decimal_to_hexadecimal(int decimal, char* hexadecimal) {
    int remainder, quotient;
    //char hexadecimal[100];
    int i = 1, j;

    quotient = decimal;

    while (quotient != 0) {
        remainder = quotient % 16;

        if (remainder < 10)
            hexadecimal[i++] = 48 + remainder;
        else
            hexadecimal[i++] = 55 + remainder;

        quotient = quotient / 16;
    }

    int cnt = 0;
    char ans[100];
    for (j = i - 1; j > 0; j--) {
        ans[cnt] = hexadecimal[j];
        cnt++;
    }
    ans[cnt] = '\0';
    for (int i = 0; i < strlen(ans); i++) {
        hexadecimal[i] = ans[i];
    }
    hexadecimal[cnt] = '\0';
    //free(ans);
}

void func_a(FILE* inp, FILE* outp)  {
    char c = fgetc(inp);
    char pr_am[50];
    while (c != EOF) {
        if (!isalpha(c) && !isdigit(c) && c != ' ') {
            char new[100];
            decimal_to_hexadecimal((int)c, new);
            fputs(new, outp);
            c = fgetc(inp);
            continue;
        }
        fputc(c, outp);
        c = fgetc(inp);
    }
}

int main(int argc, char** argv) {
    if (argc != 3 && argc != 4) {
        printf("Error: wrong amount of arguments.\n");
        return 0;
    }
    if ((strlen(argv[1]) != 2 && strlen(argv[1]) != 3) ||
     (strlen(argv[1]) == 3 && argv[1][1] != 'n') || (strlen(argv[1]) == 3
      && argv[1][2] != 'd' && argv[1][2] != 'i' && argv[1][2] != 's' &&
       argv[1][2] != 'a') || (strlen(argv[1]) == 2
      && argv[1][1] != 'd' && argv[1][1] != 'i' && argv[1][1] != 's' &&
       argv[1][1] != 'a')) {
        printf("Error: wrong flag.\n");
        return 0;
    }
    if (strlen(argv[1]) == 3 && argc != 4) {
        printf("Error: wrong amount of arguments.\n");
        return 0;
    }

    FILE* inp;
    FILE* outp;
    inp = fopen(argv[2], "r");
    if (inp == NULL) {
        printf("Error: opening file.\n");
        return 0;
    }
    if (argc == 3) {
        char *filename = argv[2];
        char *prefix = "out_";
        char new_filename[100];

        // Find the last occurrence of '/' or '\' to get the filename
        char *last_slash = strrchr(filename, '/');
        char *last_backslash = strrchr(filename, '\\');
        char *last_sep = (last_slash > last_backslash) ? last_slash : last_backslash;
        if (last_sep != NULL) {
            filename = last_sep + 1;
        }

        // Create the new filename with the prefix
        sprintf(new_filename, "%s%s", prefix, filename);
        outp = fopen(new_filename, "w+");
    }
    else {
        outp = fopen(argv[3], "w+");
    }

    if (outp == NULL) {
        printf("Error: opening file.\n");
        return 0;
    }

    if (strlen(argv[1]) == 2) {
        switch (argv[1][1])
        {
        case 'd':
            func_d(inp, outp);
            break;
        case 'i':
            func_i(inp, outp);
            break;
        case 's':
            func_s(inp, outp);
            break;
        case 'a':
            func_a(inp, outp);
            break;
        default:
            break;
        }
    }
    else {
        switch (argv[1][2])
        {
        case 'd':
            func_d(inp, outp);
            break;
        case 'i':
            func_i(inp, outp);
            break;
        case 's':
            func_s(inp, outp);
            break;
        case 'a':
            func_a(inp, outp);
            break;
        default:
            break;
        }
    }
}
