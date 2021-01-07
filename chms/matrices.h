#include "vectors.h"


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
matrix_generate(Matrix *matrix, const size_t size, const double min,
                const double max);

int
matrix_generate_ortog(Matrix *matrix, const size_t size, const double min,
                        const double max);

int
matrix_generate_symm(Matrix *matrix, const size_t size, const double min,
                    const double max, double *lam_min, double *lam_max);

int
matrix_transpose(Matrix *matrix, Matrix *res);

int
matrix_prod(Matrix m1, Matrix m2, Matrix *res);

int
matrix_vector_prod(const Matrix matrix, const Vector vector, Vector *vector_res);

double
matrix_norm(const Matrix matrix);

int
matrix_alloc(Matrix *matrix, const size_t size);

int
matrix_print(const Matrix matrix);

int
matrix_final(Matrix *matrix);




// SOLVING SYSTEM OF LINEAR EQUATIONS

int
solve_gauss(Matrix matrix, Vector right_size, Vector *sol);

int
matrix_search_nonzero_str(const Matrix matrix, const size_t str,
                            const size_t stb, size_t *res);

int
matrix_swap_str(Matrix *matrix, const size_t i1, const size_t i2);

int
matrix_str_null(const Matrix matrix, const size_t i);

int
matrix_triangulate(Matrix *matrix, Vector *right_side);

int
solve_msi(Matrix matrix, Vector right_side, Vector *sol, Vector init,
            const int n_iter, const double tau);
