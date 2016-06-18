#include<stdlib.h>
#include <stdio.h>
#include <time.h>

void printVetor(float *vetor, int i)
{
    int cont;
    puts("\n--------------------------------------------");
    for (cont = 0; cont < i; cont++)
    {
        printf("%.1f - ", vetor[cont]);
    }
    puts("\n--------------------------------------------");
}


void printMatriz(float** matriz, int i, int j)
{
    int cont;
    for (cont = 0; cont < i; cont++)
    {
        printVetor(matriz[cont], j);
    }

}

int fAtivacao(float u)
{
    if (u >= 7.0) return 1;
    return 0;
}

// x tem uma posição a mais, pois a última armazena o resultado real, que é usado para comparações
int * instancia(float *x, int i, float *w, int *bias)
{
    srand(time(NULL));
    float e[i-1], u = 0.0, w2;
    int aprend = 1, cont, y=-4, erro = 1, x2;
    puts("\n");
    while (erro)
    {
        for (cont = 0; cont < i-1; cont++)
            u = u+x[cont] * w[cont]+(float)*bias;

        y = fAtivacao(u);
        erro = x[i-1] - y;
        printf("u = %.1f\n", u);
        printf("x = %d\n", x[i-1]);
        printf("y = %d\n", y);
        printf("erro = %d\n", erro);
        printf("bias = %.1f\n\n", *bias);
        if(erro == 0)
        {
            continue;
        }
        *bias = *bias + aprend * erro;
        for (cont = 0; cont < i-1; cont++)
            w[cont] = w[cont]+aprend*erro*x[cont];
    }

    return NULL;
}


int perceptron (float ** x, int i, int j)
{
    float *w = (float *)malloc(sizeof(float)*(i-1));
    int *bias = (int *)malloc(sizeof(int));
    int cont = 0;
    while(cont < i-1)
    {
        w[cont] = rand()%1 + 0.5;
        cont++;
    }

    for(cont = 0; cont < i; cont++)
        instancia(x[cont], j, w, bias);

    return 0;
}

int main ()
{
    float **media = (float**) malloc(sizeof(float*)*4);
    int cont;

    for (cont = 0; cont < 4; cont++)
    {
        media[cont] = (float*) malloc(sizeof(float)*3);
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

    printMatriz(media, 4, 3);
    puts("asdasdasdasdas\n\n\n");
    perceptron(media, 4, 3);
    printMatriz(media, 4, 3);

    return 0;
}
