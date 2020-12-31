#include "parsing.h"

int main(int argc, char **argv) {

    int err;
    err = parsing_cmd();

    if (err != 0) {
        printf("ERROR: %d\n", err);
    }

    return 0;
}

