#ifndef BARCO_H_INCLUDED
#define BARCO_H_INCLUDED 

#define X_INICIAL_BARCO 200
#define Y_INICIAL_BARCO ((DISPLAY_HIGHT * 4)/5)

#include "ambiente.h"
#include "render.h"

bool freia, stop, fim;
long int score;
float x, y, angle;
double velocidade, distance;
short lifes;
Pixel borda[4];
bool colision;


void inicializa_configuracao_barco();
bool houveColisao();
int detectaColisao (Node map[NROWS+2]);
int teste_oito_vizinhos (Pixel centro, Node map[NROWS+2]);
int ehMargem(Pixel p, Node map[NROWS+2]);
void corrige_inclinacao_barco();
void movimenta_barco(char direcao);
void desconta_score_por_colisao();
Pixel rotacao(Pixel p, float angle);


#endif // BARCO_H_INCLUDED