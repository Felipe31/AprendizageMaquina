
#include "database.h"
#include "functions_decision_tree_id3.h"

void TEST_check_value_exist() {
    int size_atributes=2;
    double atributes[4] = {0.0, 0.5};

    printf("return:%d, size_atributes:%d\n", check_value_exist(atributes, &size_atributes, 0.5), size_atributes);
    printf("return:%d, ", check_value_exist(atributes, &size_atributes, 1.0));
    printf("size_atributes:%d\n", size_atributes);

    printf("ok TEST_check_value_exist\n\n");
}

void TEST_atribute_values() {
    char file[] = "../jogar_tenis_normalizada.in";
    int size_database=14, quantity_atributes=5;
    int atribute=0;

    struct database *data = mount_database(file, size_database, quantity_atributes);
    assert(data);

    struct my_data *my_data = (struct my_data *)calloc(1, sizeof(struct my_data));
    assert(my_data);

    my_data->my_data_array = (int *)calloc(size_database, sizeof(int));
    assert(my_data->my_data_array);
    my_data->size_my_data_array = size_database;

    int i;
    for(i=0; i < size_database; i++) {
        my_data->my_data_array[i] = i;
    }

    struct values_found *values_found = atribute_values(data, my_data, atribute);
    assert(values_found);

    for(i=0; i < values_found->size_values_found; i++) {
        printf("%f, ", values_found->values[i]);
    }
    printf("size_values_found:%d\n", values_found->size_values_found);

    free_database(data);
    free(my_data->my_data_array);
    free(my_data);
    free(values_found->values);
    free(values_found);

    printf("ok TEST_atribute_values\n\n");
}

void TEST_position_values() {
    char file[] = "../jogar_tenis_normalizada.in";
    int size_database=14, quantity_atributes=5;
    int atribute=0;

    struct database *data = mount_database(file, size_database, quantity_atributes);
    assert(data);

    struct my_data *my_data = (struct my_data *)calloc(1, sizeof(struct my_data));
    assert(my_data);

    my_data->my_data_array = (int *)calloc(size_database, sizeof(int));
    assert(my_data->my_data_array);
    my_data->size_my_data_array = size_database;

    int i;
    for(i=0; i < size_database; i++) {
        my_data->my_data_array[i] = i;
    }

    struct values_found *values_found = atribute_values(data, my_data, atribute);
    assert(values_found);

    struct position_values *position_value = (struct position_values *)calloc(1, sizeof(struct position_values));
    assert(position_value);

    position_value->values_found = values_found;
    position_value->atribute = atribute;

    position_values(data, my_data, position_value);

    int j;
    for(i=0; i < position_value->size_positions; i++) {
        for(j=0; j < position_value->positions[i].size_my_data_array; j++) {
            printf("%d, ", position_value->positions[i].my_data_array[j]);
        }
        printf("\n");
    }

    free_database(data);
    free(my_data->my_data_array);
    free(my_data);
    free(values_found->values);
    free(values_found);

    for(i=0; i < position_value->size_positions; i++) {
        assert(position_value->positions[i].my_data_array);
        free(position_value->positions[i].my_data_array);
    }
    assert(position_value->positions);
    free(position_value->positions);
    free(position_value);

    printf("ok TEST_position_values\n\n");
}

void TEST_entropy() {
    char file[] = "../jogar_tenis_normalizada.in";
    int size_database=14, quantity_atributes=5;
    int size_class_values = 2;

    struct database *data = mount_database(file, size_database, quantity_atributes);
    assert(data);
    data->class_values = (double *)calloc(size_class_values, sizeof(double));
    assert(data->class_values);
    data->size_class_values = size_class_values;
    data->class_values[0] = 1.0;
    data->class_values[1] = 0.0;

    struct my_data *my_data = (struct my_data *)calloc(1, sizeof(struct my_data));
    assert(my_data);

    my_data->my_data_array = (int *)calloc(size_database, sizeof(int));
    assert(my_data->my_data_array);
    my_data->size_my_data_array = size_database;

    int i;
    for(i=0; i < size_database; i++) {
        my_data->my_data_array[i] = i;
    }

    double ent = entropy(data, my_data);

    free_database(data);
    free(my_data->my_data_array);
    free(my_data);

    printf("entropy:%f\n", ent);
    printf("ok TEST_entropy\n\n");
}

void TEST_information_gain() {
    char file[] = "../jogar_tenis_normalizada.in";
    int size_database=14, quantity_atributes=5;
    int atribute=0;
    int size_class_values = 2;

    struct database *data = mount_database(file, size_database, quantity_atributes);
    assert(data);

    data->class_values = (double *)calloc(size_class_values, sizeof(double));
    assert(data->class_values);
    data->size_class_values = size_class_values;
    data->class_values[0] = 1.0;
    data->class_values[1] = 0.0;

    struct my_data *my_data = (struct my_data *)calloc(1, sizeof(struct my_data));
    assert(my_data);

    my_data->my_data_array = (int *)calloc(size_database, sizeof(int));
    assert(my_data->my_data_array);
    my_data->size_my_data_array = size_database;

    int i;
    for(i=0; i < size_database; i++) {
        my_data->my_data_array[i] = i;
    }

    double entropy_parent = entropy(data, my_data);

    double gain = information_gain(entropy_parent, data, my_data, atribute);

    free_database(data);
    free(my_data->my_data_array);
    free(my_data);

    printf("gain: %f\n", gain);
}

int main(void) {

    TEST_check_value_exist();

    TEST_atribute_values();

    TEST_position_values();

    TEST_entropy();

    TEST_information_gain();

    printf("ok main_test\n");

    return 0;
}
