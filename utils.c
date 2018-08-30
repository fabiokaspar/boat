
#include <stdio.h>
#include "utils.h"

void load_bitmaps() {
    //al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
    icon = al_load_bitmap("images/icon.png");
    chegada = al_load_bitmap("images/chegada.png");
    barco = al_load_bitmap("images/barco.png");

    w = al_get_bitmap_width(barco);
    h = al_get_bitmap_height(barco);

    /*h_chegada = al_get_bitmap_height(chegada);
    w_chegada = al_get_bitmap_width(chegada);
    */

    ilhabmp = al_load_bitmap("images/ilha.png");
    h_ilha = al_get_bitmap_height(ilhabmp);
    w_ilha = al_get_bitmap_width(ilhabmp);
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
