#include "database.h"
#include "functions_decision_tree_id3.h"


// RECEIVE:
//      an array with values, her size and a new value
// RETURN:
//      if ok then
//          an information if the new value exists and add the new value on the array
//      else
//          -1
// PARAMETERS:
//      atributes_found = quantity of atributes found
//      atributes = array with the atributes already found
//      new_value = new value to search in the array atributes
int check_value_exist(double *atributes, int *atributes_found, double new_value) {
    assert(atributes);
    assert(atributes_found);
    if(*atributes_found < 1) return -1;

    int i=0;
    while(i < *atributes_found) {
        if(new_value == atributes[i]) {
            return 1; // atributes already exist
        }
        i++;
    }
    atributes[i] = new_value; // set the new atribute
    (*atributes_found)++;
    return 0; // new atribute
}

// RECEIVE:
//      the database, an array with my tuples of the database,
//      her size and which atribute search
// RETURN:
//      if ok then
//          an array with the values found and her size(quantity of values)
//          XXX DYNAMIC ALLOCATION
//      else
//          NULL
// PARAMETERS:
//      data = database
//      my_data = array with my data in the database and her size
//      atribute = indice of the atribute in the database
// PRECAUTIONS
//      array static has fixed value handle by FIXED_SIZE definition
//      RETURNS DYNAMIC ALLOCATION
//      uses check_value_exist function
struct values_found *atribute_values(struct database *data, struct my_data *my_data, int atribute) {
    assert(data);
    assert(data->atributes);
    assert(my_data);
    assert(my_data->my_data_array);
    if(my_data->size_my_data_array > data->size_database) return NULL;
    if(my_data->size_my_data_array < 0) return NULL;
    if(atribute >= data->quantity_atributes) return NULL;
    if(atribute < 0) return NULL;

    int size_values_found=1;
    double values[FIXED_SIZE];

    int i;
    for(i=0; i < FIXED_SIZE; i++) {
        values[i] = 0.0;
    }
    values[0] = data->atributes[my_data->my_data_array[0]][atribute].value;
    for(i=1; i < my_data->size_my_data_array; i++) {
        check_value_exist(values, &size_values_found, data->atributes[my_data->my_data_array[i]][atribute].value);
    }

    struct values_found *values_found = (struct values_found *)calloc(1, sizeof(struct values_found));
    assert(values_found);

    values_found->values = (double *)calloc(size_values_found, sizeof(double));
    assert(values_found->values);
    values_found->size_values_found = size_values_found;

    for(i=0; i < size_values_found; i++) {
        values_found->values[i] = values[i];
    }
    return values_found;
}

// RECEIVE:
//      the database, struct my_data that have an array with tuples of the data
//      and her size
//      an struct position_values that have the values found, an array
//      my_data NULL, her size, and the atribute to search for
// RETURN:
//      if ok then
//          an array(matrix) with the indices of correspondent values
//          XXX DYNAMIC ALLOCATION
//          he return, but, he armazenate on the struct my_data position_values
//      else
//          NULL
// PARAMETERS:
//      data = database
//      position_values = struct my_data NULL, to be filled
//          atribute inside position_values = indice of the atribute in the database
//      my_data = an struct with the indices of my data and her size
// PRECAUTIONS
//      RETURNS DYNAMIC ALLOCATION
//      inside parameter struct position_values *position_values
void *position_values(struct database *data, struct my_data *my_data, struct position_values *position_values) {
    assert(data);
    assert(data->atributes);
    assert(my_data);
    assert(my_data->my_data_array);
    if(my_data->size_my_data_array > data->size_database) return NULL;
    if(my_data->size_my_data_array < 0) return NULL;
    assert(position_values->values_found);
    assert(position_values->values_found->values);
    if(position_values->atribute >= data->quantity_atributes) return NULL;
    if(position_values->atribute < 0) return NULL;

    int matrix[position_values->values_found->size_values_found][my_data->size_my_data_array];
    int sizes[position_values->values_found->size_values_found];

    int i, j, k=0;
    for(i=0; i < position_values->values_found->size_values_found; i++) {
        k=0;
        for(j=0; j < my_data->size_my_data_array; j++) {
            if(data->atributes[my_data->my_data_array[j]][position_values->atribute].value == position_values->values_found->values[i]) {
                matrix[i][k] = my_data->my_data_array[j];
                k++;
            }
        }
        sizes[i] = k;
    }

    position_values->size_positions = position_values->values_found->size_values_found;

    // MEMORY ALLOCATION
    position_values->positions = (struct my_data *)calloc(position_values->values_found->size_values_found, sizeof(struct my_data));
    assert(position_values->positions);

    for(i=0; i < position_values->values_found->size_values_found; i++) {
        position_values->positions[i].my_data_array = (int *)calloc(sizes[i], sizeof(int));
        assert(position_values->positions[i].my_data_array);
    }

    for(i=0; i < position_values->size_positions; i++) {
        for(j=0; j < sizes[i]; j++) {
            position_values->positions[i].size_my_data_array = sizes[i];
            position_values->positions[i].my_data_array[j] = matrix[i][j];
        }
    }
    return position_values;
}

// RECEIVE:
//      the database, struct my_data that has an array with tuples of the data
//      and her size
// RETURN:
//      if ok then
//          an double between 0 and 1 correspondent to the entropy
//      else
//          -1
// PARAMETERS:
//      data = database
//      my_data = an struct with the indices of my data and her size
// PRECAUTIONS
//      uses library math.h
double entropy(struct database *data, struct my_data *my_data) {
    assert(data);
    assert(data->atributes);
    assert(data->class_values);
    assert(my_data);
    assert(my_data->my_data_array);
    if(my_data->size_my_data_array > data->size_database) return -1;
    if(my_data->size_my_data_array < 0) return -1;
    if(data->size_class_values < 1) return -1;

    int quantity[data->size_class_values];
    int i, j;
    for(i=0; i < data->size_class_values; i++) {
        quantity[i] = 0;
        for(j=0; j < my_data->size_my_data_array; j++) {
            if(data->class_values[i] == data->atributes[my_data->my_data_array[j]][data->quantity_atributes-1].value) {
                quantity[i] += 1;
            }
        }
    }

    double entropy=0;
    for(i=0; i < data->size_class_values; i++) {
        if(quantity[i] == 0) continue; // to evite -nan results
        entropy += -((double)quantity[i])/((double) my_data->size_my_data_array) * log10(((double)quantity[i])/((double) my_data->size_my_data_array)) / log10(2);
    }

    return entropy;
}

// RECEIVE:
//      the entropy of her parent, the database
//      struct my_data that has an array with tuples of the data
//      and her size
// RETURN:
//      if ok then
//          an double between 0 and 1 correspondent to the information gain
//      else
//          -1
// PARAMETERS:
//      entropy_parent = entropy of her parent
//      data = database
//      my_data = an struct with the indices of my data and her size
//      atribute = atribute to calculate the information gain
// PRECAUTIONS
//      uses functions: atribute_values, position_values and entropy
double information_gain(double entropy_parent, struct database *data, struct my_data *my_data, int atribute) {
    assert(data);
    assert(data->atributes);
    assert(data->class_values);
    assert(my_data);
    assert(my_data->my_data_array);
    if(my_data->size_my_data_array > data->size_database) return -1;
    if(my_data->size_my_data_array < 0) return -1;
    if(data->size_class_values < 1) return -1;
    if(atribute > data->quantity_atributes -2 || atribute < 0) return -1;
    if(entropy_parent < 0 || entropy_parent > 1) return -1;


    struct values_found *values_found = atribute_values(data, my_data, atribute);
    assert(values_found);

    struct position_values *position_value = (struct position_values *)calloc(1, sizeof(struct position_values));
    assert(position_value);

    position_value->values_found = values_found;
    position_value->atribute = atribute;

    position_values(data, my_data, position_value);

    struct my_data *sub_my_data = (struct my_data*)calloc(1, sizeof(struct my_data));
    assert(sub_my_data);

    double entropy_children[position_value->size_positions];
    int i;
    for(i=0; i < position_value->size_positions; i++) {
        sub_my_data->my_data_array = position_value->positions[i].my_data_array;
        sub_my_data->size_my_data_array = position_value->positions[i].size_my_data_array;
        entropy_children[i] = entropy(data, sub_my_data);
    }

    double sum=0;
    for(i=0; i < position_value->size_positions; i++) {
        sum += ((double) position_value->positions[i].size_my_data_array / (double) my_data->size_my_data_array) * entropy_children[i];
    }

    for(i=0; i < position_value->size_positions; i++) {
        assert(position_value->positions[i].my_data_array);
        free(position_value->positions[i].my_data_array);
    }

    free(sub_my_data);

    assert(position_value->positions);
    free(position_value->positions);
    free(position_value);

    free(values_found->values);
    free(values_found);

    return (entropy_parent - sum);
}
