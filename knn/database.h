#ifndef _DATABASE
#define _DATABASE

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>

struct atribute {
    double value;
};

struct atributes_id {
    char *name;
    char status; // '0' = not used, '1' = used
};

struct database {
    struct atribute **atributes; // table with all the tuples
    struct atributes_id *ids; // only one tuple with the name of classes and your status
    int quantity_atributes;
    int size_database;
    double *class_values; // array with the possible values of the class
    int size_class_values;
};

// functions to CREATE data
// =============================================

struct database *create_database(int size_database, int quantity_atributes);

struct database *mount_database(const char *file_name, int size_database, int quantity_atributes);

// functions to FREE data
// =============================================

void free_database(struct database *data);

// functions to PRINT data
// =============================================

void print_values_database(struct database *data);

void print_atributes_name(struct database *data);

void print_all_values_database(struct database *data);

void print_values_class(struct database *data);

#endif
