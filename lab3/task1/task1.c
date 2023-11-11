#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>

enum status_codes {
    ok,
    INVALID_PARAMETER
};

void reverse(char* arr, int arr_size) {
    for (int i = 0; i <= (arr_size - 1) / 2; i++) {
        char tmp = arr[i];
        arr[i] = arr[arr_size - 1 - i];
        arr[arr_size - 1 - i] = tmp;
    }
}

int increment(int* a)
{
	if (a == NULL)
	{
		return 0;
	}
	int mask = 1;
	int c = 1; // carry digit
	while (*a & mask)
	{
		*a ^= c;
		c <<= 1;
		mask <<= 1;
	}
	*a |= c;
	return *a;
}

int decrement(int* a)
{
	if (a == NULL)
	{
		return 0;
	}
	int mask = 1;
	int c = 1; // carry digit
	while (~(*a) & mask)
	{
		*a ^= c;
		c <<= 1;
		mask <<= 1;
	}
	*a ^= c;
	return *a;
}


int dec_to_base(int num, const int r, char* ans) {
    if (r < 1 || r > 5) return INVALID_PARAMETER; 
    char alphabet[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    if (num == 0) {
        ans[0] = '0';
        ans[1] = '\0';
        return ok;
    }

    int sign = (num >= 0) ? 1 : -1;
    num = abs(num);

    int ans_ind = 0; 
    int two_pow = 1 << r;
    int mask = decrement(&two_pow);//subtractOne((1 << r)); 
    //(1 << r) - 1;
    while (num > 0) {
        int digit = num & mask;
        ans[ans_ind] = alphabet[digit];
        //ans_ind++;
        ans_ind = increment(&ans_ind);
        num >>= r;
    }
    if (sign == -1) {
        ans[ans_ind] = '-';
        ans_ind = increment(&ans_ind);
        //ans_ind++;
    }
    ans[ans_ind] = '\0';
    reverse(ans, ans_ind);
    return ok;
}


int main() {
    // int lol = 8;
    // printf("%d\n", subtractOne(lol));
    // return 0;
    
    int num = 32;
    char ans_2[33];
    char ans_4[33];
    char ans_8[33];
    char ans_16[33];
    char ans_32[33];
    dec_to_base(num, 1, ans_2);
    dec_to_base(num, 2, ans_4);
    dec_to_base(num, 3, ans_8);
    dec_to_base(num, 4, ans_16);
    dec_to_base(num, 5, ans_32);
    printf("Decimal: %d\n", num);
    printf("base 2: %s\n", ans_2);
    printf("base 4: %s\n", ans_4);
    printf("base 8: %s\n", ans_8);
    printf("base 16: %s\n", ans_16);
    printf("base 32: %s\n", ans_32);
    return 0;
}
