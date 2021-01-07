#include "vectors.h"


int
vector_initf(Vector *vector, const int fd)
{
    // allocating memory for vector
    ERR_MEM(vector -> data = malloc(vector -> size * sizeof *(vector -> data)),
            ERR_MEM_ALLOC);

    // getting values
    char *str;
    ERR_ACT_EQ(str = str_in(fd),  NULL, WRONG_INPUT, vector_final(vector));
    char *str1 = str;
    str1 = strtok(str1, " \0");
    for (size_t i = 0; i < vector -> size; ++i) {
        ERR_ACTS(sscanf(str1, "%lf", &(vector -> data[i])), 1, WRONG_INPUT,
                free(str), vector_final(vector));
        str1 = strtok(NULL, " \0");
    }
    free(str);

    return 0;
}

int
vector_generate(Vector *vector, const size_t size, const double min,
                const double max)
{
    if (size <= 0) return WRONG_SIZE;

    // allocating memory for vector
    vector -> size = size;
    ERR_MEM(vector -> data = malloc(size * sizeof *(vector -> data)),
            ERR_MEM_ALLOC);

    // generating vector values
    for (size_t i = 0; i < size; ++i) {
            vector -> data[i] = get_rand_range(min, max);
    }
    return 0;
}

int
vector_dot(const Vector vector1, const Vector vector2, double *res)
{
    if (vector1.size != vector2.size) return WRONG_SIZE;

    // calculating dot product
    double sum = 0;
    for (size_t i = 0; i < vector1.size; ++i) {
        sum += vector1.data[i] * vector2.data[i];
    }
    *res = sum;

    return 0;
}

int
vector_dist(const Vector vector1, const Vector vector2, double *res)
{
    if (vector1.size != vector2.size) return WRONG_SIZE;

    // calculating dist
    double sum = 0;
    for (size_t i = 0; i < vector1.size; ++i) {
        sum += pow(fabs(vector1.data[i] - vector2.data[i]), 2);
    }
    sum = pow(sum, 1.0 / 2);
    *res = sum;

    return 0;
}

int
vector_swap_elem(Vector *vector, const size_t i1, const size_t i2)
{
    if ((i1 >= vector -> size) || (i2 >= vector -> size)) return WRONG_INDEX;

    double tmp;
    tmp = vector -> data[i1];
    vector -> data[i1] = vector -> data[i2];
    vector -> data[i2] = tmp;

    return 0;
}


int
vector_print(const Vector vector)
{
    for (size_t i = 0; i < vector.size; ++i) {
        printf("%5lf     ", vector.data[i]);
    }
    printf("\n");
    return 0;
}

int
vector_final(Vector *vector)
{
    free(vector -> data);
    vector -> size = 0;
    return 0;
}
