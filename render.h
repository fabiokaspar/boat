#ifndef RENDER_H_INCLUDED
#define RENDER_H_INCLUDED



#include "ambiente.h"


typedef struct rgb
{
    unsigned char RED;
    unsigned char GREEN;
    unsigned char BLUE;
} RGB;

RGB cor;
ALLEGRO_DISPLAY* screen;

void DesenhaRio(Node* head);

#endif // RENDER_H_INCLUDED