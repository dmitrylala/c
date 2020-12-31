COMPILER = gcc
COMPILER_FLAGS = -g -Wall -Werror
LINK_COMPILER_FLAGS = -o


main: main.o utilities.o graph.o
	$(COMPILER) $^ $(LINK_COMPILER_FLAGS) $@

main.o: main.c
	$(COMPILER) $(COMPILER_FLAGS) -c main.c

utilities.o: utilities.c utilities.h
	$(COMPILER) $(COMPILER_FLAGS) -c utilities.c

graph.o: graph.c graph.h
	$(COMPILER) $(COMPILER_FLAGS) -c graph.c

clear:
	rm *.o main











ifeq ("x","y")

flags:

-O2 -ftrapv -fsanitize=undefined -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wstrict-prototypes -Wold-style-declaration -Wold-style-definition -Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign -std=gnu11 -lm

endif


