/*
Author: Luis Angelo Loss de Castro
*/

#include "decision_tree_id3.h"

struct database *create_database(int size_database, int size_classes) {
    if (size_database < 1 || size_classes < 2) return NULL;

    struct database *base = (struct database*)calloc(1, sizeof(struct database));
    assert(base);

    base->classes = (struct class*)calloc(size_classes, sizeof(struct class));
    assert(base->classes);
    base->size_classes = size_classes;

    base->atributes = (struct atribute**)calloc(size_database, sizeof(struct atribute*));
    assert(base->atributes);
    base->size_database = size_database;

    int i;
    for(i=0; i < size_database; i++) {
        base->atributes[i] = (struct atribute*)calloc(size_classes, sizeof(struct atribute));
        assert(base->atributes[i]);
    }

    return base;
}

struct database *mount_database(const char *file_name, int size_database, int size_classes) {
    assert(file_name);
    if(size_database < 1 || size_classes < 2) return NULL;

    FILE *fp = NULL;
    errno = 0; // variable for error on fopen
    fp = fopen(file_name, "r");
    if(fp == NULL) {
        printf("Errno: %d\n", errno);
        return NULL;
    }

    struct database *base = create_database(size_database, size_classes);
    assert(base);

    char name[100];

    int cont, j;
    for(cont=0; cont < size_classes; cont++) {
        fscanf(fp, "%s", name);
        j = strlen(name);
        base->classes[cont].name = (char *)calloc(j, sizeof(char)); // XXX check memory leak
        strcpy(base->classes[cont].name, name);
        base->classes[cont].status = '0';
    }

    double i;
    int atrib = cont; // number of atributes
    cont=0;
    while(cont < size_database) {
        for(j=0; j < atrib; j++) {
            fscanf(fp, "%lf,", &i);
            base->atributes[cont][j].value = i;
        }
        cont++;
    }

    fclose(fp);

    return base;
}
/*
struct node *create_node(int class, int class_status, int result) {
    if(result < 0) return NULL;

    struct node *node = (struct node *)calloc(1, sizeof(node));
    assert(node);

    node->class = class;
    node->class_status = class_status;
    node->result = result;
    node->next = NULL;
    node->children = NULL;

    return node;
}
*/
// atributes_found = quantity of atributes found
// atributes = array with the atributes already found
// new_value = new value to search in the array atributes
// XXX the value of the atributes has to be only positives
// return a boolean if the value exists
// if not exists, add the new atribute
int check_value_exist(double *atributes, int *atributes_found, double new_value) {
    assert(atributes);
    if(*atributes_found < 0) return -1;

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

static void quantity_atributes(struct database *data, int *array, int size_array, int class, double *aux_local, int *atrib_local, int atributes) {
    assert(data);
    assert(data->atributes);
    assert(array);
    assert(aux_local); // local with the values of the atributes
    assert(atrib_local); // array with the sum of each atribute
    if(size_array < 1 || class > data->size_classes) return;
    if(size_array > data->size_database || class < 0) return;

    // calculate the quantity of each atribute
    int j, i;
    for(i=0; i < atributes; i++) {
        atrib_local[i] = 0;
        for(j = 0; j < size_array; j++) {
            if(data->atributes[array[j]][class].value == aux_local[i]) {
                atrib_local[i] += 1;
            }
        }
    }
}

static void search_atributes(struct database *data, int *array, int size_array, int class, double *aux_local, int *atributes) {
    assert(data);
    assert(data->atributes);
    assert(array);
    assert(atributes);
    assert(aux_local); // local with the values of the atributes
    if(size_array < 1 || class > data->size_classes) return;
    if(size_array > data->size_database || class < 0) return;

    int i;
    for(i=0; i < size_array; i++) {
        aux_local[i] = -1;
    }
    // find the number of atributes of the given class
    aux_local[0] = data->atributes[array[0]][class].value;
    for(i=1; i < size_array; i++) {
        check_value_exist(aux_local, atributes, data->atributes[array[i]][class].value);
    }
}

// calculate the entropy of a determined class received
// data = all the database
// array = array of integers with the indices of the selected tuples from data
// size_array = size of the array
// class = int with the number of the chosen class
// class_values = possible values of the final class(aswer)
// size_class_values = size of the class_values
double entropy(struct database *data, int *array, int size_array, int class, double *class_values, int size_class_values) {
    assert(data);
    assert(data->atributes);
    assert(array);
    assert(class_values);
    if(size_class_values < 1) return -1;
    if(size_array < 1 || class > data->size_classes) return -1;
    if(size_array > data->size_database || class < 0) return -1;

    // aux[] = array with the atributes found
    // atributes = sum of all the quantity of atributes
    double aux[size_array];
    int atributes=1;
    search_atributes(data, array, size_array, class, aux, &atributes);

    // atrib[] = array with the quantity of each atribute, (sequence matches with aux[])
    int atrib[atributes];
    quantity_atributes(data, array, size_array, class, aux, atrib, atributes);

    int i, j;
/*    printf("atributes: %d\n", atributes);
    for(i=0; i < atributes; i++) {
        printf("%d ", atrib[i]);
        printf("%f %d\n", aux[i], size_array);
    }
*/
/*
    quantity_class_values =
                 class i    | class j ...
    atribube k | quantity x | ...
    atribube l | quantity y | ...
        .      | quantity w | ...
        .      | quantity z | ...
*/
    double quantity_class_values[atributes][size_class_values];
    for(i=0; i < atributes; i++) {
        for(j=0; j < size_class_values; j++) {
            quantity_class_values[i][j] = 0.0;
        }
    }

    // function that set quantity_class_values
    int k;
    for(i=0; i < atributes; i++) {
        for(j=0; j < data->size_database; j++) {
            if(j >= size_array || array[j] > data->size_database) break;
            if(data->atributes[array[j]][class].value == aux[i]) {
                for(k=0; k < size_class_values; k++) {
                    if(data->atributes[array[j]][data->size_classes-1].value == class_values[k]) {
                        quantity_class_values[i][k] += 1.0;
                        break;
                    }
                }
            }
        }
    }
/*
    for(i=0; i < atributes; i++) {
        for(j=0; j < size_class_values; j++) {
            printf("%f, ", quantity_class_values[i][j]);
        }
        printf("\n");
    }
*/
    double quantity[size_class_values];
    for(i=0; i < size_class_values; i++) {
        quantity[i] = 0.0;
        for(j=0; j < atributes; j++) {
            quantity[i] += quantity_class_values[j][i];
        }
    }

    double sum=0;
    for(i=0; i < size_class_values; i++) {
        //printf("%f\n", quantity[i]);
        if(quantity[i] == 0) continue; // to evite -nan results
        sum += -(quantity[i]/size_array) * (log10(quantity[i]/size_array) / log10(2));
    }
    //printf("sum:%f\n", sum);
    return sum;
}
/*
void add_list(struct node *list_root, struct node *new) {
    assert(list_root);
    assert(new);

    new->next = list_root->next;
    list_root->next = new;
}

struct node *create_list(struct database *table, int *array, int size) {
    // calcula entropia
    // cria a lista de cada estado possivel
    // retorna a lista
}

void add_children(struct node *node, struct node *new) {

}

void free_list(struct node *root) {
    assert(root);

    struct node *aux = root;
    while(root) {
        aux = aux->next;
        free(root);
        root = aux;
    }
}

void free_tree(struct node *root) {
    if(root == NULL) return;

    struct node *next = root->next;
    struct node *child = root->children;

    free(root);
    free_tree(child);
    free_tree(next);
}
*/
void free_database(struct database *base) {
    assert(base);
    assert(base->classes);
    assert(base->atributes);

    int i;
    for(i=0; i < base->size_classes; i++) {
        assert(base->classes[i].name);
        free(base->classes[i].name);
    }
    free(base->classes);

    for(i=0; i < base->size_database; i++) {
        assert(base->atributes[i]);
        free(base->atributes[i]);
    }

    free(base->atributes);
    free(base);
}

static void mount_sub_array(struct database *data, int *array, int size_array, int class, int *sub_array, int size_sub_array, double value) {
    assert(data);
    assert(data->atributes);
    assert(array);
    assert(sub_array);
    if(size_sub_array < 1) return;
    if(size_array < 1 || class > data->size_classes) return;
    if(size_array > data->size_database || class < 0) return;

    int i, j=0;
    for(i=0; i < size_array; i++) {
        if(data->atributes[array[i]][class].value == value) {
            if(j >= size_sub_array) {
                printf("Error: sub_array value wrong");
                return;
            }
            sub_array[j] = array[i];
            j++;
        }
    }
}

double information_gain(double entropy_parent, struct database *data, int *array, int size_array, int class, double *class_values, int size_class_values) {
    assert(data);
    assert(data->atributes);
    assert(array);
    if(size_array < 1 || class > data->size_classes) return -1;
    if(size_array > data->size_database || class < 0) return -1;

    // aux[] = array with the atributes found
    // atributes = sum of all the quantity of atributes
    double aux[size_array];
    int atributes=1;
    search_atributes(data, array, size_array, class, aux, &atributes);

    // atrib[] = array with the quantity of each atribute, (sequence matches with aux[])
    int atrib[atributes];
    quantity_atributes(data, array, size_array, class, aux, atrib, atributes);


    int i;
    int **sub_array = (int **)calloc(atributes, sizeof(int*));
    assert(sub_array);
    for(i=0; i < atributes; i++) {
        sub_array[i] = (int*)calloc(atrib[i], sizeof(int));
        assert(sub_array[i]);
        mount_sub_array(data, array, size_array, class, sub_array[i], atrib[i], aux[i]);
    }

    // calculate entropy of the children
    double entropy_children[atributes];
    for(i=0; i < atributes; i++) {
        entropy_children[i] = entropy(data, sub_array[i], atrib[i], class, class_values, size_class_values);
    }

    for(i=0; i < atributes; i++) {
        free(sub_array[i]);
    }
    free(sub_array);

    // calculate the average_entropy_children
    double average_entropy_children=0;
    for(i=0; i < atributes; i++) {
        average_entropy_children += (((double) atrib[i]) / ((double) size_array)) * entropy_children[i];
    }

    // return the information gain of the class
    return (entropy_parent - average_entropy_children);
}


/*
// table = a tabela completa, com todos os dados
// array = um vetor com os indices dos seus dados da table
struct node *create_tree(struct database *table, int *array) {

    // calcula entropia de todos
    // a maior entropia vira o no raiz
    // seus filhos serao os seus possiveis estados(ensolarado, nublado, chuvoso, etc..)
    // cria uma lista simples dos filhos do tipo noh
    // faz o cabeca apontar para a lista
    // cada noh da lista tem um vetor com os dados do seu correspondente estado
    // chama o algoritmo recursivamente
    // na volta, chama o algoritmo recursivamente para o proximo da lista
}
*/
int main(void) {

    char file[] = "../jogar_tenis_normalizada.in";
    int size_database=14, size_classes=5;
    int size_array=14;
    int array[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
    //int array[] = {1, 5, 6, 10, 11, 13};
    //int array[] = {0, 2, 3, 4, 7, 8, 9, 12};
    double class_values[] = {1.0, 0.0};
    int size_class_values = 2;

    struct database *data = mount_database(file, size_database, size_classes);
    assert(data);

    double sum = entropy(data, array, size_array, 0, class_values, size_class_values);

    int i;
    for(i=0; i < size_classes - 1; i++) {
        double gain = information_gain(sum, data, array, size_array, i, class_values, size_class_values);

        printf("information gain: %s, %f\n", data->classes[i].name, gain);
    }

    free_database(data);
    printf("ok\n");

    return 0;
}
