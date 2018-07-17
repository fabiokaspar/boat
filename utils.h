#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

ALLEGRO_BITMAP* mesqc;
ALLEGRO_BITMAP* mesqd;
ALLEGRO_BITMAP* chegada;
ALLEGRO_BITMAP* barco;
ALLEGRO_BITMAP* icon;
ALLEGRO_BITMAP* ilhabmp;

int h;
int w;

int h_chegada;
int w_chegada;

void load_bitmaps();

void* MallocSafe(size_t bytes);

float relogio (struct timeval inicio);

/* Sorteia um inteiro no intervalo [a,b]  */
int random_integer(int low, int high);

/* Sorteia um flutuante no intervalo [a,b] */
float random_real(float low, float high);

/* Inicializa uma semente */
void randomize(int seed);


#endif // UTILS_H_INCLUDED
