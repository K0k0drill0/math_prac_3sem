#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <math.h>

unsigned char* dump_memory(void* num, int size) {
    unsigned char* ans = (char*)malloc(sizeof(char) * size * 9);
    if (ans == NULL) {
        return NULL;
    }
    unsigned char* ptr = (unsigned char*)num;
    int ind = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 7; j >= 0; j--) {
            unsigned char bit = ((*ptr) >> j) & 1;
            ans[ind] = ((int)bit) + '0';
            ind++;
        }
        ans[ind] = ' ';
        ind++;
        ptr++;
    }
    ans[ind] = '\0';
    return ans;
}

int which_number(char c) {
    if (isdigit(c)) {
        return (int)c - 48;
    }
    if (isalpha(c)) {
        c = toupper(c);
        return (int)c - 55;
    }
    return -1;
}

char* my_itoa(int decimal, int new_sn) {  
    const char all_symbols[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z'
    };

    char* ans;
    int arr_size = 0;
    int sign = 1;

    if (new_sn < 2 || new_sn > 36) {
        new_sn = 10;
    }

    if (decimal < 0) {
        sign = -1;
        arr_size++;
    }
    decimal = abs(decimal);
    if (decimal == 0) {
        arr_size = 2;
    }
    else {
        arr_size += log(decimal) / log(new_sn) + 1;
    }

    ans = (char*)malloc(sizeof(char) * arr_size);
    if (ans == NULL) {
        return NULL;
    }

    if (decimal == 0) {
        ans[0] = '0';
        ans[1] = '\0';
        return ans;
    }

    int i = arr_size-1;
    while (decimal > 0) {
        char remainder = all_symbols[decimal % new_sn];
        ans[i] = remainder;
        i--;
        decimal /= new_sn;
    }
    if (sign == -1) {
        ans[i] = '-';
    } 
    ans[arr_size] = '\0';

    return ans;
}

int check_input_for_to_decimal(char* str, int (*f)(int)) {
    if (str == NULL) {
        return 0;
    }
    int ind = 0;
    while (str[ind] != '\0') {
        char tmp = f(str[ind]);
        if (str[ind] != tmp || (!isalpha(str[ind]) && !isdigit(str[ind])))  {
            return 0;
        }
        ind++;
    }
    if (ind == 0) {
        return 0;
    }
    return 1;
}

char* to_decimal(char *str_old, int notation) {
    if (str_old == NULL) {
        return NULL;
    }
    char* ans;
    if (notation < 2 || notation > 36) {
        notation = 10;
    } 

    int num_new = 0;
    int base_power = 1;
    int sign = 1;
    if (str_old[0] == '-') sign = -1;
    for (int i = strlen(str_old) - 1; i >= 0; i--) {
        if (str_old[i] == '-') continue;
        int dig = which_number(str_old[i]);
        if (dig >= notation) {
            return NULL;
        }

        if (INT_MAX - base_power * dig < num_new) {
            return NULL;
        }
        num_new += base_power * dig;

        if (INT_MAX / notation < base_power) {
            return NULL;
        }
        base_power *= notation;
    }
    num_new *= sign;
    
    ans = (char*)malloc(sizeof(char) * 12);
    if (ans == NULL) {
        return ans;
    }

    ans = my_itoa(num_new, 10);
    return ans;
}

char* Zeckendorf(unsigned int num) {
    unsigned int arr_max_size = 16;
    //unsigned int arr_tmp_size = 0;
    unsigned int* fib = (unsigned int*)malloc(sizeof(unsigned int) * arr_max_size);
    if (fib == NULL) {
        return NULL;
    }

    fib[0] = 0;
    fib[1] = 1;
    int ind = 1;
    while (fib[ind] < num) {
        ind++;
        if (ind > arr_max_size) {
            arr_max_size *= 2;
            int* tmp = (int*)realloc(fib, arr_max_size);
            if (tmp == NULL) {
                free(fib);
                return NULL;
            }
            fib = tmp;
        }
        fib[ind] = fib[ind-1] + fib[ind-2];
    }

    int num1 = num;
    for (int i = ind; i > 0; i--) {
        //printf("%d, %d, %d, %d\n", num1, fib[i], num1 - fib[i], (num1 - fib[i]) >= 0);
        int razn = num1 - fib[i];
        if (razn >= 0) {
            num1 -= fib[i];
            fib[i] = 0;
        }
        if (num1 == 0) {
            break;
        }
    }

    char* ans = (char*)malloc(sizeof(char) * (ind + 3));
    if (ans == NULL) {
        free(fib);
        return NULL;
    }
    for (int i = 0; i <= ind; i++) {
        ans[i] = (fib[i] == 0) ? '1' : '0';
    }
    if (ans[ind] == '0') {
        ans[ind] = '1';
        ans[ind+1] = '\0';
    }
    else {
        ans[ind+1] = '1';
        ans[ind+2] = '\0';
    }


    free(fib);
    return ans;
}

char* intToRoman(int num) {
    char* roman = malloc(sizeof(char) * 16);
    if (roman == NULL || num > 3999 || num < 1) {
        return NULL;
    }

    int values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    char* symbols[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    int i = 0;
    while (num > 0) {
        if (num >= values[i]) {
            strcat(roman, symbols[i]);
            num -= values[i];
        } else {
            i++;
        }
    }
    return roman;
}

char* cut(char* str, int ind1, int ind2) {
	char* ans = (char*)malloc(sizeof(char) * (ind2 - ind1 + 1));
	if (ans == NULL) {
		return NULL;
	}

	for (int i = ind1; i <= ind2; i++) {
		ans[i-ind1] = str[i];
	}
	ans[ind2 - ind1 + 1] = '\0';
	return ans;
}    

int put_string(FILE* stream, char* str) {
    if (str == NULL) {
        return 0;
    }
    int ind = 0;
    while (str[ind] != '\0' && fputc(str[ind], stream) != EOF) {
        ind++;
    }
    return ind;
} 

int overfprintf(FILE* stream, char* format, ...) {
    va_list args;
    va_start(args, format);
    int i, j = 0, num, ind2;
    int len = strlen(format);
    char* str;
    for (i = 0; i < len; i++) {
        if (format[i] == '%' && i < len - 1) {
            int not_standard = 1;
            if (i < len-2 && format[i+1] == 'R' && format[i+2] == 'o') {
                num = va_arg(args, int);
                str = intToRoman(num);
            }
            else if (i < len-2 && format[i+1] == 'Z' && format[i+2] == 'r') {
                num = va_arg(args, unsigned int);
                str = Zeckendorf(num);
            } 
            else if (i < len-2 && format[i+1] == 'C' && format[i+2] == 'v') {
                num = va_arg(args, int);
                int base = va_arg(args, int);
                str = my_itoa(num, base);
            }
            else if (i < len-2 && format[i+1] == 'C' && format[i+2] == 'V') {
                num = va_arg(args, int);
                int base = va_arg(args, int);
                str = my_itoa(num, base);
                int ind = 0;
                while (str[ind] != '\0') {
                    str[ind] = toupper(str[ind]);
                    ind++;
                }
            }
            else if (i < len-2 && format[i+1] == 't' && format[i+2] == 'o') {
                char* fir_arg = va_arg(args, char*);
                int sec_arg = va_arg(args, int);
                if (!check_input_for_to_decimal(fir_arg, tolower)) {
                    str = NULL;
                }
                else {
                    str = to_decimal(fir_arg, sec_arg);
                }
            }
            else if (i < len-2 && format[i+1] == 'T' && format[i+2] == 'O') {
                char* fir_arg = va_arg(args, char*);
                int sec_arg = va_arg(args, int);
                if (!check_input_for_to_decimal(fir_arg, toupper)) {
                    str = NULL;
                }
                else {
                    str = to_decimal(fir_arg, sec_arg);
                }
            }
            else if (i < len-2 && format[i+1] == 'm' && format[i+2] == 'i') {
                signed int num = va_arg(args, signed int);
                str = dump_memory(&num, sizeof(signed int));
            }
            else if (i < len-2 && format[i+1] == 'm' && format[i+2] == 'u') {
                unsigned int num = va_arg(args, unsigned int);
                str = dump_memory(&num, sizeof(unsigned int));
            }
            else if (i < len-2 && format[i+1] == 'm' && format[i+2] == 'd') {
                double num = va_arg(args, double);
                str = dump_memory(&num, sizeof(double));
            }
            else if (i < len-2 && format[i+1] == 'm' && format[i+2] == 'f') {
                float num = va_arg(args, double);
                str = dump_memory(&num, sizeof(float));
            }
            else {
                not_standard = 0;
                ind2 = i;
                while (!isalpha(format[ind2]) && format[ind2] != '%' && format[ind2] != '\0') {
                    ind2++;
                }

				str = cut(format, i, ind2);
            }
            if (not_standard) {
                j += put_string(stream, str);
                i += 2;
            }
            else {
                int am = vfprintf(stream, str, args);
                va_arg(args, void*);
                j += (am < 0) ? 0 : am;
				i = ind2;
            }
            free(str);
        } else {
            if (fputc(format[i], stream) != EOF) {
                j++;
            }
        }
    }
    va_end(args);
    return j;
}



int main() {
    FILE* outp;
    outp = fopen("output.txt", "w+");
    if (outp == NULL) {
        return -1;
    }

    int st = overfprintf(outp, "%d, cock, %to, %Zr", 12, "aa", 16, 11);
    printf("%d\n", st);
    
    return 0;
}