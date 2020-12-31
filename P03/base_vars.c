#include "base_vars.h"


int
base_init(Database *base)
{
    base -> vars = NULL;
    base -> size = 0;
    return 0;
}

int
base_final(Database *base)
{
    if (base -> vars != NULL) {

        for (size_t i = 0; i < base -> size; ++i) {
            free(base -> vars[i].name);
        }

        free(base -> vars);
        base -> vars = NULL;
    }
    base -> size = 0;
    return 0;
}

int
base_create(Database *base, const int num_str)
{
    printf("Enter variables one per line: \n");
    for (int i = 0; i < num_str; ++i) {
        char *str = str_in(0);
        str = str_clear_spaces(str);

        if (strstr(str, "=\0") == NULL) {
            free(str);
            return BAD_VARIABLE_INITIALIZATION;
        }

        str = strtok(str, "=\0");

        size_t len = strlen(str);

        if (len > 6) {
            free(str);
            return BAD_VARIABLE_INITIALIZATION;
        }

        int count = 0;
        int err;
        double number;
        char *name;
        do {

            if (count >= 2) {
                free(str);
                return BAD_VARIABLE_INITIALIZATION;
            }

            // it is name of variable
            if (count == 0) {
                for (int j = 0; j < len; ++j) {
                    if ((isalpha(str[j]) == 0) && (str[j] != '_')) {
                        free(str);
                        return BAD_VARIABLE_INITIALIZATION;
                    }
                }
                name = str;
            }

            // it is it's value
            if (count == 1) {
                err = sscanf(str, "%lf", &number);

                if (err != 1) {
                    free(str);
                    return BAD_VARIABLE_INITIALIZATION;
                }
            }

            count += 1;
        } while ((str = strtok(NULL, "=\0")) != NULL);
        free(str);
        err = base_add(name, number, base);

        if (err != 0) {
            return err;
        }
    }
    return 0;
}



int
base_get_data(const char *name, Data_type *data, const Database *base)
{
    if (base_seek_name(name, base) == 0) {
        return 1;
    }

    for (size_t i = 0; i < base -> size; ++i) {
        if (strcmp(name, base -> vars[i].name) == 0) {
            *data = base -> vars[i].data;
        }
    }

    return 0;
}

// function returns 1, if this variable exist in base, and 0, if it doesn't
// in case of empty database also returns 0
int
base_seek_name(const char *name, const Database *base)
{
    if (base -> vars == NULL) {
        return 0;
    }

    for (size_t i = 0; i < base -> size; ++i) {
        if (strcmp(name, base -> vars[i].name) == 0) {
            return 1;
        }
    }
    return 0;
}

// function that adds a new variable to the base;
// if this variable already exists, function rewrites it's value
int
base_add(char *name, const Data_type data, Database *base)
{
    if (base_seek_name(name, base) == 0) {
        base -> vars = realloc(base -> vars, sizeof(*base -> vars) * (base -> size + 1));

        if (base -> vars == NULL) {
            return BASE_OVERFLOW;
        }

        Variable var;
        var.name = name;
        var.data = data;
        var.num = base -> size;
        base -> vars[base -> size] = var;
        base -> size += 1;
    } else {
        for (size_t i = 0; i < base -> size; ++i) {
            if (strcmp(name, base -> vars[i].name) == 0) {
                base -> vars[i].data = data;
            }
        }
    }
    return 0;
}