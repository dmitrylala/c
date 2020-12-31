#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int new_strcmp(const void *p1, const void *p2) {
	return strcmp(*(const char **) p1, *(const char **) p2);	
}


int main(int argc, char *argv[]) {
	
	qsort(argv + 1, argc - 1, sizeof argv[0], new_strcmp);
	
	for (int i = 1; i < argc; ++i) {
		printf("%s ", argv[i]);
	}
	printf("\n");
	
	
	return 0;
}
