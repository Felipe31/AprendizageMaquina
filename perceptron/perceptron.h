#ifndef _PERCEPTRON
#define _PERCEPTRON

#include "database.h"

void printArray(double *array, int i);
void printMatrix(double** matrix, int i, int j);
int activation(double u);
int * instancia(double *x, int i, double *w, int *bias);
int perceptron (struct database * data, int age);

#endif
