#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

// клиент //

#define SAFE(msg, comm) if ((comm) == -1){\
                            perror(msg);\
                            exit(1);}
                        
int
main (void) {
	int s = socket(AF_LOCAL, SOCK_STREAM, 0);
	char *address = "file.txt";
	struct sockaddr_un addr;
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, address);
	
	SAFE("Connect", connect(s, (struct sockaddr *)&addr, sizeof(addr)))
	
	pid_t result;
	pid_t pid_out = getpid();
	int n;
	do {
		SAFE("read", n = read(s, &result, sizeof result))
		if (n == 0) {
			printf("Game over!\n");
			fflush(stdout);
			break;
		}
		if (n != sizeof result) {
			fprintf(stderr, "not date\n");
			exit(1);
		}
		result -= 1000;
		SAFE("write", write(s, &result, sizeof result))
		
		printf("Pong: %d, pid: %d\n", result, pid_out);
		fflush(stdout);
	} while (result > 0);
	
	printf("server pid = %d\n", result);
	fflush(stdout);
	shutdown(s, 2);
	close(s);
	return 0;
}
