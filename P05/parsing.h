#include <ctype.h>
#include <unistd.h>

#include "polis.h"

// errors
#define SYNTAX_ERROR 1
#define BRACKETS_DISBALANCE_MISSING_CLOSE 2
#define BRACKETS_DISBALANCE_MISSING_OPEN 3
#define INVALID_OPERATION 4
#define INVALID_OPERAND 5
#define MISSING_OPERAND 6
#define MEMORY_OVERFLOW 7
#define CRITICAL_ERROR 8
#define IS_A_DIRECTORY 9
#define ERR_OPENING_FILE 10




// parameteres
#define BUF_INIT_SIZE 128


// macroses
#define ERR_MSG(call, msg, err) do { \
                                        if (call != 0) { \
                                            char buf_err[] = msg; \
                                            ERR(write(2, buf_err, \
                                                sizeof buf_err)); \
                                            return err; \
                                        } \
                                    } while (0)


#define ERR(call) if (call == -1) return CRITICAL_ERROR



// types

typedef struct
{
    // current data
    char cur;
    char prev;
    int i;
    int pipe[2];
    int files[2];

    // arrays for jumps
    int *jumps_places;
    int jumps_places_i;
    int *jumps_dests;
    int jumps_dests_i;

    // flags
    int flag_conv;
    int flag_open;
    int flag_or;
    int flag_and;
    int pos_first_or;

    // buf
    char *buf;
    size_t buf_size;
    size_t buf_border;

    // recursion
    int rec_pos;
} Data;


// functions

int
parsing_cmd(void);

int
logic_sum(Data *data, Polis *polis);

int
logic_mult(Data *data, Polis *polis);

int
unary(Data *data, Polis *polis);

int
conv(Data *data, Polis *polis);

int
semicolon(Data *data, Polis *polis);

int
symbol(Data *data, Polis *polis);

int
add_buf(Data *data, char c);

int
dump_buf(Data *data, Polis *polis);

char *
str_clear_spaces_begin(char *str);

int
str_check_spaces(char *str);

char *
str_clear_spaces(char *str);
