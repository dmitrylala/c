COMPILER = gcc
COMPILER_FLAGS = -g -Wall -Werror
LINK_COMPILER_FLAGS = -o


main: main.o vectors.o matrices.o utils.o
	$(COMPILER) $^ -lm $(LINK_COMPILER_FLAGS) $@

main.o: main.c
	$(COMPILER) $(COMPILER_FLAGS) -c main.c

matrices.o: matrices.c matrices.h
	$(COMPILER) $(COMPILER_FLAGS) -c matrices.c

vectors.o: vectors.c vectors.h
	$(COMPILER) $(COMPILER_FLAGS) -c vectors.c

utils.o: utils.c utils.h macroses.h
	$(COMPILER) $(COMPILER_FLAGS) -c utils.c

clear:
	rm *.o main
