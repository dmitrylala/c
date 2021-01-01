COMPILER = gcc
COMPILER_FLAGS = -Wall -Werror
LINK_COMPILER_FLAGS = -o

all: main my_shell

main: main.o utilities.o
	$(COMPILER) $^ $(LINK_COMPILER_FLAGS) $@
main.o: main.c
	$(COMPILER) $(COMPILER_FLAGS) -c main.c

my_shell: stack.o utilities.o polis.o parsing.o my_shell.o
	$(COMPILER) $^ $(LINK_COMPILER_FLAGS) $@
stack.o: stack.c stack.h
	$(COMPILER) $(COMPILER_FLAGS) -c stack.c
utilities.o: utilities.c utilities.h
	$(COMPILER) $(COMPILER_FLAGS) -c utilities.c
polis.o: polis.c polis.h
	$(COMPILER) $(COMPILER_FLAGS) -c polis.c
parsing.o: parsing.c parsing.h
	$(COMPILER) $(COMPILER_FLAGS) -c parsing.c
my_shell.o: my_shell.c
	$(COMPILER) $(COMPILER_FLAGS) -c my_shell.c

clear:
	rm main my_shell *.o







ifeq ("x","y")

ejudge flags:

-g -fsanitize=address,undefined -O2 -ftrapv -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wstrict-prototypes -Wold-style-declaration -Wold-style-definition -Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign -std=gnu11
endif


