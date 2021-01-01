#include <stdio.h>

int
main(void)
{
    int count = 0, c, i = 0;

    while ((c = getchar()) != EOF) {
        if ((c == '\n') && (i == 0)) {
            ++count;
        } else if ((c == '\n') && (i != 0)) {
            i = 0;
        } else {
            ++i;
        }
    }
    printf("%d", count);
    return 0;
}