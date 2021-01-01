COMPILER = gcc
COMPILER_FLAGS = -Wall -Werror

all: grep tail mv

grep:
	$(COMPILER) $(COMPILER_FLAGS) grep.c -o grep

tail:
	$(COMPILER) $(COMPILER_FLAGS) tail.c -o tail

mv:
	$(COMPILER) $(COMPILER_FLAGS) mv.c -o mv

clear:
	rm grep tail mv




ifeq ("x","y")
-g -fsanitize=address,undefined -O2 -ftrapv -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wstrict-prototypes -Wold-style-declaration -Wold-style-definition -Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign -std=gnu11
endif
