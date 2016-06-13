#include<stdlib.h>
#include <stdio.h>
#include <time.h>

typedef int int;

void instancia(int *x, int i, float *w, int *bias)
{
    srand(time(NULL));
    float e[i-1];
    int aprend, cont, u = 0, y, erro = 1;

    while (!erro)
    {
        for (cont = 0; cont < i-1; cont++)
            u += x[cont]*w[cont]+bias;

        y = fAtivacao(u);
        erro = x[i-1] - y;

        if(erro == 0)
        {
            continue;
        }
        for (cont = 0; cont < i-1; cont++)
            w[cont] = w[cont]+aprend*erro*x[cont];
    }

    return;
}


int perceptron (int ** x, int i, int j)
{
    float *w = (float *)malloc(sizeof(float)*(i-1));
    int *bias = (int *)malloc(sizeof(int));
    cont = 0;
    while(cont < i-1)
    {
        w[cont] = rand()%1 + 0.5;
    }

    for(cont = 0; cont < i; cont++)
        instancia(x+cont, j, w, bias);

    return 0;
}
