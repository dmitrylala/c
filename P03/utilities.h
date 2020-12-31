#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "macroses.h"


// errors
#define ERR_MEM_ALLOC 18


char *
str_in(const int fd);

char *
str_clear_spaces(char *str);