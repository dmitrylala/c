#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv) {

    int fd[2];
    pipe(fd);

    if (fork() == 0) {

        dup2(fd[1], 1);

        execlp("cat", "cat", "/etc/passwd", NULL);
    }

    dup2(fd[0], 0);
    close(fd[0]);
    close(fd[1]);

    execlp("grep", "grep", "user", NULL);

    return 0;
}