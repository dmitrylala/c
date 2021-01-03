COMPILER = gcc
COMPILER_FLAGS = -g -Wall -Werror
LINK_COMPILER_FLAGS = -o


main: main.o matrices.o utils.o
	$(COMPILER) $^ $(LINK_COMPILER_FLAGS) $@

main.o: main.c
	$(COMPILER) $(COMPILER_FLAGS) -c main.c

matrices.o: matrices.c matrices.h
	$(COMPILER) $(COMPILER_FLAGS) -c matrices.c

utils.o: utils.c utils.h macroses.h
	$(COMPILER) $(COMPILER_FLAGS) -c utils.c

clear:
	rm *.o main
