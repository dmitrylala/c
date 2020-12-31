COMPILER = gcc
COMPILER_FLAGS = -Wall -Werror
LINK_COMPILER_FLAGS = -o

all: main

main: main.o utilities.o base_vars.o stack.o polis.o rec_descent.o
	$(COMPILER) $^ $(LINK_COMPILER_FLAGS) $@
main.o: main.c
	$(COMPILER) $(COMPILER_FLAGS) -c main.c
utilities.o: utilities.c utilities.h
	$(COMPILER) $(COMPILER_FLAGS) -c utilities.c
base_vars.o: base_vars.c base_vars.h
	$(COMPILER) $(COMPILER_FLAGS) -c base_vars.c
stack.o: stack.c stack.h
	$(COMPILER) $(COMPILER_FLAGS) -c stack.c
polis.o: polis.c polis.h macroses.h
	$(COMPILER) $(COMPILER_FLAGS) -c polis.c
rec_descent.o: rec_descent.c rec_descent.h
	$(COMPILER) $(COMPILER_FLAGS) -c rec_descent.c

clear:
	rm *.o main
