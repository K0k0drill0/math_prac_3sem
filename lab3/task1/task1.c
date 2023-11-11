#include <stdio.h>
#include <stdlib.h>
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

#include <stdio.h>

int subtractOne(int num) {
    int mask = 1;
    int bit_cnt = 0;
    while (num > 0) {
        num >>= 1;
        bit_cnt++;
    }
    while (bit_cnt != 2) {
        num ^= 1;
        num <<= 1;
        bit_cnt--;
    }
    num ^= 1;
    return num;
}


int dec_to_base(int num, int r, char* ans) {
    if (r < 1 || r > 5) return INVALID_PARAMETER; 
    char alphabet[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    int sign = (num >= 0) ? 1 : -1;
    num = abs(num);

    int ans_ind = 0; 
    int mask = subtractOne((1 << r)); 
    //(1 << r) - 1;
    while (num > 0) {
        int digit = num & mask;
        ans[ans_ind] = alphabet[digit];
        ans_ind++;

        num >>= r;
    }
    if (sign == -1) {
        ans[ans_ind] = '-';
        ans_ind++;
    }
    ans[ans_ind] = '\0';
    reverse(ans, ans_ind);
    return ok;
}


int main() {
    // int lol = 8;
    // printf("%d\n", subtractOne(lol));
    // return 0;

    unsigned int num = 35;
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
