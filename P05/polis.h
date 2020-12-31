#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "stack.h"


#define SYNTAX_ERROR 1
#define DIVIZION_BY_ZERO 8
#define POLIS_OVERFLOW 11
#define EXEC_ERR 12
#define ERR_MEMORY_ALLOCATION 13



#define SAFE(call) do { \
                        flag = call; \
                        if ((flag != 0) && (flag < 100)) { \
                            goto FINALLY; \
                        } \
                    } \
                    while (0)

#define ERR_RET(call, err) do { \
                                if (call != 0) { \
                                    return err; \
                                } \
                            } while (0)

#define ERR_MEM(call) do { \
                        if ((call) == NULL) { \
                            char buf_err[] = {"error memory allocation\n"}; \
                            ERR(write(2, buf_err, sizeof buf_err)); \
                            return ERR_MEMORY_ALLOCATION; \
                        } \
                    } while (0)

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
polis_set_elem(Polis *polis, const void *data, Size_elem size_elem, size_t pos);





int
polis_chg_output_stream(const void *elem, Size_elem size, Stack *stack);

int
polis_chg_input_stream(const void *elem, Size_elem size, Stack *stack);





//     executions:

// without wait
int
polis_execute(const void *elem, Size_elem size, Stack *stack);

// with wait
int
polis_execute_block(const void *elem, Size_elem size, Stack *stack);

// for brackets - doesn't work((((
int
polis_execute_all(const void *elem, Size_elem size, Stack *stack);

// for &&
int
polis_execute_and(const void *elem, Size_elem size, Stack *stack);

// for ||
int
polis_execute_or(const void *elem, Size_elem size, Stack *stack);







int
polis_get_string(const void *elem, Size_elem size, Stack *stack);

int
polis_put_fd(Polis *polis, int fd, Calc_elem func);

int
polis_put_jump(const void *elem, Size_elem size, Stack *stack);

int
polis_skip(const void *elem, Size_elem size, Stack *stack);




char **
get_cmd(char *str);

int
free_cmd(char **cmd);


