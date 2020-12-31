#include <unistd.h>

#ifdef MODE
#define MODE 1
#endif

#if MODE
#define EXECUTION do { \
                        execlp("gcc", "gcc", "1.c", "-o", "1.exe", "-lm", (char *)0); \
                    } \
                    while (0)
#else
#define EXECUTION do { \
                        char *str[] = {"gcc", "1.c", "-o", "1.exe", "-lm", \
                                        (char *)0}; \
                        execvp("gcc", str); \
                    } \
                    while (0)
#endif


int main(void) {

    int pid;
    if ((pid = fork()) == 0) {
        EXECUTION;
    }

    return 0;
}
