#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[]) {
	const char *name = argv[1];
	FILE *f = fopen(name, "r+");
	
	//perror("Error opening file");  // Success
	
	if (f == NULL) {
		
		perror("Error opening file"); 
		
		//~ fprintf(stderr, "Error opening file: %s\n", strerror(errno));
		exit(1);
	}
	
	int n, d, i = 0;
	
	sscanf(argv[2], "%d", &n);
	
	while(i < n) {
		fscanf(f, "%d", &d);
		++i;
	}
	fseek(f, 0L, SEEK_END);
	fprintf(f, " %d", d);
	
	fclose(f);
	return 0;
}
