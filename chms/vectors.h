#include <math.h>

#include "utils.h"


// vector struct type
typedef struct
{
    double *data;
    size_t size;
} Vector;


// functions
int
vector_initf(Vector *vector, const int fd);

int
vector_generate(Vector *matrix, const size_t size, const double min,
                const double max);

int
vector_dot(const Vector vector1, const Vector vector2, double *res);

int
vector_dist(const Vector vector1, const Vector vector2, double *res);

int
vector_swap_elem(Vector *vector, const size_t i1, const size_t i2);

int
vector_print(const Vector vector);

int
vector_final(Vector *vector);
