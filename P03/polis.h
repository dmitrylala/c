#include <stddef.h>

#include "stack.h"
#include "base_vars.h"


// errors
#define SYNTAX_ERROR 1
#define DIVIZION_BY_ZERO 7
#define POLIS_OVERFLOW 8
#define MISSING_OPERATOR 9
#define INCORRECT_VALUE 17

// constants
#define MAXLEN 7



typedef struct
{
    void *data;
    size_t size;
} Polis;

typedef char Size_elem;

typedef int (*Calc_elem)(const void *elem, Size_elem size, Stack *stack);



int
polis_init(Polis *polis);

int
polis_final(Polis *polis);





int
polis_calculate(const Polis *pol, void *res_p, size_t size_res);

int
polis_add_elem(Polis *polis, const void *data, Size_elem size_elem);

int
polis_create(Polis *polis, char *str, Database *base_vars);





int
polis_get_integer(const void *elem, Size_elem size, Stack *stack);

int
polis_get_double(const void *elem, Size_elem size, Stack *stack);

int
polis_get_operator(const void *elem, Size_elem size, Stack *stack);




