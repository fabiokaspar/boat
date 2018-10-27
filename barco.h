#ifndef BARCO_H_INCLUDED
#define BARCO_H_INCLUDED 

#define X_INICIAL 200
#define Y_INICIAL ((DISPLAY_HIGHT * 4)/5)
#define VEL_MIN 1.5
#define VEL_MAX 3.0


#include "ambiente.h"
#include "render.h"


float x, y, angle;
double velocidade;
Pixel borda[4];

void inicializa_configuracao_barco();
bool houveColisao();
int detectaColisao (Node map[NROWS+2]);
int teste_quatro_vizinhos (Pixel centro, Node map[NROWS+2]);
int ehMargem(Pixel p, Node map[NROWS+2]);
void corrige_inclinacao_barco();
void movimenta_barco(char direcao);
Pixel rotacao(Pixel p, float angle);


#endif // BARCO_H_INCLUDED