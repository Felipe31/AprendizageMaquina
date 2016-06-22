#ifndef _FUNCTIONS_DECISION_TREE_ID3
#define _FUNCTIONS_DECISION_TREE_ID3

#define FIXED_SIZE 200

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <math.h>
#include <string.h>
#include "database.h"

// used by position_values
struct position_values {
    struct values_found *values_found;
    struct my_data *positions;
    int size_positions;
    int atribute;
};

// used by atribute_values
struct values_found {
    double *values;
    int size_values_found;
};

// parameter of atribute_values
struct my_data {
    int *my_data_array; // elements selected from the database
    int size_my_data_array;
};

int check_value_exist(double *atributes, int *atributes_found, double new_value);

struct values_found *atribute_values(struct database *data, struct my_data *my_data, int atribute);

void *position_values(struct database *data, struct my_data *my_data, struct position_values *position_values);

double entropy(struct database *data, struct my_data *my_data);

double information_gain(double entropy_parent, struct database *data, struct my_data *my_data, int atribute);

#endif
