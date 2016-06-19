#include "database.h"

int main(void) {

    char file[] = "../jogar_tenis_normalizada.in";
    int size_database=14, quantity_atributes=5;
    int size_class_values = 2;
    double *class_values = (double *)calloc(size_class_values, sizeof(double));
    assert(class_values);
    class_values[0] = 1.0;
    class_values[1] = 0.0;

    struct database *data = mount_database(file, size_database, quantity_atributes);
    assert(data);
    data->class_values = class_values;
    data->size_class_values = size_class_values;

    print_values_database(data);
    print_atributes_name(data);
    print_all_values_database(data);
    print_values_class(data);

    free_database(data);
    printf("ok\n");

    return 0;
}
