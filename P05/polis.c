#include "polis.h"

int
polis_init(Polis *polis)
{
    polis -> data = NULL;
    polis -> size = 0;
    return 0;
}


int
polis_final(Polis *polis)
{
    if (polis -> data != NULL) {
        free(polis -> data);
        polis -> data = NULL;
    }
    polis -> size = 0;
    return 0;
}


int
polis_calculate(const Polis *pol, void *res_p, size_t size_res)
{

    Stack stack;
    int flag = stack_init(&stack);
    if (flag != 0) {
        return flag;
    }

    for (size_t curr_size = 0; curr_size < pol -> size; curr_size += sizeof(Size_elem) +
            ((char *)pol -> data)[curr_size]) {

        if (flag > curr_size + 100) {
            continue;
        } else {
            flag = 0;
        }


        void *elem = &((char *)pol -> data)[curr_size + sizeof(Size_elem)];

        Calc_elem func;
        memcpy(&func, elem, sizeof(Calc_elem));
        SAFE(func(elem, ((char *)pol -> data)[curr_size], &stack));
    }

    while (wait(NULL) != -1);

    if (stack_getsize(&stack) > 0) {
        flag = STACK_OVERFLOW;
        goto FINALLY;
    }

    stack_final(&stack);
    return 0;

FINALLY:
    stack_final(&stack);
    return flag;
}


int
polis_add_elem(Polis *polis, const void *data_p, Size_elem size_elem)
{
    size_t prev_size = polis -> size;
    polis -> size += size_elem;
    polis -> data = (char *)realloc(polis -> data, sizeof(char) * polis -> size);

    if (polis -> data == NULL) {
        return POLIS_OVERFLOW;
    }

    for (size_t i = 0; i < size_elem; ++i) {
        ((char *)polis -> data)[i + prev_size] = ((char *)data_p)[i];
    }

    return 0;
}

int
polis_set_elem(Polis *polis, const void *data_p, Size_elem size_elem,
                size_t pos)
{
    if (polis -> data == NULL) {
        return POLIS_OVERFLOW;
    }

    for (size_t i = 0; i < polis -> size; ++i) {
        if (i == pos) {
            for (size_t j = 0; j < size_elem; ++j) {
                ((char *)polis -> data)[i + j] = ((char *)data_p)[j];
            }
            break;
        }
    }

    return 0;
}

int
polis_chg_output_stream(const void *elem, Size_elem size, Stack *stack)
{

    int n = ((char *)elem)[sizeof(Calc_elem)];

    Size_elem size_data = sizeof n;
    stack_push(stack, &n, size_data);
    stack_push(stack, &size_data, sizeof size_data);

    return 0;
}

int
polis_chg_input_stream(const void *elem, Size_elem size, Stack *stack)
{
    int n = ((char *)elem)[sizeof(Calc_elem)];

    Size_elem size_data = sizeof n;
    stack_push(stack, &n, size_data);
    stack_push(stack, &size_data, sizeof size_data);

    return 0;
}

int
polis_execute(const void *elem, Size_elem size, Stack *stack)
{

    Size_elem size_out;
    int fd_out, err;
    ERR_RET((err = stack_pop(stack, &size_out, sizeof size_out)), err);
    ERR_RET((err = stack_pop(stack, &fd_out, size_out)), err);

    Size_elem size_in;
    int fd_in;
    stack_pop(stack, &size_in, sizeof size_in);
    stack_pop(stack, &fd_in, size_in);

    Size_elem size_cmd;
    stack_pop(stack, &size_cmd, sizeof size_cmd);
    char *str = malloc((size_cmd + 1) * sizeof *str);
    stack_pop(stack, str, size_cmd);
    str[(int)size_cmd] = '\0';

    char **cmd = get_cmd(str);
    free(str);

    if (fork() == 0) {

        if (fd_in != 0) {
            dup2(fd_in, 0);
            close(fd_in);
        }

        if (fd_out != 1) {
            dup2(fd_out, 1);
            close(fd_out);
        }

        execvp(cmd[0], cmd);
        exit(1);
    }

    if (fd_in != 0) {
        dup2(fd_in, 0);
    }
    close(fd_in);
    close(fd_out);


    free_cmd(cmd);

    return 0;
}

int
polis_execute_block(const void *elem, Size_elem size, Stack *stack)
{

    Size_elem size_out;
    int fd_out, err;
    ERR_RET((err = stack_pop(stack, &size_out, sizeof size_out)), err);
    ERR_RET((err = stack_pop(stack, &fd_out, size_out)), err);

    Size_elem size_in;
    int fd_in;
    stack_pop(stack, &size_in, sizeof size_in);
    stack_pop(stack, &fd_in, size_in);

    Size_elem size_cmd;
    stack_pop(stack, &size_cmd, sizeof size_cmd);
    char *str = malloc((size_cmd + 1) * sizeof *str);
    stack_pop(stack, str, size_cmd);
    str[(int)size_cmd] = '\0';

    char **cmd = get_cmd(str);
    free(str);

    int status;
    pid_t pid;
    if ((pid = fork()) == 0) {

        if (fd_in != 0) {
            dup2(fd_in, 0);
            close(fd_in);
        }

        if (fd_out != 1) {
            dup2(fd_out, 1);
            close(fd_out);
        }

        execvp(cmd[0], cmd);
        exit(1);
    }

    waitpid(pid, &status, 0);

    if (fd_in != 0) {
        dup2(fd_in, 0);
        close(fd_in);
    }

    free_cmd(cmd);

    return 0;
}

int
polis_execute_all(const void *elem, Size_elem size, Stack *stack)
{


    int err;
    //~ size_t polis_size = size;


            //~ *(size_t *)&(((char *)elem)[*(size_t *)(size)]);

    // file descriptors
    Size_elem size_out;
    int fd_out;
    ERR_RET((err = stack_pop(stack, &size_out, sizeof size_out)), err);
    ERR_RET((err = stack_pop(stack, &fd_out, size_out)), err);

    Size_elem size_in;
    int fd_in;
    ERR_RET((err = stack_pop(stack, &size_in, sizeof size_in)), err);
    ERR_RET((err = stack_pop(stack, &fd_in, size_in)), err);


    int status;
    //~ char res;
    pid_t pid;
    if ((pid = fork()) == 0){
        if (fd_in != 0) {
            dup2(fd_in, 0);
            close(fd_in);
        }

        if (fd_out != 1) {
            dup2(fd_out, 1);
            close(fd_out);
        }


        //~ err = polis_calculate(polis, &res, sizeof res);
        exit(0);
    }

    while (1) {
        waitpid(pid, &status, 0);
    }


    if (fd_in != 0) {
        dup2(fd_in, 0);
    }
    close(fd_in);
    close(fd_out);


    return 0;
}

int
polis_execute_and(const void *elem, Size_elem size, Stack *stack)
{

    Size_elem size_jmp;
    int pos_jmp, err;
    ERR_RET((err = stack_pop(stack, &size_jmp, sizeof size_jmp)), err);
    ERR_RET((err = stack_pop(stack, &pos_jmp, size_jmp)), err);

    Size_elem size_out;
    int fd_out;
    ERR_RET((err = stack_pop(stack, &size_out, sizeof size_out)), err);
    ERR_RET((err = stack_pop(stack, &fd_out, size_out)), err);

    Size_elem size_in;
    int fd_in;
    stack_pop(stack, &size_in, sizeof size_in);
    stack_pop(stack, &fd_in, size_in);

    Size_elem size_cmd;
    stack_pop(stack, &size_cmd, sizeof size_cmd);
    char *str = malloc((size_cmd + 1) * sizeof *str);
    stack_pop(stack, str, size_cmd);
    str[(int)size_cmd] = '\0';


    char **cmd = get_cmd(str);
    free(str);

    int status;
    pid_t pid;
    if ((pid = fork()) == 0) {

        if (fd_in != 0) {
            dup2(fd_in, 0);
            close(fd_in);
        }

        if (fd_out != 1) {
            dup2(fd_out, 1);
            close(fd_out);
        }

        execvp(cmd[0], cmd);
        exit(1);
    }

    waitpid(pid, &status, 0);

    if (fd_in != 0) {
        dup2(fd_in, 0);
        close(fd_in);
    }

    if (status != 0) {
        free_cmd(cmd);

        return 100 + pos_jmp;
    }

    free_cmd(cmd);


    return 0;
}

int
polis_execute_or(const void *elem, Size_elem size, Stack *stack)
{

    Size_elem size_jmp;
    int pos_jmp, err;
    ERR_RET((err = stack_pop(stack, &size_jmp, sizeof size_jmp)), err);
    ERR_RET((err = stack_pop(stack, &pos_jmp, size_jmp)), err);

    Size_elem size_out;
    int fd_out;
    ERR_RET((err = stack_pop(stack, &size_out, sizeof size_out)), err);
    ERR_RET((err = stack_pop(stack, &fd_out, size_out)), err);

    Size_elem size_in;
    int fd_in;
    stack_pop(stack, &size_in, sizeof size_in);
    stack_pop(stack, &fd_in, size_in);

    Size_elem size_cmd;
    stack_pop(stack, &size_cmd, sizeof size_cmd);
    char *str = malloc((size_cmd + 1) * sizeof *str);
    stack_pop(stack, str, size_cmd);
    str[(int)size_cmd] = '\0';

    char **cmd = get_cmd(str);
    free(str);

    int status;
    pid_t pid;
    if ((pid = fork()) == 0) {

        if (fd_in != 0) {
            dup2(fd_in, 0);
            close(fd_in);
        }

        if (fd_out != 1) {
            dup2(fd_out, 1);
            close(fd_out);
        }

        execvp(cmd[0], cmd);
        exit(1);
    }

    waitpid(pid, &status, 0);

    if (fd_in != 0) {
        dup2(fd_in, 0);
        close(fd_in);
    }

    if (status == 0) {
        free_cmd(cmd);

        return 100 + pos_jmp;
    }

    free_cmd(cmd);

    return 0;
}

int
polis_get_string(const void *elem, Size_elem size, Stack *stack)
{

    char *str = &((char *)elem)[sizeof(Calc_elem)];
    size_t len = size - sizeof(Calc_elem);
    char *cmd = malloc((len + 1) * sizeof *cmd);

    memcpy(cmd, str, len);
    cmd[len] = '\0';

    Size_elem size_data = strlen(cmd);
    stack_push(stack, cmd, size_data);
    stack_push(stack, &size_data, sizeof size_data);

    free(cmd);

    return 0;
}

int
polis_put_fd(Polis *polis, int fd, Calc_elem func)
{
    Size_elem size_elem = sizeof fd + sizeof func;
    polis_add_elem(polis, &size_elem, sizeof size_elem);
    polis_add_elem(polis, &func, sizeof func);
    polis_add_elem(polis, &fd, sizeof fd);

    return 0;
}

int
polis_put_jump(const void *elem, Size_elem size, Stack *stack)
{
    int num;
    memcpy(&num, &((char *)elem)[sizeof(Calc_elem)], sizeof num);

    Size_elem size_data = sizeof num;
    stack_push(stack, &num, size_data);
    stack_push(stack, &size_data, sizeof size_data);

    return 0;
}

int
polis_skip(const void *elem, Size_elem size, Stack *stack)
{
    return 0;
}
