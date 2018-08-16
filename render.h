#ifndef RENDER_H_INCLUDED
#define RENDER_H_INCLUDED

#define COR_AGUA (al_map_rgb(0, 127, 255))
#define COR_MARGEM (al_map_rgb(255, 230, 128))

#include "ambiente.h"


typedef struct rgb
{
    unsigned char RED;
    unsigned char GREEN;
    unsigned char BLUE;
} RGB;

RGB cor;
ALLEGRO_DISPLAY* screen;



void main_render(Node* head);

#endif // RENDER_H_INCLUDED