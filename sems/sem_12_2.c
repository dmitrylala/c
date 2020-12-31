#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>


int
main (int argc, char ** argv)
{
    int fd[2];
    pipe(fd);

    for (int i = 1; i < argc; ++i) {
        if (fork() == 0) {

            int flag = 1;
            read(fd[0], &flag, sizeof flag);

            if (fork() == 0) {
                execlp("cat", "cat", argv[i], NULL);
                exit(2);
            }

            wait(NULL);
            write(fd[1], &flag, sizeof flag);
            exit(1);
        }
    }

    int flag = 1;
    write(fd[1], &flag, sizeof flag);
    while (wait(NULL) != -1);

    return 0;
}