
#include "knn.h"

struct distances *create_distances(int size) {
    if(size < 1) return NULL;

    struct distances *array = (struct distances*)malloc(sizeof(struct distances));
    if(array == NULL) return NULL;

    array->distance = (struct distance*)malloc(size*sizeof(struct distance));
    if(array->distance == NULL) return NULL;

    array->size = size;

    return array;
}

struct database *create_database(int size) {
    if (size < 1) return NULL;

    struct database *base = (struct database*)calloc(1, sizeof(struct database));
    assert(base);

    base->tuples = (struct day*)calloc(size, sizeof(struct day));
    assert(base->tuples);

    base->size = size;

    return base;
}

struct database *mount_database(const char *file_name, int size) {
    assert(file_name);
    if(size < 1) return NULL;

    FILE *fp = NULL;
    errno = 0; // variable for error on fopen
    fp = fopen(file_name, "r");
    if(fp == NULL) {
        printf("Errno: %d\n", errno);
        return NULL;
    }

    struct database *base = create_database(size);
    assert(base);

    float i, j, k, l;
    int m, cont=0;
    while(fscanf(fp, "%f,%f,%f,%f,%d\n", &i, &j, &k, &l, &m) != EOF) {
        base->tuples[cont].outlook = i;
        base->tuples[cont].temperature = j;
        base->tuples[cont].humidity = k;
        base->tuples[cont].wind = l;
        base->tuples[cont].play_tennis = m;
        cont++;
    }

    fclose(fp);

    return base;
}

// calculate the distance between two tuples
float euclidean_distance(struct day *x, struct day *y) {
    assert(x);
    assert(y);

    float out = (x->outlook - y->outlook)*(x->outlook - y->outlook);
    float temp = (x->temperature - y->temperature)*(x->temperature - y->temperature);
    float humi = (x->humidity - y->humidity)*(x->humidity - y->humidity);
    float wind = (x->wind - y->wind)*(x->wind - y->wind);

    return sqrt(out + temp + humi + wind);
}

// function used by the qsort on function distance
static int cmp_func_dist(const void *a, const void *b) {
    struct distance *va = (struct distance*) a;
    struct distance *vb = (struct distance*) b;
    return (va->distance > vb->distance) - (va->distance < vb->distance);
}

// calculate all the distances among x and the rest of the base and
// sort all the distances on the struct distances
// base = database
// distances = array for armazenated the distances
// x = indice of a tuple in base
void distance(struct database *base, struct distances *distances, int x) {
    assert(base);
    assert(base->tuples);
    assert(distances);
    assert(distances->distance);
    if(x < 0) return;

    int i;
    for(i=0; i < base->size; i++) {
        if(i == x) {
            distances->distance[i].distance = LONG_MAX;
            distances->distance[i].indice = i;
            continue;
        }
        distances->distance[i].distance = euclidean_distance(&(base->tuples[i]), &(base->tuples[x]));
        distances->distance[i].indice = i;
    }
    qsort(distances->distance, distances->size, sizeof(struct distance), cmp_func_dist);
}

// dist is sorted
// k = number of tuples to compare
// function used by function set_class
int choose_class(struct database *base, struct distances *dist, int k) {
    assert(base);
    assert(base->tuples);
    assert(dist);
    assert(dist->distance);

    int aux[k], i;

    for(i=0; i < k; i++) {
        aux[i] = base->tuples[dist->distance[i].indice].play_tennis;
    }

    int sim=0, nao=0;
    for(i=1; i < k; i++) {
        if(aux[i] == 1) sim++;
        else nao++;
    }

    return (sim > nao);
}

void set_confusion_matrix(int answer_true, int answer_false) {
    if(answer_true == 1) {
        if(answer_true == answer_false) confusion_matrix[0][0] += 1;
        else confusion_matrix[0][1] += 1;
    } else {
        if(answer_true == answer_false) confusion_matrix[1][1] += 1;
        else confusion_matrix[1][0] += 1;
    }
}

// k = number of classes to compare
void set_class(struct database *base, struct distances *dist, int k) {
    assert(base);
    assert(base->tuples);
    assert(dist);
    assert(dist->distance);

    int i;
    for(i=0; i < base->size; i++) {
        distance(base, dist, i);
        base->tuples[i].answer = choose_class(base, dist, k);
        set_confusion_matrix(base->tuples[i].play_tennis, base->tuples[i].answer);
    }
}

void knn(struct database *base, int k) {
    assert(base);
    assert(base->tuples);
    if(k < 1) return;

    struct distances *distances = create_distances(base->size);
    assert(distances);
    assert(distances->distance);

    set_class(base, distances, k);

    free_distances(distances);
}

void print_database(struct database *base) {
    assert(base);
    assert(base->tuples);

    int cont=0;
    printf("day, outlook, temperature, humidity, wind, play_tennis | answer\n");
    while(cont < base->size) {
        printf("%d,   %.2f,    %.2f,        %.2f,     %.2f, %d,          | %d\n",
        cont, base->tuples[cont].outlook, base->tuples[cont].temperature,
        base->tuples[cont].humidity, base->tuples[cont].wind,
        base->tuples[cont].play_tennis, base->tuples[cont].answer);
        cont++;
    }
}

void free_database(struct database *base) {
    assert(base);
    assert(base->tuples);

    free(base->tuples);
    free(base);
}

void free_distances(struct distances *dist) {
    assert(dist);
    assert(dist->distance);

    free(dist->distance);
    free(dist);
}

int main(void) {

    char file[] = "../jogar_tenis_normalizada.in";
    int size=14;
    int k=5;

    struct database *base = mount_database(file, size);
    assert(base);

    print_database(base);

    knn(base, k);

    printf("\n");
    print_database(base);

    printf("\nTP:%d, FN:%d\nFP:%d, TN:%d\n",
        confusion_matrix[0][0], confusion_matrix[0][1],
        confusion_matrix[1][0], confusion_matrix[1][1]);

    double i = (double)(confusion_matrix[0][0] + confusion_matrix[1][1]);
    double j = (double)(confusion_matrix[0][0] + confusion_matrix[0][1] +
                confusion_matrix[1][0] + confusion_matrix[1][1]);
    printf("k:%d\naccuracy: %f\n", k, (double)(i/j));

    free_database(base);
    printf("ok\n");

    return 0;
}
