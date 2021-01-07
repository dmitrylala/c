#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "macroses.h"


// functions
char *
str_in(const int fd);

double
get_rand(void);

double
get_rand_range(const double min, const double max);
