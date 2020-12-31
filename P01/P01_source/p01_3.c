#include <stdio.h>

void print_bit_short(short input) {
    unsigned short mask = 1 << 15;
    while(mask != 0) {
        printf("%d", (input & mask) == 0 ? 0 : 1);
        mask = mask >> 1;
    }
    printf("\n");
}

void print_bit_int(int input) {
    unsigned int mask = 1 << 31;
    while (mask != 0) {
        printf("%d", (input & mask) == 0 ? 0 : 1);
        mask = mask >> 1;
    }
    printf("\n");
}

int main(void) {
    printf("short = %ld bytes\n", sizeof(short));
    printf("int = %ld bytes\n", sizeof(int));

    // M < N
    unsigned short i = 255;

    print_bit_short(i);
    print_bit_int((int)i);

    // M = N (where is the point?)
    unsigned int j = 1023;

    print_bit_int(j);
    print_bit_int((int)j);

    // M > N
    unsigned int k = 12345678;

    print_bit_int(k);
    print_bit_short((short)k);

    return 0;
}