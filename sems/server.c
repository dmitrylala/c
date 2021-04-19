#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

// сервер //

#define SAFE(msg, comm) if ((comm) == -1) {\
                            perror(msg);\
                            exit(1);}


void quit(int s) {
	exit(0);
}

int
main (void) {
	unlink("file.txt"); // аналог remove()
	int sock_fd;
	SAFE("Socket", sock_fd = socket(AF_LOCAL, SOCK_STREAM, 0))

	struct sockaddr_un addr;
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, "file.txt");

	SAFE("Bind", bind(sock_fd, (struct sockaddr *) &addr, sizeof addr))

	SAFE("Listen", listen(sock_fd, 10))

	signal(SIGINT, quit);

	while (1) {
		pid_t pid = getpid();
		int cl_sock;
		SAFE("Accept", cl_sock = accept(sock_fd, NULL, NULL))
		// параллельная обработка
		if (fork() == 0) {
			while (pid > 0) {
				SAFE("Write", write(cl_sock, &pid, sizeof pid))
				SAFE("Read", read(cl_sock, &pid, sizeof pid))
				pid -= 1000;
				printf("Ping: %d\n", pid);
				fflush(stdout);
			}
			// SAFE("Shutdown", shutdown(cl_sock, SHUT_RDWR))
			close(cl_sock);
			exit(0);
		}
		int status;
		// неблокирующий wait()
		while (waitpid(-1, &status, WNOHANG) > 1) {
		}
	}

    SAFE("Shutdown", shutdown(sock_fd, SHUT_RDWR))
	close(sock_fd);
}
