#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


// function that prints invitation string
void
invitation(void);


// strings processing
char *
str_clear_spaces_begin(char *str);

int
str_check_spaces(const char *str);

char *
str_clear_spaces(char *str);


// arrays of strings processing
char **
get_cmd(char *str);

int
free_cmd(char **cmd);
