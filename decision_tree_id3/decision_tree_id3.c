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

// calculate the sum of a determined class received
// class = int with the number of the chosen class
double entropy_sum(struct database *data, int *array, int size_array, int class) {
    assert(data);
    assert(data->atributes);
    assert(array);
    if(size_array < 1) return -1;

    int atributes=1, i, j=0;
    double aux[size_array]; // local with the values of the atributes
    for(i=0; i < size_array; i++) {
        aux[i] = -1;
    }
    // find the number of atributes of the given class
    aux[0] = data->atributes[array[0]][class].value;
    for(i=1; i < size_array; i++) {
        check_value_exist(&aux, &atributes, data->atributes[array[i]][class].value);
    }

    // calculate the quantity of each atribute
    int j, atrib[*atributes]; // array with the sum of each atribute
    for(i=0; i < *atributes; i++) {
        atrib[i] = 0;
        for(j = 0; j < size_array; j++) {
            if(data->atributes[array[j]][class].value == aux[i]) {
                atrib[i] += 1;
            }
        }
    }

    double sum=0;

    // make the sum
    // calculate the number of possibilities of the answer class
    // and then make the sum of the entropy
    // double sum=0;
    // for(i=0; i < *atributes; i++) {
    //     sum +=
    // }

}
/*
// data = all the database
// array = array 0f integers with the indices of the selected tuples from data
// size = size of the array
double entropy(struct database *data, int *array, int size) {
    assert(data);
    assert(data->tuples);
    assert(array);
    if(size < 1) return 0;



    int i;
    for(i=0; i < size; i++) {

    }

    return 0;
}

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

    struct database *base = mount_database(file, size_database, size_classes);
    assert(base);

    free_database(base);
    printf("ok\n");

    return 0;
}
