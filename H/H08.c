/*
    a, b, h should be double; a is less than b, h is less than b - a
    functions available: sin, cos, tan, log, fabs (absolute value), sqrt, exp
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef double (*func)(double);
const char *funcs[7] = {"sin", "cos", "tan", "log", "fabs", "sqrt", "exp"};

int main(int argc,char **argv) {
    func names[7] = {sin, cos, tan, log, fabs, sqrt, exp};
    double x, a = atof(argv[2]), b = atof(argv[3]), h = atof(argv[4]);
    int i;

    for (i = 0; i < 7; ++i) {
        if (!strcmp(argv[1], funcs[i])) {
            break;
        }
    }

    if (i == 7) {
        printf("Wrong function\n");
        return 1;
    }

    for (x = a; x < b; x += h) {
        printf("function: %s(x) epsilon: %f argument: %f value: %f\n", funcs[i],
                h, x, names[i](x));
    }

    return 0;
}
