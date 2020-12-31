#include <stdio.h>

int main(void) {
    int i1, i2, i3, i4;
    scanf("%d%d%d%d", &i1, &i2, &i3, &i4);
    char c1 = i1, c2 = i2, c3 = i3, c4 = i4;
    unsigned p;

    p = (unsigned int) (unsigned char) c1 +
        (unsigned int) (unsigned char) c2 * (1 << 8) +
        (unsigned int) (unsigned char) c3 * (1 << 16) +
        (unsigned int) (unsigned char) c4 * (1 << 24);

    printf("%u\n", p);
    return 0;
}