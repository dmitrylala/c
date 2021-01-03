#include "utils.h"


// errors
#define WRONG_INPUT 4
#define BAD_FD 5


// matrix struct type
typedef struct
{
    double **data;
    size_t size;
} Matrix;


// functions
int
matrix_initf(Matrix *matrix, const int fd);

int
matrix_print(Matrix matrix);

int
matrix_final(Matrix *matrix);
