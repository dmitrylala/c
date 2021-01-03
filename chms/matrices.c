#include "matrices.h"


// matrix initialization from file
int
matrix_initf(Matrix *matrix, const int fd)
{
    // getting first string - there should be size
    char *str;
    ERR_MEM(str = str_in(fd),  WRONG_INPUT);

    // getting size of matrix
    int size;
    ERR_ACT(sscanf(str, "%d", &size), 1, WRONG_INPUT, free(str));
    free(str);
    if (size <= 0) return WRONG_INPUT;

    // allocating memory for matrix
    ERR_MEM(matrix -> data = malloc(size * sizeof *(matrix -> data)),
            ERR_MEM_ALLOC);
    matrix -> size = size;
    for (size_t i = 0; i < size; ++i) {
        ERR_MEM(matrix -> data[i] = malloc(size * sizeof **(matrix -> data)),
                ERR_MEM_ALLOC);
    }

    // getting values
    for (size_t i = 0; i < size; ++i) {
        ERR_ACT_EQ(str = str_in(fd),  NULL, WRONG_INPUT, matrix_final(matrix));

        char *str1 = str;
        str1 = strtok(str1, " \0");
        for (size_t j = 0; j < size; ++j) {
            ERR_ACTS(sscanf(str1, "%lf", &(matrix -> data[i][j])), 1, WRONG_INPUT,
                    free(str), matrix_final(matrix));
            str1 = strtok(NULL, " \0");
        }
        free(str);
    }
    return 0;
}

// functions that prints matrix to stdout
// considering that matrix was initialized
int
matrix_print(Matrix matrix)
{
    printf("Matrix:\n");
    size_t size = matrix.size;
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            printf("%5lf     ", matrix.data[i][j]);
        }
        printf("\n");
    }
    return 0;
}


// function that frees allocated for matrix memory
int
matrix_final(Matrix *matrix)
{
    for (size_t i = 0; i < matrix -> size; ++i) {
        free(matrix -> data[i]);
    }
    free(matrix -> data);
    return 0;
}
