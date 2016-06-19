
#include "knn.h"

// definition of the confusion matrix
//int confusion_matrix[2][2] = {{0, 0}, {0, 0}};

struct distances *create_distances(int size_distances) {
    if(size_distances < 1) return NULL;

    struct distances *array = (struct distances*)calloc(1, sizeof(struct distances));
    assert(array);

    array->distance = (struct distance*)calloc(size_distances, sizeof(struct distance));
    assert(array->distance);

    array->size_distances = size_distances;

    return array;
}

// calculate the distance between two tuples
// x and y are indices of the database
double euclidean_distance(struct database *data, int x, int y) {
    assert(data);
    assert(data->atributes);

    double sum=0;
    int i;
    for(i=0; i < data->quantity_atributes-2; i++) {
        sum += (data->atributes[x][i].value - data->atributes[y][i].value) * (data->atributes[x][i].value - data->atributes[y][i].value);
    }

    return (double) sqrt(sum);
}

// function used by the qsort on function distance
static int cmp_func_dist(const void *a, const void *b) {
    struct distance *va = (struct distance*) a;
    struct distance *vb = (struct distance*) b;
    return (va->distance > vb->distance) - (va->distance < vb->distance);
}

// calculate all the distances among x and the rest of the base and
// sort all the distances on the struct distances
// data = database
// distances = array for armazenated the distances
// x = indice of a tuple in data
void distance(struct database *data, struct distances *distances, int x) {
    assert(data);
    assert(data->atributes);
    assert(distances);
    assert(distances->distance);
    if(x < 0 || x > data->size_database) return;

    int i;
    for(i=0; i < data->size_database; i++) {
        if(i == x) {
            distances->distance[i].distance = LONG_MAX;
            distances->distance[i].indice = i;
            continue;
        }
        distances->distance[i].distance = euclidean_distance(data, x, i);
        distances->distance[i].indice = i;
    }
    qsort(distances->distance, distances->size_distances, sizeof(struct distance), cmp_func_dist);
}

// dist is sorted
// k = number of tuples to compare
// function used by function set_class
double choose_class(struct database *data, struct distances *dist, int k) {
    assert(data);
    assert(data->atributes);
    assert(dist);
    assert(dist->distance);

    int aux[k], i, j;
    for(i=0; i < k; i++) {
        aux[i] = 0;
        for(j=0; j < k; j++) {
            if(data->atributes[dist->distance[i].indice][data->quantity_atributes-1].value == data->atributes[dist->distance[j].indice][data->quantity_atributes-1].value) {
                aux[i] += 1;
            }
        }
    }

    int bigger=-1, indice=-1;
    for(i=0; i < k; i++) {
        if(aux[i] > bigger) {
            bigger = aux[i];
            indice = i;
        }
    }

    return data->atributes[dist->distance[indice].indice][data->quantity_atributes-1].value;
}

void set_confusion_matrix(int **confusion_matrix, double answer_true, double answer_false) {
    if(answer_true == 1.0) {
        if(answer_true == answer_false) confusion_matrix[0][0] += 1;
        else confusion_matrix[0][1] += 1;
    } else {
        if(answer_true == answer_false) confusion_matrix[1][1] += 1;
        else confusion_matrix[1][0] += 1;
    }
}

void knn(struct database *data, int k, int **confusion_matrix) {
    assert(data);
    assert(data->atributes);
    assert(confusion_matrix);
    if(k < 1) return;

    struct distances *distances = create_distances(data->size_database);
    assert(distances);
    assert(distances->distance);

    double answer[data->size_database];

    int i;
    for(i=0; i < data->size_database; i++) {
        distance(data, distances, i);
        answer[i] = choose_class(data, distances, k);
        set_confusion_matrix(confusion_matrix, data->atributes[i][data->quantity_atributes-1].value, answer[i]);
    }

    free_distances(distances);
}

int **create_confusion_matrix() {
    int **l = (int **)calloc(2, sizeof(int *));
    assert(l);

    l[0] = (int *)calloc(2, sizeof(int));
    assert(l[0]);
    l[1] = (int *)calloc(2, sizeof(int));
    assert(l[1]);

    return l;
}

void free_confusion_matrix(int **confusion_matrix) {
    assert(confusion_matrix);

    free(confusion_matrix[0]);
    free(confusion_matrix[1]);
    free(confusion_matrix);
}

void free_distances(struct distances *dist) {
    assert(dist);
    assert(dist->distance);

    free(dist->distance);
    free(dist);
}
