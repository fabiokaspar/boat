#ifndef RENDER_H_INCLUDED
#define RENDER_H_INCLUDED


#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "ambiente.h"


#define RGB_SCREEN(cor)  (al_clear_to_color(al_map_rgb(cor.RED, cor.GREEN, cor.BLUE)))


typedef struct rgb
{
    unsigned char RED;
    unsigned char GREEN;
    unsigned char BLUE;
} RGB;

typedef struct
{
    RGB cor;
    ALLEGRO_DISPLAY* scr;
    ALLEGRO_BITMAP* img;
} SCREEN;

void DesenhaImagem(SCREEN* s, char* caminho, int x, int y);
void DesenhaRio(Node* head);


#endif // RENDER_H_INCLUDED
