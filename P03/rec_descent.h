#include "polis.h"


// errors
#define SYNTAX_ERROR 1
#define BRACKETS_DISBALANCE_MISSING_CLOSE 2
#define BRACKETS_DISBALANCE_MISSING_OPEN 3
#define INVALID_OPERATION 4
#define INVALID_OPERAND 5
#define MISSING_OPERAND 6


typedef struct
{
    char c;
    char prev;
    int i;
    char *str;
    int len;
    int flag_open;
} Input_str;

typedef struct
{
    char *str;
    int j;
} Output_str;


int
recursive_descent(char *str, Polis *polis, Database *base_vars);

int
begin(Input_str *input_str, Output_str *output_str);

int
mult_div(Input_str *input_str, Output_str *output_str);

int
symbol(Input_str *input_str, Output_str *output_str);

char
nextChar(Input_str *input_str);

int
isnum(char number);
