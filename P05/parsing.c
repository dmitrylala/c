#include "parsing.h"


int
parsing_cmd(void)
{
    // Polis init
    Polis polis;
    polis_init(&polis);


    // input string data
    Data data;
    // symbol
    data.cur = getchar();
    data.i = 1;

    // buf
    data.buf_size = 0;
    data.buf_border = BUF_INIT_SIZE;
    data.buf = malloc(sizeof(*data.buf) * BUF_INIT_SIZE);
    data.buf[0] = '\0';

    // file descriptors
    data.files[0] = 0;
    data.files[1] = 1;

    // arrays for jumps
    data.jumps_places_i = -1;
    data.jumps_dests_i = -1;
    data.jumps_places = malloc(sizeof *data.jumps_places);
    data.jumps_dests = malloc(sizeof *data.jumps_dests);

    // flags
    data.flag_conv = 0;
    data.flag_open = 0;
    data.flag_or = 0;
    data.flag_and = 0;


    // starting descend
    int err;
    err = logic_sum(&data, &polis);

    if (err != 0) {
        fprintf(stderr, "ERROR PARSING: %d\n", err);
        free(data.buf);
        free(data.jumps_dests);
        free(data.jumps_places);
        return err;
    }


    free(data.buf);
    free(data.jumps_dests);
    free(data.jumps_places);



    // polis calculation
    char res;
    if (data.flag_and == 1) {
        if (fork() == 0) {
            err = polis_calculate(&polis, &res, sizeof res);
            polis_final(&polis);
            return err;
        }
    } else {
        err = polis_calculate(&polis, &res, sizeof res);
    }

    polis_final(&polis);
    return err;
}

int
logic_sum(Data *data, Polis *polis)
{

    int err;
    ERR_RET((err = logic_mult(data, polis)), err);

    Calc_elem func;
    Size_elem size_elem;
    if ((data -> buf_size != 0) && ((data -> cur == '\n') || (data -> cur == ')')) &&
            (str_check_spaces(data -> buf) == 0)) {

        // adding buf as cmd to polis
        dump_buf(data, polis);

        int fd_in = 0;
        int fd_out = 1;

            // changing input stream to fd_in
            polis_put_fd(polis, fd_in, &polis_chg_input_stream);

            // changing output stream to fd_out
            polis_put_fd(polis, fd_out, &polis_chg_output_stream);

            // execution
            func = &polis_execute_block;
            size_elem = sizeof func;
            polis_add_elem(polis, &size_elem, sizeof size_elem);
            polis_add_elem(polis, &func, sizeof func);

        if (data -> jumps_dests_i < data -> jumps_places_i) {
            data -> jumps_dests_i += 1;
            data -> jumps_dests = realloc(data -> jumps_dests,
                                            sizeof(*data -> jumps_dests) *
                                            (data -> jumps_dests_i + 1));
            data -> jumps_dests[data -> jumps_dests_i] = polis -> size;

            int pos = polis -> size;
            polis_set_elem(polis, &pos, sizeof pos, data -> jumps_places[data -> jumps_dests_i]);
        }
    }


    int fd_in = data -> files[0], fd_out = data -> files[1];
    int pos_jmp;
    while (data -> cur == '|') {

        // processing
        if ((str_check_spaces(data -> buf) == 0) && (data -> buf_size != 0)) {

            dump_buf(data, polis);

            // changing input stream to fd_in
            polis_put_fd(polis, fd_in, &polis_chg_input_stream);

            // changing output stream to fd_out
            polis_put_fd(polis, fd_out, &polis_chg_output_stream);

            // allocating space for jump
            func = &polis_put_jump;
            size_elem = sizeof pos_jmp + sizeof func;
            polis_add_elem(polis, &size_elem, sizeof size_elem);
            polis_add_elem(polis, &func, sizeof func);
            pos_jmp = polis -> size;
            polis_add_elem(polis, &pos_jmp, sizeof pos_jmp);

            data -> jumps_places_i += 1;
            data -> jumps_places = realloc(data -> jumps_places,
                                            sizeof(*data -> jumps_places) *
                                            (data -> jumps_places_i + 1));
            data -> jumps_places[data -> jumps_places_i] = pos_jmp;

            if (data -> flag_or == 0) {
                data -> flag_or = 1;
                data -> pos_first_or = pos_jmp;
            }

            // execution
            func = &polis_execute_or;
            size_elem = sizeof func;
            polis_add_elem(polis, &size_elem, sizeof size_elem);
            polis_add_elem(polis, &func, sizeof func);
        }

        char tmp1 = data -> prev;
        char tmp2 = data -> cur;

        data -> prev = data -> cur;
        data -> cur = getchar();
        data -> i += 1;

        if (data -> cur == '\n') {
            return MISSING_OPERAND;
        }

        if (tmp1 != tmp2) {
            return 0;
        }


        ERR_RET((err = logic_mult(data, polis)), err);


        // processing
        if (data -> cur == '\n') {
            if ((data -> buf_size != 0) && (str_check_spaces(data -> buf) == 0)) {
                dump_buf(data, polis);
            }

            // changing input stream to fd_in
            polis_put_fd(polis, fd_in, &polis_chg_input_stream);

            // changing output stream to fd_out
            polis_put_fd(polis, fd_out, &polis_chg_output_stream);

            // execution
            func = &polis_execute;
            size_elem = sizeof func;
            polis_add_elem(polis, &size_elem, sizeof size_elem);
            polis_add_elem(polis, &func, sizeof func);
        }

        data -> jumps_dests_i += 1;
        data -> jumps_dests = realloc(data -> jumps_dests, sizeof(*data -> jumps_dests) *
                                        (data -> jumps_dests_i + 1));
        data -> jumps_dests[data -> jumps_dests_i] = polis -> size;

        int pos = polis -> size;
        if (data -> flag_or == 0) {
            polis_set_elem(polis, &pos, sizeof pos, data -> jumps_places[data -> jumps_dests_i]);
        } else {
            polis_set_elem(polis, &pos, sizeof pos, data -> pos_first_or);
        }
    }

    return 0;
}

int
logic_mult(Data *data, Polis *polis)
{

    int err;
    ERR_RET((err = semicolon(data, polis)), err);


    int fd_in = data -> files[0], fd_out = data -> files[1];
    Calc_elem func;
    Size_elem size_elem;
    int pos_jmp, cur_num;
    while (data -> cur == '&') {

        char tmp1 = data -> cur;
        data -> prev = data -> cur;
        data -> cur = getchar();
        data -> i += 1;

        // processing
        if ((str_check_spaces(data -> buf) == 0) && (data -> buf_size != 0)) {

            dump_buf(data, polis);

            if (data -> flag_open == 0) {
                // changing input stream to fd_in
                polis_put_fd(polis, fd_in, &polis_chg_input_stream);

                // changing output stream to fd_out
                polis_put_fd(polis, fd_out, &polis_chg_output_stream);

                if (data -> cur == data -> prev) {
                    // allocating space for jump
                    func = &polis_put_jump;
                    size_elem = sizeof pos_jmp + sizeof func;
                    polis_add_elem(polis, &size_elem, sizeof size_elem);
                    polis_add_elem(polis, &func, sizeof func);
                    pos_jmp = polis -> size;
                    polis_add_elem(polis, &pos_jmp, sizeof pos_jmp);

                    data -> jumps_places_i += 1;
                    cur_num = data -> jumps_places_i;
                    data -> jumps_places = realloc(data -> jumps_places,
                                                    sizeof(*data -> jumps_places) *
                                                    (data -> jumps_places_i + 1));
                    data -> jumps_places[cur_num] = pos_jmp;
                }

                // execution
                if (data -> cur == data -> prev) {
                    func = &polis_execute_and;
                } else {
                    func = &polis_execute;
                }
                size_elem = sizeof func;
                polis_add_elem(polis, &size_elem, sizeof size_elem);
                polis_add_elem(polis, &func, sizeof func);
            }
        }

        if (data -> cur == '\n') {

            // operator '&'
            data -> flag_and = 1;
            return 0;
        }

        char tmp2 = data -> cur;
        data -> cur = getchar();
        data -> i += 1;

        if (tmp1 != tmp2) {
            return SYNTAX_ERROR;
        }

        if (data -> cur == '\n') {
            return MISSING_OPERAND;
        }

        ERR_RET((err = semicolon(data, polis)), err);

        if ((data -> cur != '\n') && (data -> jumps_dests_i < data -> jumps_places_i)) {
            data -> jumps_dests_i += 1;
            data -> jumps_dests = realloc(data -> jumps_dests,
                                            sizeof(*data -> jumps_dests) *
                                            (data -> jumps_dests_i + 1));
            data -> jumps_dests[data -> jumps_dests_i] = polis -> size;
            int pos = polis -> size;
            polis_set_elem(polis, &pos, sizeof pos, data -> jumps_places[data -> jumps_dests_i]);
        }
    }
    return 0;
}

int
semicolon(Data *data, Polis *polis)
{

    int err;
    ERR_RET((err = conv(data, polis)), err);

    int fd_in, fd_out;
    Calc_elem func;
    Size_elem size_elem;

    while (data -> cur == ';') {

        if ((str_check_spaces(data -> buf) == 0) && (data -> buf_size != 0)) {
            fd_in = 0;
            fd_out = 1;
            dump_buf(data, polis);

            // changing input stream to fd_in
            polis_put_fd(polis, fd_in, &polis_chg_input_stream);

            // changing output stream to fd_out
            polis_put_fd(polis, fd_out, &polis_chg_output_stream);

            // execution
            func = &polis_execute_block;
            size_elem = sizeof func;
            polis_add_elem(polis, &size_elem, sizeof size_elem);
            polis_add_elem(polis, &func, sizeof func);
        }

        data -> prev = data -> cur;
        data -> cur = getchar();
        data -> i += 1;

        ERR_RET((err = conv(data, polis)), err);

    }
    return 0;
}


int
conv(Data *data, Polis *polis)
{

    int err;
    ERR_RET((err = symbol(data, polis)), err);

    while ((data -> cur == '|') || (data -> cur == '>') ||
            (data -> cur == '<')) {

        char prev = data -> prev;
        char tmp = data -> cur;

        data -> prev = data -> cur;
        data -> cur = getchar();
        data -> i += 1;


        if ((data -> cur == '\n') || (data -> cur == ')')) {
            return MISSING_OPERAND;
        }

        if (data -> cur == '|') {
            data -> prev = '|';
            return 0;
        }


        char c = data -> prev;

        Calc_elem func;
        Size_elem size_elem;
        int fd_in, fd_out;

        // processing
        if ((data -> buf_size != 0) && (str_check_spaces(data -> buf) == 0)) {
            dump_buf(data, polis);

            if (c == '|') {
                int fd[2];
                ERR(pipe(fd));
                data -> pipe[0] = fd[0];
                data -> pipe[1] = fd[1];

                if (data -> flag_conv == 0) {
                    fd_in = 0;
                    data -> flag_conv = 1;
                } else {
                    fd_in = data -> pipe[1];
                }
                fd_out = data -> pipe[1];

                // changing input stream to fd_in
                polis_put_fd(polis, fd_in, &polis_chg_input_stream);

                // changing output stream to fd_out
                polis_put_fd(polis, fd_out, &polis_chg_output_stream);

                // execution
                func = &polis_execute;
                size_elem = sizeof func;
                polis_add_elem(polis, &size_elem, sizeof size_elem);
                polis_add_elem(polis, &func, sizeof func);
            }
        }

        ERR_RET((err = symbol(data, polis)), err);

        if ((data -> buf_size == 0) || (str_check_spaces(data -> buf) == 1)) {
            return MISSING_OPERAND;
        }


        // opening file and pipe
        fd_in = data -> files[0];
        fd_out = data -> files[1];
        int fd = 0;
        if ((tmp == '>') || (tmp == '<')) {
            data -> buf = str_clear_spaces(data -> buf);

            struct stat s;
            stat(data -> buf, &s);

            int flag_dir = S_ISDIR(s.st_mode);

            if (flag_dir == 1) {
                fprintf(stderr, "< or >: argument is a directory\n");
                return IS_A_DIRECTORY;
            }

            fd = open(data -> buf, O_RDWR | O_CREAT, 0666);

            if (fd == -1) {
                fprintf(stderr, "< or >: error opening file\n");
                return ERR_OPENING_FILE;
            }
        } else if (tmp == '|') {
            dump_buf(data, polis);
        }


        if (tmp == '>') {

            // changing output stream to fd of file
            fd_out = fd;
            data -> files[1] = fd;

            data -> buf[0] = '\0';
            data -> buf_size = 0;

        } else if (tmp == '|') {

            // changing input stream to pipe_prev[0]
            fd_in = data -> pipe[0];

            // changing output stream to fd_out
            int fd_2[2];
            ERR(pipe(fd_2));
            data -> pipe[1] = fd_2[1];
            fd_out = fd_2[1];
            c = data -> cur;
            if ((c != '|') && (c != '<') && (c != '>')) {
                data -> flag_conv = 0;
                fd_out = 1;
            }

            data -> pipe[0] = fd_2[0];

        } else if (tmp == '<') {

            // changing input_stream to fd of file
            fd_in = fd;

            data -> files[0] = fd;

            data -> buf[0] = '\0';
            data -> buf_size = 0;
        }


        c = data -> cur;
        if ((c != '<') && (c != '>')) {

            // changing streams
            polis_put_fd(polis, fd_in, &polis_chg_input_stream);
            polis_put_fd(polis, fd_out, &polis_chg_output_stream);

            // execution
            if (prev == ')') {
                if (c == ';') {
                    func = &polis_execute_block;
                } else if (tmp == '|') {
                    func = &polis_execute;
                }
            } else {
                if ((c == ';') || (c == '\n')) {
                    func = &polis_execute_block;
                } else if (c == '&') {
                    // allocating space for jump
                    func = &polis_put_jump;
                    int pos_jmp;
                    size_elem = sizeof pos_jmp + sizeof func;
                    polis_add_elem(polis, &size_elem, sizeof size_elem);
                    polis_add_elem(polis, &func, sizeof func);
                    pos_jmp = polis -> size;
                    polis_add_elem(polis, &pos_jmp, sizeof pos_jmp);

                    data -> jumps_places_i += 1;
                    int cur_num = data -> jumps_places_i;
                    data -> jumps_places = realloc(data -> jumps_places,
                                                    sizeof(*data -> jumps_places) *
                                                    (data -> jumps_places_i + 1));
                    data -> jumps_places[cur_num] = pos_jmp;
                    func = &polis_execute_and;
                } else if ((tmp == '|') || (tmp == '<') || (tmp == '>')) {
                    func = &polis_execute;
                }
            }
            size_elem = sizeof func;
            polis_add_elem(polis, &size_elem, sizeof size_elem);
            polis_add_elem(polis, &func, sizeof func);
        }
    }
    return 0;
}




int
symbol(Data *data, Polis *polis)
{

    int err;
    if (data -> cur == '(') {

        // putting the special element to polis
        data -> flag_open += 1;
        data -> prev = data -> cur;
        data -> cur = getchar();
        data -> i += 1;

        Calc_elem func = &polis_skip;
        Size_elem size_elem = sizeof func;
        polis_add_elem(polis, &size_elem, sizeof size_elem);
        data -> rec_pos = polis -> size;
        polis_add_elem(polis, &func, sizeof func);


        // starting descend
        err = logic_sum(data, polis);

        if (err != 0) {
            return err;
        }


        size_t new_size = polis -> size  - data -> rec_pos + sizeof func;
        polis_set_elem(polis, &new_size, sizeof new_size, data -> rec_pos);

        func = &polis_execute_all;
        size_elem = sizeof func;
        polis_add_elem(polis, &size_elem, sizeof size_elem);
        polis_add_elem(polis, &func, sizeof func);


        if (data -> cur != ')') {
            return BRACKETS_DISBALANCE_MISSING_CLOSE;
        } else {
            data -> flag_open -= 1;

            data -> prev = data -> cur;
            data -> cur = getchar();
            data -> i += 1;

            ERR_RET((err = symbol(data, polis)), err);
        }
    } else if (isalpha(data -> cur) || ((data -> cur >= '0') &&
                (data -> cur <= '9')) || (data -> cur == ' ') ||
                (data -> cur == '.') || (data -> cur == '/') ||
                (data -> cur == '-')) {
        ERR_RET((err = add_buf(data, data -> cur)), err);

        if ((data -> cur != ' ') && (data -> cur != '\t')) {
            data -> prev = data -> cur;
        }
        data -> cur = getchar();
        data -> i += 1;

        ERR_RET((err = symbol(data, polis)), err);
    } else if ((data -> cur != '|') && (data -> cur != '>') && (data -> cur != '<') &&
                (data -> cur != '&') && (data -> cur != '\n') && (data -> cur != ')') &&
                (data -> cur != ';')) {

        if ((data -> cur == '=') || (data -> cur == '^') ||
            (data -> cur == '%') || (data -> cur == '!')) {
            return INVALID_OPERATION;
        }
        return INVALID_OPERAND;
    } else if ((data -> cur == ')') && (data -> flag_open == 0)) {
        return BRACKETS_DISBALANCE_MISSING_OPEN;
    }
    return 0;
}


int
add_buf(Data *data, char c)
{
    if (data -> buf_size + 2 * sizeof c > data -> buf_border) {
        data -> buf_border *= 2;
        data -> buf = realloc(data -> buf, sizeof(*data -> buf) *
                                data -> buf_border);

        if (data -> buf == NULL) {
            return MEMORY_OVERFLOW;
        }
    }

    data -> buf[data -> buf_size] = c;
    data -> buf[data -> buf_size + 1] = '\0';

    data -> buf_size += 1;
    return 0;
}

int
dump_buf(Data *data, Polis *polis)
{
    data -> buf = str_clear_spaces_begin(data -> buf);
    Calc_elem func = &polis_get_string;
    Size_elem size_elem = data -> buf_size + sizeof func;
    polis_add_elem(polis, &size_elem, sizeof size_elem);
    polis_add_elem(polis, &func, sizeof func);
    polis_add_elem(polis, data -> buf, data -> buf_size);

    data -> buf[0] = '\0';
    data -> buf_size = 0;

    return 0;
}

char *
str_clear_spaces_begin(char *str)
{

    int begin = 0;
    size_t len = strlen(str);

    for(int i = 0; i < len; ++i) {
        if ((str[i] != ' ') && (str[i] != '\t')) {
            begin = i;
            break;
        }
    }

    if (begin == 0) {
        return str;
    }

    for (int i = begin; i < len; ++i) {
        str[i - begin] = str[i];
    }
    str[len - begin] = '\0';

    return str;
}

int
str_check_spaces(char *str)
{
    int count = 0, flag = 0;
    for(int i = 0; i < strlen(str); ++i) {
        if ((str[i] == ' ') || (str[i] == '\t')) {
            count++;
        }
    }

    if (count == strlen(str)) {
        flag = 1;
    }

    return flag;
}

char *
str_clear_spaces(char *str)
{
    int end = 0;
    for(int i = 0; i < strlen(str); ++i) {
        if ((str[i] != ' ') && (str[i] != '\t')) {
            if (i != end) {
                str[end] = str[i];
            }
            ++end;
        }
    }
    str[end] = '\0';
    return str;
}
