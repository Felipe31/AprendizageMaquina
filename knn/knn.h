#ifndef _KNN
#define _KNN

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <errno.h>
#include <limits.h>

// definition of the confusion matrix
int confusion_matrix[2][2] = {{0, 0}, {0, 0}};

// struct for normalized tuple
struct day {
    float outlook;
    float temperature;
    float humidity;
    float wind;
    int play_tennis;
    int answer;
};

struct database {
    struct day *tuples;
    int size;
};

struct distance {
    float distance;
    int indice;
};

struct distances {
    struct distance *distance;
    int size;
};

struct database *create_database(int size);

// mount the database with the file_name
struct database *mount_database(const char *file_name, int size);

// calculate the distance between two tuples
float euclidean_distance(struct day *x, struct day *y);


void distance(struct database *base, struct distances *distances, int x);

// dist is sorted
// k = number of tuples to compare
int choose_class(struct database *base, struct distances *dist, int k);

// k = number of classes to compare
void set_class(struct database *base, struct distances *dist, int k);

void knn(struct database *base, int k);

void print_database(struct database *base);

void free_database(struct database *base);

void free_distances(struct distances *dist);


#endif
