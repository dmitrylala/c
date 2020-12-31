#include "rec_descent.h"

int
recursive_descent(char *str, Polis *polis, Database *base_vars)
{
    // input string data
    Input_str input_str;
    input_str.i = 0;
    input_str.str = str;
    input_str.len = strlen(input_str.str);
    input_str.flag_open = 0;

    // output string data
    Output_str output_str;
    output_str.j = 0;
    output_str.str = malloc(sizeof(output_str.str) * (input_str.len + 1));

    input_str.c = nextChar(&input_str);
    input_str.prev = '+'; // initialization with any operator
    int err = begin(&input_str, &output_str);

    if (err != 0) {
        free(output_str.str);
        return err;
    }

    if (input_str.i != input_str.len + 1) {
        free(output_str.str);
        return SYNTAX_ERROR;
    }
    output_str.str[output_str.j] = '\0';

    err = polis_create(polis, output_str.str, base_vars);
    if (err != 0) {
        return err;
    }

    free(output_str.str);
    return 0;
}

int
begin(Input_str *input_str, Output_str *output_str)
{

    int err = mult_div(input_str, output_str);
    if (err != 0) {
        return err;
    }


    while ((input_str -> c == '+') || (input_str -> c == '-')) {
        char temp = input_str -> c;
        input_str -> prev = input_str -> c;
        input_str -> c = nextChar(input_str);

        if (((input_str -> c == '+') || (input_str -> c == '-') ||
            (input_str -> c == '\0')) && ((temp == '+') || (temp == '-'))) {
            return MISSING_OPERAND;
        }

        err = mult_div(input_str, output_str);
        if (err != 0) {
            return err;
        }

        output_str -> str[output_str -> j] = temp;
        output_str -> j += 1;
        output_str -> str[output_str -> j] = ';';
        output_str -> j += 1;
    }
    return 0;
}

int
mult_div(Input_str *input_str, Output_str *output_str)
{
    int err = symbol(input_str, output_str);
    if (err != 0) {
        return err;
    }

    while ((input_str -> c == '*') || (input_str -> c == '/')) {
        char temp = input_str -> c;
        input_str -> prev = input_str -> c;
        input_str -> c = nextChar(input_str);

        if (((input_str -> c == '*') || (input_str -> c == '/') ||
            (input_str -> c == '\0')) && ((temp == '*') || (temp == '/'))) {
            return MISSING_OPERAND;
        }

        err = symbol(input_str, output_str);
        if (err != 0) {
            return err;
        }

        output_str -> str[output_str -> j] = temp;
        output_str -> j += 1;
        output_str -> str[output_str -> j] = ';';
        output_str -> j += 1;
    }
    return 0;
}

int
symbol(Input_str *input_str, Output_str *output_str)
{
    int err;
    if (input_str -> c == '(') {
        input_str -> flag_open += 1;
        input_str -> prev = input_str -> c;
        input_str -> c = nextChar(input_str);
        err = begin(input_str, output_str);
        if (err != 0) {
            return err;
        }
        if (input_str -> c != ')') {
            return BRACKETS_DISBALANCE_MISSING_CLOSE;
        } else {
            input_str -> flag_open -= 1;
            input_str -> prev = input_str -> c;
            input_str -> c = nextChar(input_str);
        }
    } else if ((isalpha(input_str -> c)) || (isnum(input_str -> c)) ||
            (input_str -> c == ' ') || (input_str -> c == '\t')) {

        char prev = input_str -> prev;
        char cur = input_str -> c;

        (output_str -> str)[output_str -> j] = input_str -> c;
        output_str -> j += 1;

        if ((input_str -> c != ' ') && (input_str -> c != '\t')) {
            input_str -> prev = input_str -> c;
        }

        input_str -> c = nextChar(input_str);

        //~ printf("IN SYMBOL:%c%c%c%c\n", input_str -> c, cur, input_str -> prev, prev);

        if (((cur == ' ') || (cur == '\t')) && ((isalpha(prev)) || (isnum(prev))) &&
                ((isalpha(input_str -> c)) || (isnum(input_str -> c)))) {
            return MISSING_OPERATOR;
        }

        err = symbol(input_str, output_str);
        if (err != 0) {
            return err;
        }

        // creating CSV string
        (output_str -> str)[output_str -> j] = ';';
        output_str -> j += 1;

    } else if ((input_str -> c != '+') && (input_str -> c != '-') &&
                (input_str -> c != '*') && (input_str -> c != '/') &&
                (input_str -> c != '\0') && (input_str -> c != ')') &&
                (input_str -> c != ' ')) {

        if ((input_str -> c == '=') || (input_str -> c == '^') || (input_str -> c == '&') ||
            (input_str -> c == '|') || (input_str -> c == '%') || (input_str -> c == '!')) {
            return INVALID_OPERATION;
        }
        return INVALID_OPERAND;
    } else if ((input_str -> c == ')') && (input_str -> flag_open == 0)) {
        return BRACKETS_DISBALANCE_MISSING_OPEN;
    }
    return 0;
}

char
nextChar(Input_str *input_str)
{

    if (input_str -> i <= input_str -> len) {
        char c = (input_str -> str)[input_str -> i];
        input_str -> i += 1;
        return c;
    }
    return '\0';
}

int
isnum(char number)
{
    if (((number >= '0') && (number <= '9')) || (number == '.')){
        return 1;
    }
    return 0;
}
