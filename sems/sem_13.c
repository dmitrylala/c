//~ #undef _DEFAULT_SOURCE

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>


char *arg;
unsigned int count = 1;


void sig_hndlr(int s) {
    exit(0);
}

//~ void sigchld_hndlr(int s) {
    //~ int status;
    //~ while ((pid = waitpid(-1, &status, WNOHANG)) > -1) {
        //~ printf("child: %d\n", pid);
    //~ }
//~ }

void sigalrm_hndlr(int s) {
    if (fork() == 0) {
        printf("child: %d\n", getpid());

        signal(SIGINT, SIG_IGN);

        execlp(arg, arg, NULL);
    }
    alarm(count);
}

void sigint_hndlr(int s) {
    ++count;
}

void sighup_hndlr(int s) {
    kill(0, SIGKILL);
}



int main(int argc, char **argv) {

    arg = argv[1];

    signal(SIGTERM, sig_hndlr);
    signal(SIGQUIT, sig_hndlr);
    //~ signal(SIGCHLD, sigchld_hndlr);
    signal(SIGALRM, sigalrm_hndlr);

    //~ signal(SIGINT, SIG_IGN);
    signal(SIGINT, sigint_hndlr);

    signal(SIGHUP, sighup_hndlr);

    pid_t pid = getpid();
    printf("parent pid: %d\n", pid);

    alarm(count);
    int status;
    while (1) {
        while ((pid = waitpid(-1, &status, WNOHANG)) > 1) {
            printf("child and status: %d %d\n", pid, WIFEXITED(status));
        }
        pause();
    }

    return 0;
}
