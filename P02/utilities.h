#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "macroses.h"


#define ERR_MEM_ALLOC 8


typedef int** Matrix;


int
arr_calc_sum(const int *arr, const size_t n);

char *
str_in(const int fd);

char *
str_clear_spaces(char *str);

int
substr_c(const char *str, const char *substr);

int
matrix_shortest_dists_compute(Matrix *matrix, const int n, int *max_sum);

int
matrix_free(Matrix *matrix, const int n);
