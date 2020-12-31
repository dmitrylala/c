#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>


void
invitation(void)
{
    printf("\033[1;30;91m");

    // let's assume that current path suits into string with length = 100
    char s[100];
    char *res = NULL;
    res = getcwd(s, 100);
    printf("%s", res);
    printf("\x1b[0m");
    printf("\x1b[96m");
    printf("\u27A4 ");
    printf("\x1b[0m");
    fflush(stdout);
}


int main(int argc, char **argv) {

    invitation();
    while (1) {

        if (fork() == 0) {
            execl("my_shell", "my_shell", NULL);
            perror("my_shell: exec error");
            return 1;
        }
        wait(NULL);

        invitation();
    }

    return 0;
}
