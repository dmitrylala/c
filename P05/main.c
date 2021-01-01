#include <sys/wait.h>
#include <unistd.h>

#include "utilities.h"


int main(int argc, char **argv) {

    invitation();
    int status;
    while (1) {

        if (fork() == 0) {
            execl("my_shell", "my_shell", NULL);
            perror("my_shell: exec error");
            return 1;
        }
        wait(&status);

        if (status != 0) {
            fprintf(stderr, "status: %d\n", status);
            break;
        }

        invitation();
    }

    return 0;
}
