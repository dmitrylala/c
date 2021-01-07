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
    int err;
    matrix -> size = size;
    ERR(err = matrix_alloc(matrix, size), 0, err);

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

int
matrix_generate(Matrix *matrix, const size_t size, const double min,
                const double max)
{
    if (size <= 0) return WRONG_SIZE;

    // allocating memory for matrix
    int err;
    matrix -> size = size;
    ERR(err = matrix_alloc(matrix, size), 0, err);

    // generating matrix values
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) matrix -> data[i][j] =
                                                    get_rand_range(min, max);
    }
    return 0;
}

int
matrix_generate_ortog(Matrix *matrix, const size_t size, const double min,
                        const double max)
{
    if (size <= 0) return WRONG_SIZE;

    // allocating memory for matrix
    int err;
    matrix -> size = size;
    ERR(err = matrix_alloc(matrix, size), 0, err);

    // generating ortogonal matrix values
    double sum;
    for (size_t i = 0; i < size; ++i) {
        sum = 0;
        for (size_t j = 0; j < size; ++j) {
            matrix -> data[i][j] = get_rand_range(min, max);
            sum += pow(matrix -> data[i][j], 2);
        }
        sum = pow(sum, 1.0 / 2);

        // norming the vector-string
        for (size_t j = 0; j < size; ++j) matrix -> data[i][j] /= sum;
    }

    Vector vec;
    RAISE(vector_generate(&vec, size, min, max));
    printf("Ortog matrix before:\n");
    matrix_print(*matrix);
    matrix_triangulate(matrix, &vec);
    printf("Ortog matrix after:\n");
    matrix_print(*matrix);
    double prod = 1;
    for (size_t i = 0; i < size; ++i) prod *= matrix -> data[i][i];
    printf("Det: %lf\n", prod);

    return 0;
}

int
matrix_generate_symm(Matrix *matrix, const size_t size, const double min,
                    const double max, double *lam_min, double *lam_max)
{
    if (size <= 0) return WRONG_SIZE;

    // generating ortogonal matrix and transposed
    int err;
    Matrix ortog, ortog_t;
    ERR(err = matrix_generate_ortog(&ortog, size, min, max), 0, err);
    ERR(err = matrix_transpose(&ortog, &ortog_t), 0, err);


    // generating diagonal matrix with positive values
    Matrix diag;
    diag.size = size;
    ERR(err = matrix_alloc(&diag, size), 0, err);
    *lam_min = fabs(max);
    *lam_max = 0;
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            if (i == j) {
                diag.data[i][j] = get_rand_range(0, fabs(max));
                if (diag.data[i][j] > *lam_max) *lam_max = diag.data[i][j];
                if (diag.data[i][j] < *lam_min) *lam_min = diag.data[i][j];
            } else diag.data[i][j] = 0;
        }
    }

    // the result: matrix = ortog_t * diag * ortog;
    Matrix prod;
    ERR(err = matrix_prod(ortog_t, diag, &prod), 0, err);
    ERR(err = matrix_prod(prod, ortog, matrix), 0, err);

    matrix_final(&ortog);
    matrix_final(&ortog_t);
    matrix_final(&diag);
    matrix_final(&prod);

    return 0;
}

int
matrix_transpose(Matrix *matrix, Matrix *res)
{
    // allocating memory for matrix
    int err;
    res -> size = matrix -> size;
    ERR(err = matrix_alloc(res, res -> size), 0, err);

    // generating diagonal matrix values
    for (size_t i = 0; i < res -> size; ++i) {
        for (size_t j = 0; j < res -> size; ++j) {
            res -> data[i][j] = matrix -> data[j][i];
        }
    }

    return 0;
}

int
matrix_prod(Matrix m1, Matrix m2, Matrix *res)
{
    // allocating memory for res matrix
    int err;
    res -> size = m1.size;
    ERR(err = matrix_alloc(res, res -> size), 0, err);

    // calculating the product
    double sum;
    for (size_t i = 0; i < res -> size; ++i) {
        for (size_t j = 0; j < res -> size; ++j) {
            sum = 0;
            for (size_t k = 0; k < res -> size; ++k) {
                sum += m1.data[i][k] * m2.data[k][j];
            }
            res -> data[i][j] = sum;
        }
    }

    return 0;
}

int
matrix_vector_prod(const Matrix matrix, const Vector vector, Vector *vector_res)
{
    if (matrix.size != vector.size) return WRONG_SIZE;

    // allocating memory for vector
    vector_res -> size = matrix.size;
    ERR_MEM(vector_res -> data = malloc(vector_res -> size *
            sizeof *(vector_res -> data)), ERR_MEM_ALLOC);

    // calculating result vector
    int err;
    for (size_t i = 0; i < vector_res -> size; ++i) {

        // vector of current string
        Vector cur_string;
        cur_string.size = matrix.size;
        ERR_ACT_EQ(cur_string.data = malloc(cur_string.size * sizeof *(cur_string.data)),
                    NULL, ERR_MEM_ALLOC, vector_final(vector_res));
        for (size_t j = 0; j < cur_string.size; ++j) {
            cur_string.data[j] = matrix.data[i][j];
        }

        ERR_ACTS(err = vector_dot(cur_string, vector, &(vector_res -> data[i])),
                    0, err, vector_final(&cur_string), vector_final(vector_res));

        vector_final(&cur_string);
    }
    return 0;
}

double
matrix_norm(const Matrix matrix)
{
    double norm = 0;
    size_t n = matrix.size;
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            norm += pow(matrix.data[i][j], 2);
        }
    }
    norm = pow(norm, 1.0 / 2);
    return norm;
}

int
matrix_alloc(Matrix *matrix, const size_t size)
{
    ERR_MEM(matrix -> data = malloc(size * sizeof *(matrix -> data)),
            ERR_MEM_ALLOC);
    for (size_t i = 0; i < matrix -> size; ++i) {
        ERR_MEM(matrix -> data[i] = malloc(size * sizeof **(matrix -> data)),
                ERR_MEM_ALLOC);
    }
    return 0;
}

// functions that prints matrix to stdout
// considering that matrix was initialized
int
matrix_print(const Matrix matrix)
{
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
    matrix -> size = 0;
    return 0;
}




// SOLVING SYSTEM OF LINEAR EQUATIONS


int
solve_gauss(Matrix matrix, Vector right_side, Vector *sol)
{
    int err;
    ERR(err = matrix_triangulate(&matrix, &right_side), 0, err);

    // allocating memory for vector
    sol -> size = matrix.size;
    ERR_MEM(sol -> data = malloc(sol -> size * sizeof *(sol -> data)),
            ERR_MEM_ALLOC);

    // calculating solution vector values (reverse gauss step)
    double sum;
    for (int i = sol -> size - 1; i > -1; --i) {
        sum = 0;
        for (size_t j = i + 1; j < sol -> size; ++j) {
            sum += matrix.data[i][j] * sol -> data[j];
        }
        sol -> data[i] = 1.0 / matrix.data[i][i] * (right_side.data[i] - sum);
    }

    return 0;
}

// Находит индекс строки первого ненулевого элемента в столбце с индексом
// stb начиная со строки с индексом str
int
matrix_search_nonzero_str(const Matrix matrix, const size_t str,
                            const size_t stb, size_t *res)
{
    for (size_t i = str; i < matrix.size; ++i) {
        if (matrix.data[i][stb] != 0) {
            *res = i;
            return 0;
        }
    }
    return NO_SUCH_INDEX;
}

// Свап двух строк с индексами i1 и i2
int
matrix_swap_str(Matrix *matrix, const size_t i1, const size_t i2)
{
    if ((i1 >= matrix -> size) || (i2 >= matrix -> size)) return WRONG_INDEX;

    double tmp;
    for (size_t j = 0; j < matrix -> size; ++j) {
        tmp = matrix -> data[i1][j];
        matrix -> data[i1][j] = matrix -> data[i2][j];
        matrix -> data[i2][j] = tmp;
    }
    return 0;
}

int
matrix_str_null(const Matrix matrix, const size_t i)
{
    int flag = 0;
    size_t count = 0;
    for (size_t j = 0; j < matrix.size; ++j) if (matrix.data[i][j] == 0) ++count;
    if (count == matrix.size) flag = 1;
    return flag;
}

// приведение матрицы к треугольному виду
int
matrix_triangulate(Matrix *matrix, Vector *right_side)
{
    size_t n = matrix -> size;
    size_t str_i;
    int err;
    double dif;
    for (size_t j = 0; j < n; ++j) {

        ERR(err = matrix_search_nonzero_str(*matrix, j, j, &str_i), 0, err);

        if (str_i != j) {
            ERR(err = matrix_swap_str(matrix, str_i, j), 0, err);
            ERR(err = vector_swap_elem(right_side, str_i, j), 0, err);
            str_i = j;
        }

        // преобразования (str = j)
        for (size_t i = j + 1; i < n; ++i) {
            dif = matrix -> data[i][j] / matrix -> data[str_i][j];
            for (size_t k = 0; k < n; ++k) {
                matrix -> data[i][k] = matrix -> data[i][k] - dif *
                                        matrix -> data[str_i][k];
            }
            right_side -> data[i] = right_side -> data[i] - dif *
                                    right_side -> data[str_i];
        }

        for (size_t i = 0; i < n; ++i) if (matrix_str_null(*matrix, i)) return 0;
    }
    return 0;
}

int
solve_msi(Matrix matrix, Vector right_side, Vector *sol, Vector init,
            const int n_iter, const double tau)
{
    Vector x_prev;
    x_prev.size = init.size;
    ERR_MEM(x_prev.data = malloc(x_prev.size * sizeof *(x_prev.data)),
            ERR_MEM_ALLOC);
    for (size_t i = 0; i < x_prev.size; ++i) {
        x_prev.data[i] = init.data[i];
    }
    vector_final(&init);

    Vector x_new;
    for (int k = 0; k < n_iter; ++k) {
        // x_new = x_prev + tau(f - A * x_prev)
        matrix_vector_prod(matrix, x_prev, &x_new);
        for (size_t i = 0; i < x_new.size; ++i) {
            x_new.data[i] = x_prev.data[i] + tau * (right_side.data[i] -
                                                            x_new.data[i]);
        }
        for (size_t i = 0; i < x_prev.size; ++i) {
            x_prev.data[i] = x_new.data[i];
        }
        vector_final(&x_new);
    }

    *sol = x_prev;

    return 0;
}
