#include "utilities.h"


void
invitation(void)
{
    printf("\033[1;30;91m");

    // let's assume that current path suits into string with length = 100
    char s[100];
    char *res = NULL;
    res = getcwd(s, 100);
    printf("%s", res);
    printf("\x1b[0m");
    printf("\x1b[96m");
    printf("\u27A4 ");
    printf("\x1b[0m");
    fflush(stdout);
}

char *
str_clear_spaces_begin(char *str)
{

    int begin = 0;
    size_t len = strlen(str);

    for(int i = 0; i < len; ++i) {
        if ((str[i] != ' ') && (str[i] != '\t')) {
            begin = i;
            break;
        }
    }

    if (begin == 0) {
        return str;
    }

    for (int i = begin; i < len; ++i) {
        str[i - begin] = str[i];
    }
    str[len - begin] = '\0';

    return str;
}

int
str_check_spaces(const char *str)
{
    int count = 0, flag = 0;
    for(int i = 0; i < strlen(str); ++i) {
        if ((str[i] == ' ') || (str[i] == '\t')) {
            count++;
        }
    }

    if (count == strlen(str)) {
        flag = 1;
    }

    return flag;
}

char *
str_clear_spaces(char *str)
{
    int end = 0;
    for(int i = 0; i < strlen(str); ++i) {
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

char **
get_cmd(char *str)
{

    char **cmd = NULL;

    char *space = " \0";
    str = strtok(str, space);

    int count = 0;
    do {
        cmd = realloc(cmd, sizeof(*cmd) * (count + 1));

        if (cmd == NULL) {
            return NULL;
        }

        cmd[count] = strdup(str);
        ++count;

    } while ((str = strtok(NULL, space)) != NULL);

    cmd = realloc(cmd, (count + 1) * sizeof *cmd);
    cmd[count] = NULL;

    return cmd;
}

int
free_cmd(char **cmd)
{
    int i = 0;
    while (cmd[i] != NULL) {
        free(cmd[i]);
        i++;
    }

    free(cmd);
    cmd = NULL;
    return 0;
}
