#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED



/* Sorteia um inteiro no intervalo [a,b]  */
int random_integer(int low, int high);

/* Sorteia um flutuante no intervalo [a,b] */
float random_real(float low, float high);

/* Inicializa uma semente */
void randomize(int seed);

#endif // RANDOM_H_INCLUDED
