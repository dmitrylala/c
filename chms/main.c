/*
    Gauss for 1k variables:
    Time: 4
    ||solution - answer|| = 0.0000000600

    Gauss for 10k variables:
    Time: 4134
    ||solution - answer|| = 0.0002289491


*/
#include <fcntl.h>
#include <time.h>

#include "matrices.h"



int main(int argc, char **argv) {

    if (argv[1] == NULL) RAISE(NO_METHOD);
    char *method;

    // constants
    char gauss[] = "gauss\0";
    char msi[] = "msi\0";
    size_t size = 5;
    double min = -100;
    double max = 100;
    int n_iter = 100;
    double lam_min, lam_max;

    // randomize
    srand(time(0));

    // generating random matrix depending on method
    Matrix matrix;
    if (strcmp(argv[1], gauss) == 0) {
        method = gauss;
        RAISE(matrix_generate(&matrix, size, min, max));
    } else if (strcmp(argv[1], msi) == 0) {
        method = msi;
        RAISE(matrix_generate_symm(&matrix, size, min, max, &lam_min, &lam_max));
    } else RAISE(WRONG_METHOD);

    // generating random answer vector
    Vector answer;
    RAISE(vector_generate(&answer, size, min, max));

    // calculating vector of right side
    Vector right_side;
    RAISE(matrix_vector_prod(matrix, answer, &right_side));

    // printing matrix and answer vector
    // printf("\nMatrix of system:\n");
    // matrix_print(matrix);
    // printf("\nCorrect answer:\n");
    // vector_print(answer);
    // printf("\nRight side vector:\n");
    // vector_print(right_side);

    // solving system of linear equations
    Vector solution;
    time_t time_begin = time(0);
    if (strcmp(method, gauss) == 0) {
        RAISE(solve_gauss(matrix, right_side, &solution));
    } else if (strcmp(method, msi) == 0) {
        double tau1 = 1.0 / matrix_norm(matrix);
        // double tau2 = 2.0 * tau1;
        // double tau3 = 1.0 / (lam_min + lam_max);
        // printf("Taus: %lf %lf %lf\n", tau1, tau2, tau3);
        Vector init;
        RAISE(vector_generate(&init, size, min, max));
        RAISE(solve_msi(matrix, right_side, &solution, init, n_iter, tau1));
    } else RAISE(WRONG_METHOD);
    time_t time_end = time(0);
    printf("Time: %ld\n", time_end - time_begin);
    // printf("Predicted vector:\n");
    // vector_print(solution);
    double dist;
    RAISE(vector_dist(solution, answer, &dist));
    printf("||solution - answer|| = %.10lf\n", dist);


    // finalize
    matrix_final(&matrix);
    vector_final(&answer);
    vector_final(&right_side);
    vector_final(&solution);

    return 0;
}
