#include <fcntl.h>

#include "matrices.h"



int main(int argc, char **argv) {

    // opening file
    int fd;
    ERR_SYS(fd = open(argv[1], O_RDONLY));

    // initializating matrix from file
    Matrix matrix;
    int err = matrix_initf(&matrix, fd);
    close(fd);
    RAISE(err);

    // printing matrix
    matrix_print(matrix);

    // finalize
    matrix_final(&matrix);

    return 0;
}
