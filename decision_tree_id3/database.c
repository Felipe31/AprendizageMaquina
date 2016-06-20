#include "database.h"

/*
Author: Luis Angelo Loss de Castro
*/

// functions to CREATE data
// =============================================

struct database *create_database(int size_database, int quantity_atributes) {
    if (size_database < 1 || quantity_atributes < 2) return NULL;

    struct database *data = (struct database*)calloc(1, sizeof(struct database));
    assert(data);

    data->ids = (struct atributes_id*)calloc(quantity_atributes, sizeof(struct atributes_id));
    assert(data->ids);
    data->quantity_atributes = quantity_atributes;

    data->atributes = (struct atribute**)calloc(size_database, sizeof(struct atribute*));
    assert(data->atributes);
    data->size_database = size_database;

    int i;
    for(i=0; i < size_database; i++) {
        data->atributes[i] = (struct atribute*)calloc(quantity_atributes, sizeof(struct atribute));
        assert(data->atributes[i]);
    }

    return data;
}

struct database *mount_database(const char *file_name, int size_database, int quantity_atributes) {
    assert(file_name);
    if(size_database < 1 || quantity_atributes < 2) return NULL;

    FILE *fp = NULL;
    errno = 0; // variable for error on fopen
    fp = fopen(file_name, "r");
    if(fp == NULL) {
        printf("Errno: %d\n", errno);
        return NULL;
    }

    struct database *data = create_database(size_database, quantity_atributes);
    assert(data);

    char name[100];

    int cont, j;
    for(cont=0; cont < quantity_atributes; cont++) {
        fscanf(fp, "%s", name);
        j = strlen(name);
        data->ids[cont].name = (char *)calloc(j+1, sizeof(char)); // XXX check memory leak
        strcpy(data->ids[cont].name, name);
        data->ids[cont].status = '0'; // set not used
    }

    double i;
    int atrib = cont; // number of atributes
    cont=0;
    while(cont < size_database) {
        for(j=0; j < atrib; j++) {
            fscanf(fp, "%lf,", &i);
            data->atributes[cont][j].value = i;
        }
        cont++;
    }

    fclose(fp);

    return data;
}

// functions to FREE data
// =============================================

void free_database(struct database *data) {
    assert(data);
    assert(data->ids);
    assert(data->atributes);

    int i;
    for(i=0; i < data->quantity_atributes; i++) {
        assert(data->ids[i].name);
        free(data->ids[i].name);
    }
    free(data->ids);

    for(i=0; i < data->size_database; i++) {
        assert(data->atributes[i]);
        free(data->atributes[i]);
    }

    free(data->class_values);
    free(data->atributes);
    free(data);
}

// functions to PRINT data
// =============================================

void print_values_database(struct database *data) {
    assert(data);

    printf("values of the database\n");
    printf("%p, %p, %d, %d, %p, %d\n", data->atributes, data->ids, data->size_database, data->quantity_atributes, data->class_values, data->size_class_values);
}

void print_atributes_name(struct database *data) {
    assert(data);
    assert(data->ids);

    printf("atributes name\n");
    int i;
    for(i=0; i < data->quantity_atributes; i++) {
        assert(data->ids[i].name);
        printf("%s, ", data->ids[i].name);
    }
    printf("\n");
}

void print_all_values_database(struct database *data) {
    assert(data);
    assert(data->atributes);

    int i, j;
    printf("values on the database\n");
    for(i=0; i < data->size_database; i++) {
        for(j=0; j < data->quantity_atributes; j++) {
            assert(data->atributes[i]);
            printf("%f, ", data->atributes[i][j].value);
        }
        printf("\n");
    }
}

void print_values_class(struct database *data) {
    assert(data);
    assert(data->class_values);

    int i;
    printf("possible values of the class\n");
    for(i=0; i < data->size_class_values; i++) {
        printf("%f, ", data->class_values[i]);
    }
    printf("\n");
}
