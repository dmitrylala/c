/*
    we will assume that files f1 and f2 do not yet exist in the current directory
*/

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char **argv) {

    // fd - descriptor for init file, f1 and f2 - for already existing files
    char buf[1024], buf_c[1];
    int fd, fd1, fd2, n;
    int n_strings = 100;

    if (argc != 4) {
        printf("wrong number of parametres\n");
        return 1;
    }

    if ((fd = open(argv[1], O_RDONLY)) == -1) {
        printf("error opening");
        return 2;
    }


    if ((fd1 = creat(argv[2], 0644)) == -1) {
        printf("error creating file\n");
        return 3;
    }

    if ((fd2 = creat(argv[3], 0644)) == -1) {
        printf("error creating file\n");
        return 3;
    }

    int i = 0, str_count = 0;
    while ((n = read(fd, buf_c, 1)) > 0) {
        buf[i] = *buf_c;
        i += 1;
        if (*buf_c == '\n') {
            i = 0;
            if (str_count < n_strings) {
                write(fd1, buf, i);
                str_count += 1;
            } else {
                write(fd2, buf, i);
            }
        }
    }

    // if the last symbol was not '\n'
    write(fd2, buf, i);

    close(fd);
    close(fd1);
    close(fd2);
    return 0;
}
