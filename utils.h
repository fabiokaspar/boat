#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>


bool esta_contido_em(float retangulo[], float x, float y);


void* MallocSafe(size_t bytes);

float relogio (struct timeval inicio);

/* Sorteia um inteiro no intervalo [a,b]  */
int random_integer(int low, int high);

/* Sorteia um flutuante no intervalo [a,b] */
float random_real(float low, float high);

/* Inicializa uma semente */
void randomize(int seed);

void limpa_buffer_teclado (int nit);



#endif // UTILS_H_INCLUDED
