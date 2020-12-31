#include "stack.h"


int
stack_init(Stack *stack)
{
    stack -> data = malloc(sizeof(*stack -> data) * STACK_INIT_SIZE);

    if (stack -> data == NULL) {
        return STACK_OVERFLOW;
    }

    stack -> size = 0;
    stack -> border = STACK_INIT_SIZE;
    return 0;
}

int
stack_final(Stack *stack)
{
    if (stack -> data != NULL) {
        free(stack -> data);
        stack -> data = NULL;
    }
    stack -> size = 0;
    stack -> border = STACK_INIT_SIZE;
    return 0;
}

int
stack_push(Stack *stack, const void *value_p, size_t size_value)
{
    while (stack -> size + size_value > stack -> border) {
        stack -> border += STACK_STEP;
        stack -> data = realloc(stack -> data, sizeof(*stack -> data) *
                                stack -> border);

        if (stack -> data == NULL) {
            return STACK_OVERFLOW;
        }
    }

    for (size_t i = 0; i < size_value; ++i) {
        stack -> data[i + stack -> size] = ((const char *)value_p)[i];
    }
    stack -> size += size_value;
    return 0;
}

int
stack_pop(Stack *stack, void *res_p, size_t size_res)
{
    if (stack -> size < size_res) {
        return STACK_UNDERFLOW;
    }

    for (size_t i = 0; i < size_res; ++i) {
        ((char *)res_p)[i] = stack -> data[stack -> size - size_res + i];
    }

    stack -> size -= size_res;
    return 0;
}

size_t
stack_getsize(const Stack *stack)
{
    return stack -> size;
}