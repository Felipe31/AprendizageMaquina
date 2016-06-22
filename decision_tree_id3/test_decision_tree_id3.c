#include "database.h"
#include "functions_decision_tree_id3.h"
#include "decision_tree_id3.h"
#include "list_insertion_sort.h"

void TEST_create_node() {
    char file[] = "../jogar_tenis_normalizada.in";
    int size_database=14, quantity_atributes=5;
    int size_class_values = 2;
    int node_atribute = 0;
    double node_value = 1.0;
    double node_class = 1.0;
    double node_entropy = 0.5;
    double node_information_gain = 0.34;

    struct database *data = mount_database(file, size_database, quantity_atributes);
    assert(data);
    data->class_values = (double *)calloc(size_class_values, sizeof(double));
    assert(data->class_values);
    data->size_class_values = size_class_values;
    data->class_values[0] = 1.0;
    data->class_values[1] = 0.0;

    struct my_data *node_data = (struct my_data *)calloc(1, sizeof(struct my_data));
    assert(node_data);

    node_data->my_data_array = (int *)calloc(size_database, sizeof(int));
    assert(node_data->my_data_array);
    node_data->size_my_data_array = size_database;

    int i;
    for(i=0; i < size_database; i++) {
        node_data->my_data_array[i] = i;
    }

    struct node *node = create_node(data, node_data, node_atribute, node_value, node_class, node_entropy, node_information_gain);

    printf("%p, %p, %d, %f, %f, %f, %f, %p, %p\n", node->data, node->node_data, node->node_atribute, node->node_value, node->node_class, node->node_entropy, node->node_information_gain, node->next, node->children);

    free_database(data);
    free(node_data->my_data_array);
    free(node_data);
    free(node);

    printf("ok TEST_create_node\n\n");
}

void TEST_create_root() {
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

    struct node *root = create_root(data);
    assert(root);

    printf("entropy:%f, information_gain:%f, atribute:%d\n", root->node_entropy, root->node_information_gain, root->node_atribute);

    int i;
    printf("status: 1 = used, 0 = not used\n");
    for(i=0; i < data->quantity_atributes; i++) {
        printf("status:%c\n", data->ids[i].status);
    }

    free_database(data);
    free(root->node_data->my_data_array);
    free(root->node_data);
    free(root);

    printf("ok TEST_create_root\n\n");
}

void TEST_create_children() {
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

    struct node *root = create_root(data);
    assert(root);
    assert(root->node_data);
    assert(root->data);
    assert(root->node_data->my_data_array);

    struct node_list *_list = NULL;

    create_children(root, &_list);

    int i;
    printf("parent\n");
    for(i=0; i < root->node_data->size_my_data_array; i++) {
        printf("%d, ", root->node_data->my_data_array[i]);
    }
    printf("\n");

    struct node *list = root->children;
    printf("children\n");
    while(list) {
        for(i=0; i < list->node_data->size_my_data_array; i++) {
            printf("%d, ", list->node_data->my_data_array[i]);
        }
        printf("\n");
        list = list->next;
    }

    struct node_list *aux = _list;
    struct node_list *aux2 = _list;
    while(aux) {
        aux2 = aux2->next;
        printf("entropy:%f, gain:%f\n", aux->node->node_entropy, aux->node->node_information_gain);
        free(aux);
        aux = aux2;
    }

    for(i=0; i < data->quantity_atributes; i++) {
        printf("status:%c\n", data->ids[i].status);
    }

    free_database(data);
    free_list(root->children);
    free_list(root);

    printf("ok TEST_create_children\n\n");
}

void TEST_create_tree() {
    char file[] = "../treinamento.in", file_test[] = "../teste.in";
    int size_database=290, size_database_test=145, quantity_atributes=17;
    int size_class_values = 2;

    struct database *data = mount_database(file, size_database, quantity_atributes);
    assert(data);
    data->class_values = (double *)calloc(size_class_values, sizeof(double));
    assert(data->class_values);
    data->size_class_values = size_class_values;
    data->class_values[0] = 1.0;
    data->class_values[1] = 0.0;

    struct node *root = create_tree(data);

    //print_tree(root);
    //print_atributes_name(data);

    struct database *test = mount_database(file_test, size_database_test, quantity_atributes);
    assert(test);
    test->class_values = (double *)calloc(size_class_values, sizeof(double));
    assert(test->class_values);
    test->size_class_values = size_class_values;
    test->class_values[0] = 1.0;
    test->class_values[1] = 0.0;

    double out[size_database_test], array[quantity_atributes-1];
    int i, j;
    for(i=0; i < size_database_test; i++) {
        for(j=0; j < quantity_atributes - 1; j++) {
            array[j] = test->atributes[i][j].value;
        }
        out[i] = search(root, array);
        //printf("%.1f\n", out[i]);
    }

    int confusion_matrix[2][2] = {{0, 0},
                                  {0, 0}};

    for(i=0; i < size_database_test; i++) {
        if(out[i] >= 0.5) {
            if(test->atributes[i][test->quantity_atributes-1].value >= 0.5) confusion_matrix[0][0] += 1;
            else confusion_matrix[1][0] += 1;
        } else {
            if(test->atributes[i][test->quantity_atributes-1].value <= 0.5) confusion_matrix[1][1] += 1;
            else confusion_matrix[0][1] += 1;
        }
    }

    printf("TP:%d, FN:%d\nFP:%d, TN:%d\n",
        confusion_matrix[0][0], confusion_matrix[0][1],
        confusion_matrix[1][0], confusion_matrix[1][1]);

    double k = (double)(confusion_matrix[0][0] + confusion_matrix[1][1]);
    double l = (double)(confusion_matrix[0][0] + confusion_matrix[0][1] +
                confusion_matrix[1][0] + confusion_matrix[1][1]);
    printf("accuracy: %f\n", (double)(k/l));

    free_tree(root);
    free_database(data);
    free_database(test);
}

int main(void) {

    // TEST_create_node();
    //
    // TEST_create_root();
    //
    // TEST_create_children();

    TEST_create_tree();

    return 0;
}
