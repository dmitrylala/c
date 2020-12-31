/*

    The signal SIGTRAP raises when you enter (to stdin) the latin letter a
    Created processes transmit their pids to each other through the pipes and
    print them
    Sons are killed after they print pids

*/

#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

int count_sigint = 1;
int count_sigtrap = 1;


void sigint_handler(int s);
void sigtrap_handler(int s);


void sigint_handler(int s)
{
    printf("\nI got SIGINT %d time(s) \n", count_sigint);
    ++count_sigint;
    if (count_sigint == 3) {
        signal(SIGINT, SIG_DFL);
    } else {
        signal(SIGINT, sigint_handler);
    }
}

void sigtrap_handler(int s)
{
    printf("I got SIGTRAP %d time(s) \n", count_sigtrap);
    if (count_sigtrap % 2 == 0) {

        // creating sons and pipe(s)
        int fd[2], fd2[2];
        pipe(fd);
        pipe(fd2);

        if (fork() == 0) {
            signal(SIGINT, SIG_DFL);
            pid_t pid = getpid();
            write(fd[1], &pid, sizeof(pid));

            pid_t pid_brother;
            read(fd2[0], &pid_brother, sizeof(pid_brother));

            printf("pid of brother and my pid: %d %d\n", pid_brother, pid);
            raise(SIGINT);
        } else if (fork() == 0) {
            signal(SIGINT, SIG_DFL);
            pid_t pid = getpid();
            write(fd2[1], &pid, sizeof(pid));

            pid_t pid_brother;
            read(fd[0], &pid_brother, sizeof(pid_brother));

            printf("pid of brother and my pid: %d %d\n", pid_brother, pid);
            raise(SIGINT);
        } else {
            close(fd[0]);
            close(fd[1]);
            close(fd2[0]);
            close(fd2[1]);
            wait(NULL);
            wait(NULL);
        }
    }
    ++count_sigtrap;
}



int main(int argc, char **argv)
{
    signal(SIGINT, sigint_handler);
    signal(SIGTRAP, sigtrap_handler);

    while (1) {
        char c = getc(stdin);
        if (c == 'a') {
            raise(SIGTRAP);
        }
    }

    return 0;
}
