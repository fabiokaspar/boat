#include "render.h"
#include "utils.h"
#include <math.h>
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#define PI 3.141592
#define MIN(a, b) ( (a) <= (b) ? (a) : (b)  )
#define LARGURA_LINHA 1
#define COR_LINHA (al_map_rgb(204, 153, 0))
#define COR_TEXTURA (al_map_rgb(255, 217, 102))
#define FORMAT_PIXEL ALLEGRO_PIXEL_FORMAT_ANY
#define LOCK_PIXEL ALLEGRO_LOCK_READWRITE
#define NTHREADS 2

void* computa_thread(ALLEGRO_THREAD*, void*);
void renderNode(int);
void renderTextura(int index);
void renderIlha(int index);


ALLEGRO_BITMAP* screen_bmp = NULL;
static ALLEGRO_MUTEX* mutex = NULL;
static ALLEGRO_THREAD* threads[NTHREADS];




void main_render(Node* head) {
    int i, j;
        
    mutex = al_create_mutex();
    
    al_set_target_bitmap(NULL);

    al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
    screen_bmp = al_create_bitmap(DISPLAY_WEIGHT, DISPLAY_HIGHT);

    // Cria as threads e as dispara
    for (i = 0; i < NTHREADS; i++) {
        
        threads[i] = al_create_thread(computa_thread, (void*) i);
        al_start_thread(threads[i]);
    }

    for (j = 0; j < NTHREADS; j++) {
        al_join_thread(threads[j], NULL);
        al_destroy_thread(threads[j]);
    }

    al_destroy_mutex(mutex);
    
    printf("Sou a Thread PAI\n");

    //ALLEGRO_BITMAP* bmp = al_get_backbuffer(al_get_current_display());
    //al_lock_bitmap(bmp, FORMAT_PIXEL, LOCK_PIXEL);
    al_set_target_backbuffer(screen);
    al_draw_bitmap(screen_bmp, 0, 0, 0);
    al_destroy_bitmap(screen_bmp);
    //al_unlock_bitmap(bmp);
    
}

void* computa_thread(ALLEGRO_THREAD* thread, void* arg) {
    int i;
    int id = ((int) arg);
    ALLEGRO_BITMAP* bmp;

    al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
    bmp = al_create_bitmap(DISPLAY_WEIGHT, DISPLAY_HIGHT);

    al_set_target_bitmap(bmp);
    al_clear_to_color(COR_AGUA);

    int inicio = id * (NROWS+2)/2;
    int fim = (id+1) * (NROWS+2)/2;
    printf("Thread id=%d\n",id );

    for (i = inicio; i < fim; i++) 
    {
        renderNode(i);
    }
    
    //al_lock_bitmap(screen_bmp, FORMAT_PIXEL, LOCK_PIXEL);

    al_lock_mutex(mutex);
    al_set_target_bitmap(screen_bmp);
    al_draw_bitmap_region(bmp, 0, id * DISPLAY_HIGHT/2, DISPLAY_WEIGHT, DISPLAY_HIGHT/2, 0, id * DISPLAY_HIGHT/2, 0);
    al_set_target_bitmap(NULL);
    al_unlock_mutex(mutex);
    
    //al_unlock_bitmap(screen_bmp);
    

    al_destroy_bitmap(bmp);

    return NULL;
}

void renderTextura(int index) {
    /* textura margem esquerda 
    */
    double mod_seno, alfa;
    float xf, x, x0, x1, x2, y;
    
    x0 = margem[index-1].margem_esq * BLOCO_X;            
    x1 = margem[index].margem_esq * BLOCO_X;
    x2 = margem[index+1].margem_esq * BLOCO_X;
    y = (index-1) * BLOCO_Y;

    xf = MIN(x0, x1);
    xf = MIN(xf, x2);

    for (x = 0; x <= xf; x += 1) 
    {
        alfa = x * PI/60;
        mod_seno = fabs(cos(alfa)/margem[index].coef_relevo);

        al_draw_line(x, y + (1-mod_seno) * BLOCO_Y + 1, x, y + (1-mod_seno) * BLOCO_Y,
            COR_TEXTURA, margem[index].coef_relevo);
    }
}

void renderNode (int index)
{
    float xf, x, x0, x1, x2, y;

    if (index == 0 || index == NROWS+1)
        return;
        
    x0 = margem[index-1].margem_esq * BLOCO_X;            
    x1 = margem[index].margem_esq * BLOCO_X;
    x2 = margem[index+1].margem_esq * BLOCO_X;

    y = (index-1) * BLOCO_Y;

    // margem retangular esquerda
    al_draw_filled_rectangle(0, y, x1, y + BLOCO_Y, COR_MARGEM);
    
    // LINHAS DO MEIO: a primeira e a ultima não aparecem no cenário (não contam)
    
    if (x0 > x1 && x2 > x1) 
    {
        if (x0 > x2) {
            // extende o retangulo
            al_draw_filled_rectangle(x1, y, x2, y + BLOCO_Y, al_map_rgb(255, 230, 128));
            al_draw_filled_triangle(x2, y + BLOCO_Y, x2, y, x0, y, al_map_rgb(255, 230, 128));            
            al_draw_line(x0, y, x2, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);
        }
        else if (x0 == x2) {
            // extende o retangulo
            al_draw_filled_rectangle(x1, y, x2, y + BLOCO_Y, al_map_rgb(255, 230, 128));
            al_draw_line(x0, y, x2, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);
        }
        else {
            al_draw_filled_rectangle(x1, y, x0, y + BLOCO_Y, al_map_rgb(255, 230, 128));
            al_draw_filled_triangle(x0, y, x0, y + BLOCO_Y, x2, y + BLOCO_Y, al_map_rgb(255, 230, 128));            
            al_draw_line(x0, y, x2, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);
        }
    }

    else if (x0 <= x1) {
        if (x1 < x2) {
            al_draw_filled_triangle(x1, y, x1, y + BLOCO_Y, x2, y + BLOCO_Y, al_map_rgb(255, 230, 128));            
            al_draw_line(x1, y, x2, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);   
        }
        else {
            al_draw_line(x1, y, x1, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);   
        }
    }

    else if (x0 > x1) {
        al_draw_filled_triangle(x1, y, x1, y + BLOCO_Y, x0, y, al_map_rgb(255, 230, 128));            
        al_draw_line(x0, y, x1, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);
    }
    
    renderTextura(index);
    renderIlha(index);
    
}

void renderIlha(int index) {
    // DESENHA ILHA
    float y = (index-1) * BLOCO_Y;
        
    if (margem[index].inicio_ilha != -1) 
    {    
        // no pixel y=100 começa a parte de terra da ilha 
        al_draw_bitmap(ilhabmp, margem[index].inicio_ilha * BLOCO_X, y-100, 0);
    }
}

