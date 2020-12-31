COMPILER = gcc
COMPILER_FLAGS = -Wall -Werror
LINK_COMPILER_FLAGS = -o

all: main my_shell

main:
	$(COMPILER) $(COMPILER_FLAGS) main.c -o main

my_shell: stack.o polis.o parsing.o
	$(COMPILER) $^ $(LINK_COMPILER_FLAGS) $@

stack.o: stack.c stack.h
	$(COMPILER) $(COMPILER_FLAGS) -c stack.c

polis.o: polis.c polis.h
	$(COMPILER) $(COMPILER_FLAGS) -c polis.c

parsing.o: parsing.c parsing.h
	$(COMPILER) $(COMPILER_FLAGS) -c parsing.c

clear:
	rm main my_shell *.o







ifeq ("x","y")

ejudge flags:

-g -fsanitize=address,undefined -O2 -ftrapv -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wstrict-prototypes -Wold-style-declaration -Wold-style-definition -Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign -std=gnu11
endif


