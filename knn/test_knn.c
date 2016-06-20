#include "knn.h"

void TEST_database() {
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

    print_atributes_name(data);
    print_all_values_database(data);

    free_database(data);
}

void TEST_euclidean_distance() {
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

    euclidean_distance(data, 1, 2);

    free_database(data);
}

void TEST_distance() {
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

    struct distances *dist = create_distances(data->size_database);
    assert(dist);

    distance(data, dist, 1);

    int i;
    for(i=0; i < dist->size_distances; i++) {
        printf("dist:%f, %d\n", dist->distance[i].distance, dist->distance[i].indice);
    }

    free_database(data);
    free_distances(dist);
}

void TEST_choose_class() {
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

    struct distances *dist = create_distances(data->size_database);
    assert(dist);

    distance(data, dist, 1);

    double class = choose_class(data, dist, 3);

    printf("k=3, class:%f\n", class);

    free_database(data);
    free_distances(dist);
}

void TEST_knn() {
    char file[] = "../test_weka.in";
    int size_database=435, quantity_atributes=17;
    int size_class_values = 2;

    struct database *data = mount_database(file, size_database, quantity_atributes);
    assert(data);
    data->class_values = (double *)calloc(size_class_values, sizeof(double));
    assert(data->class_values);
    data->size_class_values = size_class_values;
    data->class_values[0] = 1.0;
    data->class_values[1] = 0.0;

    int **confusion_matrix = create_confusion_matrix();

    int k=45;
    knn(data, k, confusion_matrix);

    printf("\nTP:%d, FN:%d\nFP:%d, TN:%d\n",
        confusion_matrix[0][0], confusion_matrix[0][1],
        confusion_matrix[1][0], confusion_matrix[1][1]);

    double i = (double)(confusion_matrix[0][0] + confusion_matrix[1][1]);
    double j = (double)(confusion_matrix[0][0] + confusion_matrix[0][1] +
                confusion_matrix[1][0] + confusion_matrix[1][1]);
    printf("k:%d\naccuracy: %f\n", k, (double)(i/j));

    free_database(data);
    free_confusion_matrix(confusion_matrix);
}


int main(void) {

    // TEST_database();
    //
    // TEST_euclidean_distance();
    //
    // TEST_distance();
    //
    // TEST_choose_class();

    TEST_knn();

    return 0;
}
