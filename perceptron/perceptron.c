#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "database.h"

void printArray(double *array, int i) {
    int count;
    puts("\n--------------------------------------------");
    for (count = 0; count < i; count++) {
        printf("%.1lf - ", array[count]);
    }
    puts("\n--------------------------------------------");
}


void printMatrix(double** matrix, int i, int j) {
    int count;
    for (count = 0; count < i; count++) {
        printArray(matrix[count], j);
    }

}

int activation(double u) {
    if (u >= 1.0) return 1;
    return 0;
}

// x tem uma posição a mais, pois a última armazena o resultado real, que é usado para comparações
int * instancia(struct atribute *x, int i, double *w, int *bias) {
    double u = 0.0;
    int aprend = 1, count, y=-4, erro = 1;
    puts("\n");
    while (erro) {

        for (count = 0; count < i-1; count++)
        {
            u = u+x[count].value * w[count]+(double)*bias;
            printf("w[%d] = %.1f", count, w[count]);
            if(count+1 < i-1)
                printf(" - ");
        }
        puts("");


        y = activation(u);
        erro = x[i-1].value - y;
        printf("u = %.1lf\n", u);
        printf("classe = %.1lf\n", x[i-1].value);
        printf("y = %d\n", y);
        printf("erro = %d\n", erro);
        printf("bias = %d\n\n", *bias);

        if(erro == 0) {
            puts("-----------------------------------------------------------------------------------");
            continue;
        }
        *bias = *bias + aprend * erro;
        for (count = 0; count < i-1; count++)
            w[count] = w[count]+aprend*erro*x[count].value;
    }

    return NULL;
}


int perceptron (struct database * data, int age) {
    srand(time(NULL));
    double *w = (double *)malloc(sizeof(double)*(data->size_database));
    int *bias = (int *)malloc(sizeof(int));
    int count = 0, count2;
    while(count < data->size_database) {
        w[count] = rand()%11 /10.0 ;
        count++;
    }
    *bias = 1;
    for(count2 = 0; count2 < age; count2++) {
        puts("\n+-----------------------+");
        printf("|Inicio da epoca #%d\t|\n", age );
        puts("+-----------------------+");
        for(count = 0; count < data->size_database; count++)
        {
            printf("Tupla #%d\n", count );
            instancia(data->atributes[count], data->quantity_atributes, w, bias);
        }
    }

    return 0;
}
