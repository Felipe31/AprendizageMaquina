#ifndef _KNN
#define _KNN

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include "database.h"

struct distance {
    float distance;
    int indice;
};

struct distances {
    struct distance *distance;
    int size_distances;
};

struct distances *create_distances(int size_distances);

// calculate the distance between two tuples
double euclidean_distance(struct database *data, int x, int y);


void distance(struct database *base, struct distances *distances, int x);

void free_distances(struct distances *dist);

double choose_class(struct database *data, struct distances *dist, int k);

void set_confusion_matrix(int **confusion_matrix, double answer_true, double answer_false);

void knn(struct database *data, int k, int **confusion_matrix);

int **create_confusion_matrix();

void free_confusion_matrix(int **confusion_matrix);

#endif
