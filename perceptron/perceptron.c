#include<stdlib.h>
#include <stdio.h>
#include <time.h>

void printArray(float *array, int i) {
    int count;
    puts("\n--------------------------------------------");
    for (count = 0; count < i; count++) {
        printf("%.1f - ", array[count]);
    }
    puts("\n--------------------------------------------");
}


void printMatriz(float** matriz, int i, int j) {
    int count;
    for (count = 0; count < i; count++) {
        printArray(matriz[count], j);
    }

}

int fAtivacao(float u) {
    if (u >= 7.0) return 1;
    return 0;
}

// x tem uma posição a mais, pois a última armazena o resultado real, que é usado para comparações
int * instancia(float *x, int i, float *w, int *bias) {
    srand(time(NULL));
    float e[i-1], u = 0.0, w2;
    int aprend = 1, count, y=-4, erro = 1, x2;
    puts("\n");
    while (erro) {
        for (count = 0; count < i-1; count++)
            u = u+x[count] * w[count]+(float)*bias;

        y = fAtivacao(u);
        erro = x[i-1] - y;
        printf("u = %.1f\n", u);
        printf("x = %.1f\n", x[i-1]);
        printf("y = %d\n", y);
        printf("erro = %d\n", erro);
        printf("bias = %d\n\n", *bias);
        if(erro == 0) {
            continue;
        }
        *bias = *bias + aprend * erro;
        for (count = 0; count < i-1; count++)
            w[count] = w[count]+aprend*erro*x[count];
    }

    return NULL;
}


int perceptron (float ** x, int i, int j, int age) {
    float *w = (float *)malloc(sizeof(float)*(i-1));
    int *bias = (int *)malloc(sizeof(int));
    int count = 0, count2;
    while(count < i-1) {
        w[count] = rand()%1 + 0.5;
        count++;
    }

    for(count2 = 0; count2 < age; count2++) {
        puts("--------------------------------------------");
        for(count = 0; count < i; count++)
            instancia(x[count], j, w, bias);
    }

    return 0;
}
float ** mount_database(const char *file_name, int size_database, int size_classes) {
    FILE *fp = fopen(file_name, "r");


}

int main () {
    char file[] = "../jogar_tenis_normalizada.in";
    int size_database=14, size_classes=5;
    int size_array=14;

    float **data = mount_database(file, size_database, size_classes);

    float **media = (float**) malloc(sizeof(float*)*4);
    int count;

    for (count = 0; count < 4; count++) {
        media[count] = (float*) malloc(sizeof(float)*3);
    }
    media[0][0]=10.0;
    media[0][1]=4.0;
    media[0][2]=0.0;
    media[1][0]=4.0;
    media[1][1]=10.0;
    media[1][2]=1.0;
    media[2][0]=5.0;
    media[2][1]=5.0;
    media[2][2]=0.0;
    media[3][0]=5.5;
    media[3][1]=8.0;
    media[3][2]=1.0;

    // printMatriz(media, 4, 3);
    // puts("asdasdasdasdas\n\n\n");
    perceptron(media, 4, 3, 3);
    // printMatriz(media, 4, 3);

    return 0;
}
