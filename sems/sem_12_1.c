#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>


int
main (int argc, char ** argv)
{
    int fd[2], fd2[2];
    pipe(fd);
    pipe(fd2);

    if (fork() == 0) {
        pid_t pid = getpid();
        write(fd[1], &pid, sizeof(pid));

        pid_t pid_brother;
        read(fd2[0], &pid_brother, sizeof(pid_brother));

        printf("pid of brother and his pid: %d %d\n", pid_brother, pid);
    } else if (fork() == 0) {
        pid_t pid = getpid();
        write(fd2[1], &pid, sizeof(pid));

        pid_t pid_brother;
        read(fd[0], &pid_brother, sizeof(pid_brother));

        printf("pid of brother and his pid: %d %d\n", pid_brother, pid);
    } else {
        close(fd[0]);
        close(fd[1]);
        close(fd2[0]);
        close(fd2[1]);
        wait(NULL);
        wait(NULL);
    }
    return 0;
}