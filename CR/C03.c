#include <stdio.h>
#include <string.h>

enum { MAXLEN = 82 };

int main(void) {
    int max = 0;
    char s[MAXLEN] = "\0";
    char temp[MAXLEN], res[MAXLEN] = "\0";

    while (fgets(s, MAXLEN, stdin)) {
        strcpy(temp, s);
        if (strlen(temp) > max) {
            max = strlen (temp);
            strcpy(res, temp);
        }
    }

    printf("%s", res);
    return 0;
}