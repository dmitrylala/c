#include "rec_descent.h"


int main(void) {

    int num;
    printf("Enter number of variables: ");
    int err = scanf("%d", &num);
    getchar();
    if ((err != 1) || (num < 0)) {
        printf("Error: bad_number of variables\n");
        return BAD_NUMBER_VARIABLES;
    }


    Database base_vars;
    base_init(&base_vars);

    if (num > 0) {
        RAISE_1(base_create(&base_vars, num));
    }


    Polis polis;
    polis_init(&polis);

    printf("Enter expression:\n");

    int fd_in = 0;
    char *str = str_in(fd_in);

    if (str == NULL) {
        perror("Error reading string\n");
        base_final(&base_vars);
        return 1;
    }

    RAISE_2(recursive_descent(str, &polis, &base_vars));

    double result;

    RAISE_2(polis_calculate(&polis, &result, sizeof result));


    printf("---------------\n");
    printf("result: %lf\n", result);
    printf("---------------\n");

    free(str);
    polis_final(&polis);
    base_final(&base_vars);

    return 0;

FINALLY:
    free(str);
    polis_final(&polis);
    base_final(&base_vars);
    return err;
FINALLY_BASE:
    base_final(&base_vars);
    return err;
}



