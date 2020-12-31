#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "utilities.h"


// errors
#define UNINITIALIZED_VARIABLE 10
#define BASE_OVERFLOW 11
#define EMPTY_DATABASE 12
#define BAD_VARIABLE_INITIALIZATION 13
#define BAD_NUMBER_VARIABLES 14


typedef double Data_type;

typedef struct
{
    char *name;
    Data_type data;
    int num;
} Variable;

typedef struct
{
    Variable *vars;
    size_t size;
} Database;


int
base_init(Database *base);

int
base_final(Database *base);

int
base_create(Database *base, const int num_str);


int
base_get_data(const char *name, Data_type *data, const Database *base);

int
base_seek_name(const char *name, const Database *base);

int
base_add(char *name, const Data_type data, Database *base);

