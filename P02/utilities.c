#include "utilities.h"


// Function returns the sum of int array's elements, n is array's length
int
arr_calc_sum(const int *arr, const size_t n)
{
    int sum = 0;
    for (size_t i = 0; i < n; ++i) {
        sum += arr[i];
    }
    return sum;
}

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

// Function that counts the number of occurrences of substring within string
int
substr_c(const char *str, const char *substr)
{
    int c = 0;
    size_t len_sub = strlen(substr);
    for (size_t i = 0; i < strlen(str); ++i) {
        if (str[i] == substr[0]) {
            if (strncmp(&str[i], substr, len_sub) == 0) {
                ++c;
                i += len_sub - 1;
            }
        }
    }
    return c;
}

// Function that computes the int matrix size n x n of shortest distances
int
matrix_shortest_dists_compute(Matrix *matrix, const int n, int *max_sum)
{
    int i, j, k;

    // Calculating the sum of all paths
    for (i = 0 ; i < n ; ++i) {
        for (j = 0 ; j < n ; ++j) {
            *max_sum += (*matrix)[i][j];
        }
    }

    // All paths that doesn't exist are set to max_sum (except paths node->node)
    for (i = 0 ; i < n ; ++i) {
        for (j = 0 ; j < n ; ++j) {
            if (((*matrix)[i][j] == 0) && (i != j)) {
                (*matrix)[i][j] = *max_sum;
            }
        }
    }

    // Calculating the matrix
    for (k = 0 ; k < n; ++k) {
        for (i = 0 ; i < n; ++i) {
            for (j = 0 ; j < n ; ++j) {
                if (((*matrix)[i][k] + (*matrix)[k][j]) < (*matrix)[i][j]) {
                    (*matrix)[i][j] = (*matrix)[i][k] + (*matrix)[k][j];
                }
            }
        }
    }

    return 0;
}

// Function that frees allocated for int matrix size n x n memory
int
matrix_free(Matrix *matrix, const int n)
{
    for (int i = 0; i < n; ++i) {
        free((*matrix)[i]);
    }
    free(*matrix);
    return 0;
}