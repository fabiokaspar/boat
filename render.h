#ifndef RENDER_H_INCLUDED
#define RENDER_H_INCLUDED

#include "ambiente.h"

#define COR_AGUA (al_map_rgb(0, 127, 255))
#define COR_MARGEM (al_map_rgb(255, 230, 128))
#define LARGURA_LINHA 1.0
#define COR_LINHA (al_map_rgb(204, 153, 0))
#define COR_TEXTURA (al_map_rgb(255, 217, 102))
#define X_INICIAL_BARCO 200
#define Y_INICIAL_BARCO ((DISPLAY_HIGHT * 4)/5)


typedef struct
{
    int x;
    int y;
} Pixel;


ALLEGRO_BITMAP* chegada;
ALLEGRO_BITMAP* barco;
ALLEGRO_BITMAP* icon;
ALLEGRO_BITMAP* ilhabmp;
ALLEGRO_BITMAP* life;

int h;
int w;

int w_ilha, h_ilha;

/*
int h_chegada;
int w_chegada;
*/

ALLEGRO_DISPLAY* screen;
ALLEGRO_FONT* fnt_pause;
ALLEGRO_FONT* fnt_score;
ALLEGRO_FONT* fnt;
ALLEGRO_FONT* fnt_river;
ALLEGRO_FONT* fnt_texto;

void carrega_imagens();
void render_play_frame();
void render_pause_frame();
void inicializa_fontes();

#endif // RENDER_H_INCLUDED