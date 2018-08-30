#ifndef RENDER_H_INCLUDED
#define RENDER_H_INCLUDED

#define COR_AGUA (al_map_rgb(0, 127, 255))
#define COR_MARGEM (al_map_rgb(255, 230, 128))
#define LARGURA_LINHA 1.0
#define COR_LINHA (al_map_rgb(0, 0, 0))
#define COR_TEXTURA (al_map_rgb(255, 217, 102))
#define X_INICIAL_BARCO 200
#define Y_INICIAL_BARCO ((DISPLAY_HIGHT * 4)/5)


#include "ambiente.h"


typedef struct rgb
{
    unsigned char RED;
    unsigned char GREEN;
    unsigned char BLUE;
} RGB;

typedef struct bmp {
	ALLEGRO_BITMAP* bitmap;
	Node river_map[NROWS+2];
	float x;
	float angle;
	struct bmp* prox;
} BITMAP;


BITMAP* head_bmp;
BITMAP* tail_bmp;
BITMAP* ptr_frame;
ALLEGRO_MUTEX* mutex;
int nframes;


RGB cor;
ALLEGRO_DISPLAY* screen;


void* thread_render_cenarios(ALLEGRO_THREAD* thread, void* arg);
BITMAP* consome_buffer();
BITMAP* get_next_bmp();

void init_bmp();
void insert_bmp(ALLEGRO_BITMAP* bitmap, Node river_map[NROWS+2], float x, float angle);
void remove_bmp();
short is_empty_bmp();
void remove_all_bmp();


#endif // RENDER_H_INCLUDED