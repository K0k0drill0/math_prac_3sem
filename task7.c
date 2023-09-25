#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

static char *Error_names[] = {
    "Wrong amount of arguments.",
    "Wrong flag.",
    "Can not open a file."
};

enum Errors{
    ok = -1,
    WRONG_AMOUNT_OF_ARGUMENTS,
    WRONG_FLAG,
    ENABLE_TO_OPEN_FILE
};

int is_razd(char c) {
    return (c == ' ' || c == '\t' || c == '\n');
}

void wr_word(FILE* inp, FILE* outp, char *c) {
    while (!is_razd(*c) && *c != EOF){
        putc(*c, outp);
        *c = getc(inp);
    }
    putc(' ', outp);
}

int decimal_to_quaternary(int decimal) {
    int quaternary[32];
    int i = 0;
    while (decimal > 0) {
        int remainder = decimal % 4;
        quaternary[i] = remainder;
        i++;
        decimal /= 4;
    }
    int result = 0;
    for (int j = 0; j < i; j++) {
        result += quaternary[j] * (int)pow(10, j);
    }
    return result;
}

int decimal_to_octal(int decimal) {
    int octal[32];
    int i = 0;
    while (decimal > 0) {
        int remainder = decimal % 8;
        octal[i] = remainder;
        i++;
        decimal /= 8;
    }
    int result = 0;
    for (int j = 0; j < i; j++) {
        result += octal[j] * pow(10, j);
    }
    return result;
}

void func_r(FILE* inp1, FILE* inp2, FILE* outp) {
    char c1 = getc(inp1);
    char c2 = getc(inp2);
    while (c1 != EOF && c2 != EOF) {
        while (is_razd(c1)) {
            c1 = getc(inp1);
        }
        while (is_razd(c2)) {
            c2 = getc(inp2);
        }
        if (c1 == EOF && c2 == EOF) {
            break;
        }
        if (c1 == EOF) {
            wr_word(inp2, outp, &c2);
            continue;
        }
        if (c2 == EOF) {
            wr_word(inp1, outp, &c1);
            continue;
        }
        wr_word(inp1, outp, &c1);
        wr_word(inp2, outp, &c2);
    }
}

void first_cond(FILE* inp, FILE* outp, char* c) {
    while (!is_razd(*c) && *c != EOF) {
        if (isalpha(*c)) {
            *c = tolower(*c);
            fprintf(outp, "%d", decimal_to_quaternary((int)*c));
        }
        *c = getc(inp);
    }
    putc(' ', outp);
}

void second_condition(FILE* inp, FILE* outp, char* c) {
    while (!is_razd(*c) && *c != EOF){
        if (isalpha(*c)) {
            *c = tolower(*c);
        }
        putc(*c, outp);
        *c = getc(inp);
    }
    putc(' ', outp);
}

void third_condition(FILE* inp, FILE* outp, char* c) {
    while (!is_razd(*c) && *c != EOF){
        fprintf(outp, "%d", decimal_to_octal((int)*c));
        *c = getc(inp);
    }
    putc(' ', outp);
}

void func_a(FILE* inp, FILE* outp) {
    int cnt = 1;

    char c = getc(inp);
    while (c != EOF) {
        while (is_razd(c)) {
            c = getc(inp);
        }
        if (cnt != 0 && cnt % 10 == 0) {
            first_cond(inp, outp, &c);
            cnt++;
            continue;
        }
        if (cnt != 0 && !(cnt & 1)) {
            second_condition(inp, outp, &c);
            cnt++;
            continue;
        }
        if (cnt != 0 && cnt % 5 == 0) {
            third_condition(inp, outp, &c);
            cnt++;
            continue;
        }
        wr_word(inp, outp, &c);
        cnt++;
    }
}

int valid_args(int argc, char** argv) {
    if ((argc != 4 && argc != 5)) {
        return WRONG_AMOUNT_OF_ARGUMENTS;
    }
    if (strlen(argv[1]) != 2 || (argv[1][0] != '-' && argv[1][0] != '/') ||
     (argv[1][1] != 'r' && argv[1][1] != 'a')) {
        return WRONG_FLAG;
    }
    if ((argv[1][1] == 'r' && argc != 5) || (argv[1][1] == 'a' && argc != 4))  {
        return WRONG_AMOUNT_OF_ARGUMENTS;
    }
    return ok;
}

void pr_errors(int num) {
    switch (num)
    {
    case WRONG_AMOUNT_OF_ARGUMENTS:
        printf("%s\n", Error_names[WRONG_AMOUNT_OF_ARGUMENTS]);
        break;
    case WRONG_FLAG:
        printf("%s\n", Error_names[WRONG_FLAG]);
        break;
    case ENABLE_TO_OPEN_FILE:
        printf("%s\n", Error_names[ENABLE_TO_OPEN_FILE]);
        break;
    default:
        break;
    }
}

int main(int argc, char** argv) {
    if (valid_args(argc, argv) != ok) {
        pr_errors(valid_args(argc, argv));
        return 1;
    }

    FILE* inp1;
    FILE* inp2;
    FILE* outp;
    switch (argv[1][1])
    {
    case 'r':
        inp1 = fopen(argv[2], "r");
        inp2 = fopen(argv[3], "r");
        outp = fopen(argv[4], "w+");
        if (inp1 == NULL || inp2 == NULL || outp == NULL) {
            pr_errors(ENABLE_TO_OPEN_FILE);
            fclose(inp1);
            fclose(inp2);
            fclose(outp);
            return -2;
        }
        func_r(inp1, inp2, outp);
        break;
    case 'a':
        inp1 = fopen(argv[2], "r");
        outp = fopen(argv[3], "w+");
        if (inp1 == NULL || outp == NULL) {
            pr_errors(ENABLE_TO_OPEN_FILE);
            fclose(inp1);
            fclose(outp);
            return -2;
        }
        func_a(inp1, outp);
        break;
    default:
        break;
    }

}