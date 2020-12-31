#include "utilities.h"


// Input string of unknown length from already opened fd
char *
str_in(const int fd)
{

    int len = 1;
    size_t str_step = 200;
    char *str;
    ERR_MEM(str = malloc(str_step * sizeof *str), NULL);


    char c;
    read(fd, &c, sizeof c);
    while ((c != '\n') && (c != EOF)) {
        str[len - 1] = c;
        ++len;

        if (len > str_step) {
            str_step += 200;
            ERR_MEM(str = realloc(str, str_step * sizeof *str), NULL);
        }

        read(fd, &c, sizeof c);
    }
    str[len - 1] = '\0';

    if (len == 1) {
        free(str);
        return NULL;
    }

    return str;
}

// Function that clears all spaces and tabs in string
char *
str_clear_spaces(char *str)
{
    size_t end = 0;
    for (size_t i = 0; i < strlen(str); ++i) {
        if ((str[i] != ' ') && (str[i] != '\t')) {
            if (i != end) {
                str[end] = str[i];
            }
            ++end;
        }
    }
    str[end] = '\0';
    return str;
}