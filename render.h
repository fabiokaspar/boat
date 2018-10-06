#ifndef RENDER_H_INCLUDED
#define RENDER_H_INCLUDED

#include "ambiente.h"

#define COR_AGUA (al_map_rgb(0, 127, 255))
#define COR_MARGEM (al_map_rgb(255, 230, 128))
#define LARGURA_LINHA 1.0
#define COR_LINHA (al_map_rgb(204, 153, 0))
#define COR_TEXTURA (al_map_rgb(255, 217, 102))



typedef struct
{
    int x;
    int y;
} Pixel;

int h, w, w_ilha, h_ilha;

ALLEGRO_BITMAP* chegada;
ALLEGRO_BITMAP* barco;
ALLEGRO_BITMAP* icon;
ALLEGRO_BITMAP* ilhabmp;
ALLEGRO_BITMAP* life;
ALLEGRO_BITMAP* img_menu_inicial;
ALLEGRO_BITMAP* img_about;
ALLEGRO_BITMAP* img_winner;
ALLEGRO_BITMAP* img_game_over;

ALLEGRO_DISPLAY* screen;

ALLEGRO_FONT* fnt_pause;
ALLEGRO_FONT* fnt_score;
ALLEGRO_FONT* fnt;
ALLEGRO_FONT* fnt_river;
ALLEGRO_FONT* fnt_texto;


void render_cenario_frame();
void render_pause();
void inicializa_fontes();
void inicializa_janela();
void desaloca_janela();
void render_placar(long int score, int vidas, double distancia);

#endif // RENDER_H_INCLUDED