#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#include "macroses.h"


// constants
#define BUF_SIZE 1024



int main(int argc, char **argv) {

    if (argc <= 2) {
        char buf[] = "mv: not enough parameters\n";
        ERR(write(2, buf, sizeof buf));
        return TOO_FEW_PARAMETERS;
    }

    int fd_old, fd_new;
    char *file_old = argv[1];
    char *file_new = argv[2];


    if (strcmp(file_old, file_new) == 0) {
        return 0;
    }

    // to define if we deal with files or directories let's get stats
    struct stat s_old, s_new;
    stat(file_old, &s_old);
    stat(file_new, &s_new);

    int flag_dir_old = S_ISDIR(s_old.st_mode);

    if (flag_dir_old == 1) {
        char buf_err[] = "mv: first argument is a directory\n";
        ERR(write(2, buf_err, sizeof buf_err));
        return IS_A_DIRECTORY;
    }

    ERR_MSG((fd_old = open(file_old, O_RDONLY)), "mv: error opening file\n",
                ERR_OPENING_FILE);

    int flag_reg = S_ISREG(s_new.st_mode);
    int flag_dir = S_ISDIR(s_new.st_mode);

    // if there is no such file or directory as file_new
    if (flag_reg || flag_dir == 0) {
        ERR_MSG((fd_new = creat(file_new, s_old.st_mode)),
                    "mv: error creating file\n", ERR_CREATING_FILE);

    // if file_new is directory: copy file_old to that directory with old name
    } else if ((flag_reg == 0) && (flag_dir == 1)) {

        // creating correct path
        if (file_new[strlen(file_new) - 1] != '/') {
            file_new = strcat(file_new, "/\0");
        }

        char *file_old_prev = NULL;
        char *str = strdup(file_old);

        str = strtok(str, "/\0");
        do {
            file_old_prev = str;
            str = strtok(NULL, "/\0");
        } while (str != NULL);

        free(str);

        file_new = strcat(file_new, file_old_prev);

        ERR_MSG((fd_new = creat(file_new, s_old.st_mode)),
                    "mv: err creating file\n", ERR_CREATING_FILE);

    // else if file_new already exists
    } else if (flag_reg == 1) {
        ERR_MSG((fd_new = open(file_new, s_old.st_mode)),
                    "mv: error opening file\n", ERR_OPENING_FILE);
    }

    // copying file
    char buf[BUF_SIZE];
    int n;
    while ((n = read(fd_old, buf, sizeof buf)) > 0) {
        ERR(write(fd_new, buf, n));
    }

    close(fd_old);
    close(fd_new);

    // deleting old file
    ERR_MSG(unlink(file_old), "mv: error deleting file\n", ERR_DELETING_FILE);

    return 0;
}
