#include <stdlib.h>
#include "random.h"
#include <time.h>

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
