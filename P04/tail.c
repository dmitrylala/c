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
#define TMP_BUF_SIZE 1024



int main(int argc, char **argv) {

    int count = -10; // flag value by default
    int n_flag = 0, i_file = 0;
    for (int i = 0; i < argc; ++i) {
        if ((strstr(argv[i], "-") != NULL) || (strstr(argv[i], "+") != NULL)) {
            if (strcmp(argv[i], "-n\0") == 0) {

                if ((argv[i + 1] == NULL) ||
                    (sscanf(argv[i + 1], "%d", &count) != 1)) {
                    char buf[] = "tail: illegal option\n";
                    ERR(write(2, buf, sizeof buf));
                    return ILLEGAL_OPTION;
                }
                ++i;

            } else if (strcmp(argv[i], "+n\0") == 0) {
                n_flag = 1;

                if ((argv[i + 1] == NULL) ||
                    (sscanf(argv[i + 1], "%d", &count) != 1)) {
                    char buf[] = "tail: illegal option\n";
                    ERR(write(2, buf, sizeof buf));
                    return ILLEGAL_OPTION;
                }
                ++i;

            } else {
                char buf[] = "tail: illegal option\n";
                ERR(write(2, buf, sizeof buf));
                return ILLEGAL_OPTION;
            }
        } else if (i_file == 0) {
            i_file = i;
        }
    }



    // creating file descriptor
    char *file = NULL;
    char f_tmp[] = "tempfile-XXXXXXX"; // for from stdin
    int fd;

    int n;
    char tmp_buf[TMP_BUF_SIZE];
    if (i_file != 0) {
        struct stat s;
        file = argv[i_file];
        stat(file, &s);

        int flag_dir = S_ISDIR(s.st_mode);

        if (flag_dir == 1) {
            char buf_err[] = "tail: this is a directory\n";
            ERR(write(2, buf_err, sizeof buf_err));
            return IS_A_DIRECTORY;
        }

        ERR_MSG((fd = open(file, O_RDONLY)), "tail: error opening file\n",
                    ERR_OPENING_FILE);
    } else {
        ERR_MSG((fd = mkstemp(f_tmp)), "tail: error creating file\n",
                ERR_CREATING_FILE);
        ERR_MSG(unlink(f_tmp), "tail: error deleting file\n", ERR_DELETING_FILE);

        while ((n = read(0, tmp_buf, sizeof tmp_buf)) > 0) {
            ERR(write(fd, tmp_buf, n));
        }
    }


    if (count < 0) {
        count = -1 * count;
    }

    char buf[BUF_SIZE];
    int count_strings = 0;

    if (n_flag == 1) {

        // just reading the file and printing to stdout those strings, which
        // number is bigger than value of count variable

        lseek(fd, 0L, SEEK_SET);

        while ((n = read(fd, buf, sizeof buf)) > 0) {

            if (count_strings + 1 < count) {
                int i_last = 0;
                for (int i = 0; i < n; ++i) {

                    if (buf[i] == '\n') {
                        i_last = i;
                        ++count_strings;
                    }

                    if (count_strings + 1 >= count) {
                        ERR(write(1, &buf[i_last + 1], n - i_last - 1));
                        break;
                    }
                }
            } else {
                ERR(write(1, buf, n));
            }
        }
    } else {

        // reading file from end (very carefully)

        long cur = lseek(fd, -1 * sizeof buf, SEEK_END);

        while ((n = read(fd, buf, sizeof buf)) > 0) {

            if (count_strings - 1 < count) {
                int i_last = n - 1;
                for (int i = n - 1; i >= 0; --i) {

                    if ((buf[i] == '\n') || (cur == 0)) {
                        i_last = i;
                        ++count_strings;

                        if (cur == 0) {
                            lseek(fd, 0L, SEEK_SET);
                            break;
                        }
                    }

                    if (count_strings - 1 >= count) {
                        ERR(write(1, &buf[i_last + 1], n - i_last - 1));
                        break;
                    }
                }


                if (count_strings - 1 < count) {

                    cur = lseek(fd, 0L, SEEK_CUR);

                    long offset;
                    if (2 * sizeof buf < cur) {
                        offset = 2 * sizeof buf;
                    } else {
                        offset = cur;
                    }

                    cur = lseek(fd, -1 * offset, SEEK_CUR);
                }
            } else {
                ERR(write(1, buf, n));
            }
        }
    }

    close(fd);

    return 0;
}
