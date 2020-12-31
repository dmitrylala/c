#include <stdio.h>
#include <string.h>

enum { MAXLEN = 256 };

int main(void) {
    char str[MAXLEN];
    int i;

    fgets(str, MAXLEN, stdin);
    int len = strlen(str);
    if (str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }

    char c;
    for (i = 0; i < (len - 1) / 2; ++i) {
        c = str[i];
        str[i] = str[len - 2 - i];
        str[len - 2 - i] = c;
    }

    printf("%s\n", str);
    return 0;
}