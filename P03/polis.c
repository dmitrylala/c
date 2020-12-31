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
        void *elem = &((char *)pol -> data)[curr_size + sizeof(Size_elem)];

        Calc_elem func;
        memcpy(&func, elem, sizeof(Calc_elem));
        SAFE(func(elem, ((char *)pol -> data)[curr_size], &stack));
    }

    Size_elem res_size_stack;
    SAFE(stack_pop(&stack, &res_size_stack, sizeof(res_size_stack)));

    if (res_size_stack < size_res) {
        flag = STACK_UNDERFLOW;
        goto FINALLY;
    }

    SAFE(stack_pop(&stack, res_p, size_res));

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
polis_create(Polis *polis, char *str, Database *base_vars)
{
    double num_db;
    int num_int, err;
    char operator;
    char *operators[4] = {"+\0", "-\0", "*\0", "/\0"};

    str = strtok(str, ";");
    char *str1;
    do {
        str1 = str_clear_spaces(str);
        // this token is double number
        if (strstr(str, ".\0") != NULL) {
            double num_2;
            err = sscanf(str, "%lf%lf", &num_db, &num_2);
            if (err != 1) {
                return MISSING_OPERATOR;
            }

            // data will be located as  size_elem|func_pointer|number
            Size_elem size_elem = sizeof(double) + sizeof(Calc_elem);
            Calc_elem func = &polis_get_double;
            polis_add_elem(polis, &size_elem, sizeof(Size_elem));
            polis_add_elem(polis, &func, sizeof(Calc_elem));
            polis_add_elem(polis, &num_db, sizeof(double));

        // this token is not double number
        } else {

            // it is an operator
            if ((strstr(str, operators[0]) != NULL) || (strstr(str, operators[1]) != NULL) ||
                (strstr(str, operators[2]) != NULL) || (strstr(str, operators[3]) != NULL)) {

                err = sscanf(str, "%c", &operator);
                if (err != 1) {
                    return SYNTAX_ERROR;
                }

                Size_elem size_elem = sizeof(char) + sizeof(Calc_elem);
                Calc_elem func = &polis_get_operator;
                polis_add_elem(polis, &size_elem, sizeof(Size_elem));
                polis_add_elem(polis, &func, sizeof(Calc_elem));
                polis_add_elem(polis, &operator, sizeof(char));

            // it is variable
            } else if (isalpha(str1[0])) {

                if (base_seek_name(str1, base_vars) == 0) {
                    return UNINITIALIZED_VARIABLE;
                }

                base_get_data(str1, &num_db, base_vars);

                Size_elem size_elem = sizeof(double) + sizeof(Calc_elem);
                Calc_elem func = &polis_get_double;
                polis_add_elem(polis, &size_elem, sizeof(Size_elem));
                polis_add_elem(polis, &func, sizeof(Calc_elem));
                polis_add_elem(polis, &num_db, sizeof(double));

            // it is integer
            } else {
                double num_2;
                err = sscanf(str, "%d%lf", &num_int, &num_2);

                if (err != 1) {
                    return MISSING_OPERATOR;
                }

                Size_elem size_elem = sizeof(int) + sizeof(Calc_elem);
                Calc_elem func = &polis_get_integer;
                polis_add_elem(polis, &size_elem, sizeof(Size_elem));
                polis_add_elem(polis, &func, sizeof(Calc_elem));
                polis_add_elem(polis, &num_int, sizeof(int));
            }
        }
    } while ((str = strtok(NULL, ";\0")) != NULL);

    return 0;
}

int
polis_get_integer(const void *elem, Size_elem size, Stack *stack)
{
    int n = *(int *)(&((char *)elem)[sizeof(Calc_elem)]);
    stack_push(stack, &n, sizeof(int));

    Size_elem size_data = sizeof(int);
    stack_push(stack, &size_data, sizeof(size_data));

    return 0;
}

int
polis_get_double(const void *elem, Size_elem size, Stack *stack)
{
    double n = *(double *)(&((char *)elem)[sizeof(Calc_elem)]);
    stack_push(stack, &n, sizeof(double));

    Size_elem size_data = sizeof(double);
    stack_push(stack, &size_data, sizeof(size_data));

    return 0;
}

int
polis_get_operator(const void *elem, Size_elem size, Stack *stack)
{
    char op = *(char *)(&((char *)elem)[sizeof(Calc_elem)]);

    int num1_int, num2_int, res_int;
    double num1_db, num2_db, res_db;


    Size_elem size1, size2;

    stack_pop(stack, &size2, sizeof(size2));
    if (size2 == sizeof(int)) {
        stack_pop(stack, &num2_int, size2);
    } else if (size2 == sizeof(double)) {
        stack_pop(stack, &num2_db, size2);
    } else {
        return INCORRECT_VALUE;
    }

    stack_pop(stack, &size1, sizeof(size1));
    if (size1 == sizeof(int)) {
        stack_pop(stack, &num1_int, size1);
    } else if (size1 == sizeof(double)) {
        stack_pop(stack, &num1_db, size1);
    } else {
        return INCORRECT_VALUE;
    }


    if ((size1 == sizeof(int)) && (size2 == sizeof(int))) {
        if (op == '+') {
            res_int = num1_int + num2_int;
        } else if (op == '-') {
            res_int = num1_int - num2_int;
        } else if (op == '*') {
            res_int = num1_int * num2_int;
        } else if (op == '/') {

            if (num2_int == 0) {
                return DIVIZION_BY_ZERO;
            }

            res_int = num1_int / num2_int;
        } else {
            return INCORRECT_VALUE;
        }
        res_db = (double)res_int;
    } else {
        if ((size1 == sizeof(double)) && (size2 == sizeof(int))) {
            num2_db = (double)num2_int;
        } else if ((size2 == sizeof(double)) && (size1 == sizeof(int))) {
            num1_db = (double)num1_int;
        }

        if (op == '+') {
            res_db = num1_db + num2_db;
        } else if (op == '-') {
            res_db = num1_db - num2_db;
        } else if (op == '*') {
            res_db = num1_db * num2_db;
        } else if (op == '/') {

            if (num2_db == 0) {
                return DIVIZION_BY_ZERO;
            }

            res_db = num1_db / num2_db;
        } else {
            return INCORRECT_VALUE;
        }
    }

    stack_push(stack, &res_db, sizeof(res_db));
    Size_elem size_res = sizeof(res_db);
    stack_push(stack, &size_res, sizeof(size_res));

    return 0;
}


