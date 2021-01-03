#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "macroses.h"


// constants
#define BUF_SIZE 1024



// prints the given number of string to stdout according to value of n_flag
int
print_number(int string_number, int n_flag)
{
    if (n_flag == 1) {
        int number = string_number;
        int count_digits = (number == 0) ? 1 : 0;
        while (number != 0) {
            ++count_digits;
            number /= 10;
        }
        char *buf1 = NULL;
        ERR_MEM((buf1 = malloc(count_digits * sizeof *buf1)));
        char buf2[] = {":"};

        sprintf(buf1, "%d", string_number);
        ERR(write(1, buf1, count_digits));
        free(buf1);

        ERR(write(1, buf2, sizeof buf2));
    }

    return 0;
}



int main(int argc, char **argv) {

    // collecting -v and -n flags
    int v_flag = 0, n_flag = 0, i_substr = 0, i_file = 0;

    for (int i = 0; i < argc; ++i) {
        if (strstr(argv[i], "-") != NULL) {
            if (strcmp(argv[i], "-v\0") == 0) {
                v_flag = 1;
            } else if (strcmp(argv[i], "-n\0") == 0) {
                n_flag = 1;
            } else {
                char buf[] = "grep: illegal option\n";
                ERR(write(2, buf, sizeof buf));
                return ILLEGAL_OPTION;
            }
        } else if (i_substr == 0) {
            i_substr = i;
        } else if ((i_substr != 0) && (i_file == 0)) {
            i_file = i;
        }
    }

    if (argc - v_flag - n_flag <= 1) {
        char buf[] = "grep: not enough parameters\n";
        ERR(write(2, buf, sizeof buf));
        return TOO_FEW_PARAMETERS;
    }

    char *substr = argv[i_substr];

    // creating file descriptor
    char *file = NULL;
    int fd;
    if (i_file != 0) {
        struct stat s;
        file = argv[i_file];
        stat(file, &s);

        int flag_dir = S_ISDIR(s.st_mode);

        if (flag_dir == 1) {
            char buf_err[] = "grep: this is a directory\n";
            ERR(write(2, buf_err, sizeof buf_err));
            return IS_A_DIRECTORY;
        }

        ERR_MSG((fd = open(file, O_RDONLY)), "grep: error opening file\n",
                    ERR_OPENING_FILE);
    } else {
        fd = 0;
    }



    char buf[BUF_SIZE];
    int n, i_start, i_end;

    char *big_buf = NULL, *tmp = NULL, *res = NULL;
    size_t size = 0, size_tmp = 0, size_res = 0;

    int string_number = 0;

    while ((n = read(fd, buf, sizeof buf)) > 0) {

        i_end = 0;
        i_start = 0;
        for (int i = 0; i < n; ++i) {
            if (buf[i] == '\n') {
                ++string_number;
                i_end = i;

                size_tmp = i_end - i_start + 1;
                ERR_MEM((tmp = realloc(tmp, sizeof(*tmp) * (size_tmp + 1))));

                for (int j = 0; j < size_tmp; ++j) {
                    tmp[j] = buf[j + i_start];
                }
                tmp[size_tmp] = '\0';

                // concatenating remained symbols from previous iterations with
                // current buf
                if (big_buf != NULL) {
                    size_res = size + size_tmp + 1;
                    ERR_MEM((res = realloc(res, sizeof(*res) *
                                (size + size_tmp + 1))));

                    for (size_t j = 0; j < size; ++j) {
                        res[j] = big_buf[j];
                    }

                    for (size_t j = 0; j <= size_tmp; ++j) {
                        res[j + size] = tmp[j];
                    }

                    if (((strstr(res, substr) != NULL) && (v_flag == 0)) ||
                        ((strstr(res, substr) == NULL) && (v_flag == 1))) {

                        print_number(string_number, n_flag);
                        ERR(write(1, res, size_res));
                    }

                    free(big_buf);
                    big_buf = NULL;
                    size = 0;

                    free(res);
                    res = NULL;
                    size_res = 0;

                } else {
                    if (((strstr(tmp, substr) != NULL) && (v_flag == 0)) ||
                        ((strstr(tmp, substr) == NULL) && (v_flag == 1))) {

                        print_number(string_number, n_flag);
                        ERR(write(1, tmp, size_tmp));
                    }
                }

                free(tmp);
                tmp = NULL;
                size_tmp = 0;

                i_start = i_end + 1;
                i_end = i_start;
            }
        }


        // part of buf that remains after last '\n'
        ERR_MEM((big_buf = realloc(big_buf, size + sizeof(*big_buf) *
                    (n - i_start + 1))));

        for (int i = 0; i < n - i_start; ++i) {
            big_buf[i + size] = buf[i + i_start];
        }
        big_buf[n - i_start] = '\0';

        size += sizeof(*buf) * (n - i_start + 1);
    }

    if (big_buf != NULL) {
        if (((strstr(big_buf, substr) != NULL) && (v_flag == 0)) ||
            ((strstr(big_buf, substr) == NULL) && (v_flag == 1))) {
            ERR(write(1, big_buf, size));
        }
        free(big_buf);
    }

    if (fd != 0) {
        close(fd);
    }

    return 0;
}
