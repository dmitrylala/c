#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[]) {

    int status, pid;

    if ((pid = fork()) == 0) {
        execlp(argv[1], argv[1], (char *)0);
        perror("exec error");
        return 1;
    }
    wait(&status);

    if ((WIFEXITED(status) && WEXITSTATUS(status)) == 0) {
        execlp(argv[2], argv[2], (char *)0);
        perror("exec error");
        return 2;
    }
}