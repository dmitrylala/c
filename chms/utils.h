#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "macroses.h"


// errors
#define ERR_MEM_ALLOC 1
#define SYS_ERR 2
#define CRIT_ERR 3


// functions
char *
str_in(const int fd);
