
#include <stdio.h>
#include <math.h>
#include "utils.h"



bool esta_contido_em(float retangulo[4], float x, float y) {
    
    //printf("%f %f %f %f\n", retangulo[0], retangulo[1], retangulo[2], retangulo[3]);
    //printf("%f %f\n", x, y);
    if (x >= retangulo[0] &&
        y >= retangulo[1] &&
        x <= retangulo[2] &&
        y <= retangulo[3]) {
    
        return true;
    }

    return false;
}

void limpa_buffer_teclado (int nit) {
    int i;
    for (i = 0; i < nit; i++) {
        __fpurge(stdin);
    }
}

void* MallocSafe(size_t bytes)
{
    void* p = malloc(bytes);

    if (p != NULL) return p;

    else
    {
        printf("Overflow Memory!\n\n");
        exit(EXIT_FAILURE);
    }
}

float relogio (struct timeval inicio) {
    struct timeval fim;
    float timedif;
    gettimeofday(&fim, NULL);
    timedif = (float)(fim.tv_sec - inicio.tv_sec);
    timedif += (float)(fim.tv_usec - inicio.tv_usec)/1000000;
    return timedif;
}


/* O código foi copiado da biblioteca random
de Eric Roberts */

int random_integer(int low, int high)
{
    int k;
    double d;

    d = (double) rand() / ( (double) RAND_MAX + 1);
    k = d * (high - low + 1);
    return low + k;
}

float random_real(float low, float high)
{
    float k;
    float d;

    d = (float) (rand( ) /(  (float) RAND_MAX + 1)  );
    k = d * (high - low);
    return low + k;
}


void randomize(int seed)
{
    srand(seed);
}
