#include <stdlib.h>


// errors
#define STACK_OVERFLOW 15
#define STACK_UNDERFLOW 16


// constants
#define STACK_INIT_SIZE 200 // init value for border
#define STACK_STEP 600 // value for reallocating memory


/*
    stack struct:
        char *data - for data itself
        size_t size - current size of stack in bytes
        size_t border - upper bound of allocated for stack memory in bytes
*/
typedef struct
{
    char *data;
    size_t size;
    size_t border;
} Stack;


// stack initialization and finalize functions
int
stack_init(Stack *stack);

int
stack_final(Stack *stack);


// operations with stack - push and pop
int
stack_push(Stack *stack, const void *value_p, size_t size_value);

int
stack_pop(Stack *stack, void *res_p, size_t size_res);


// get functions
size_t
stack_getsize(const Stack *stack);
