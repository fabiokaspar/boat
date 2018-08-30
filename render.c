#include "render.h"
#include "utils.h"
#include <math.h>
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#define PI 3.141592
#define MIN(a, b) ( (a) <= (b) ? (a) : (b)  )
#define MAX(a, b) ( (a) >= (b) ? (a) : (b)  )
#define FORMAT_PIXEL ALLEGRO_PIXEL_FORMAT_ANY
#define LOCK_PIXEL ALLEGRO_LOCK_READWRITE
#define NTHREADS 2
#define NFRAMES 25


void render_margem_esquerda(int index, float x0, float x1, float x2);
void render_margem_direita(int index, float x0, float x1, float x2);
void render_textura(int index, float x0, float x1, float x2, float xi, float xf);
void render_ilha(int index);
void render_frame();


ALLEGRO_BITMAP* screen_bmp = NULL;


    
//al_destroy_mutex(mutex);  


void* thread_render_cenarios(ALLEGRO_THREAD* thread, void* arg) {
    al_set_target_backbuffer(NULL);
    
    int id = ((int) arg);
    
    //al_set_new_display_refresh_rate(0);
    al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
    
    printf("Thread id=%d\n", id);
    
    head = geraRio();
    
    al_lock_mutex(mutex);
    fim = false;
    init_bmp();
    al_unlock_mutex(mutex);

    while (!fim) {
        while (nframes > NFRAMES && !fim) al_rest(0.5);
        if (fim) break;
                
        ALLEGRO_BITMAP* bmp = al_create_bitmap(DISPLAY_WEIGHT, DISPLAY_HIGHT);
        al_set_target_bitmap(bmp);
        
        al_clear_to_color(COR_AGUA);

        render_frame();
        
        al_lock_mutex(mutex);
        insert_bmp(al_clone_bitmap(bmp), river_map, -1, 0);
        al_unlock_mutex(mutex);
        
        al_destroy_bitmap(bmp);
        
        atualizaRio(head);
    }

    printf("saiu2\n");
    //al_draw_bitmap_region(bmp, 0, id * DISPLAY_HIGHT/2, DISPLAY_WEIGHT, DISPLAY_HIGHT/2, 0, id * DISPLAY_HIGHT/2, 0);
    
    return NULL;
}

void render_margem_direita(int index, float x0, float x1, float x2) {
    float xf, x, y;

    //if (index == 0 || index == NROWS+1)
    //    return;

    //x0 = river_map[index-1].margem_dir * BLOCO_X;            
    //x1 = river_map[index].margem_dir * BLOCO_X;
    //x2 = river_map[index+1].margem_dir * BLOCO_X;
    y = (index-1) * BLOCO_Y;

    // margem retangular direita
    al_draw_filled_rectangle(x1, y, DISPLAY_WEIGHT, y + BLOCO_Y, COR_MARGEM);    

    if (x0 >= x1) {
        if (x2 >= x1) {
            al_draw_line(x1, y, x1, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);
        } 
        else {
            al_draw_filled_triangle(x1, y, x1, y+BLOCO_Y, x2, y+BLOCO_Y, COR_MARGEM);
            al_draw_line(x1, y, x2, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);
        }
    }
    else {
        if (x1 <= x2) {
            al_draw_filled_triangle(x0, y, x1, y, x1, y+BLOCO_Y, COR_MARGEM);
            al_draw_line(x0, y, x1, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);   
        } 
        else if (x0 < x2) {
            al_draw_filled_rectangle(x2, y, x1, y+BLOCO_Y, COR_MARGEM);            
            al_draw_filled_triangle(x0, y, x2, y, x2, y+BLOCO_Y, COR_MARGEM);
            al_draw_line(x0, y, x2, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);   
        }
        else if (x0 == x2) {
            al_draw_filled_rectangle(x0, y, x1, y+BLOCO_Y, COR_MARGEM);            
            al_draw_line(x0, y, x0, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);   
        }
        else {
            al_draw_filled_rectangle(x0, y, x1, y+BLOCO_Y, COR_MARGEM);
            al_draw_filled_triangle(x0, y, x0, y+BLOCO_Y, x2, y+BLOCO_Y, COR_MARGEM);
            al_draw_line(x0, y, x2, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);   
        }
    }
}

void render_margem_esquerda(int index, float x0, float x1, float x2)
{
    float xf, x, y;

    //if (index == 0 || index == NROWS+1)
    //    return;
        
    //x0 = river_map[index-1].margem_esq * BLOCO_X;            
    //x1 = river_map[index].margem_esq * BLOCO_X;
    //x2 = river_map[index+1].margem_esq * BLOCO_X;

    y = (index-1) * BLOCO_Y;

    // margem retangular esquerda
    al_draw_filled_rectangle(0, y, x1, y + BLOCO_Y, COR_MARGEM);
    
    // LINHAS DO MEIO: a primeira e a ultima não aparecem no cenário (não contam)
    
    if (x0 > x1 && x2 > x1) 
    {
        if (x0 > x2) {
            // extende o retangulo
            al_draw_filled_rectangle(x1, y, x2, y + BLOCO_Y, COR_MARGEM);
            al_draw_filled_triangle(x2, y + BLOCO_Y, x2, y, x0, y, COR_MARGEM);            
            al_draw_line(x0, y, x2, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);
        }
        else if (x0 == x2) {
            // extende o retangulo
            al_draw_filled_rectangle(x1, y, x2, y + BLOCO_Y, COR_MARGEM);
            al_draw_line(x0, y, x2, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);
        }
        else {
            al_draw_filled_rectangle(x1, y, x0, y + BLOCO_Y, COR_MARGEM);
            al_draw_filled_triangle(x0, y, x0, y + BLOCO_Y, x2, y + BLOCO_Y, COR_MARGEM);            
            al_draw_line(x0, y, x2, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);
        }
    }

    else if (x0 <= x1) {
        if (x1 < x2) {
            al_draw_filled_triangle(x1, y, x1, y + BLOCO_Y, x2, y + BLOCO_Y, COR_MARGEM);            
            al_draw_line(x1, y, x2, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA );   
        }
        else {
            al_draw_line(x1, y, x1, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);   
        }
    }

    else if (x0 > x1) {
        al_draw_filled_triangle(x1, y, x1, y + BLOCO_Y, x0, y, COR_MARGEM);            
        al_draw_line(x0, y, x1, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);
    }
}

void render_textura(int index, float x0, float x1, float x2, float xi, float xf) {
    /* textura river_map esquerda 
    */
    double mod_seno, alfa;
    float x, y;
    int relevo = river_map[index].coef_relevo;

    if (relevo == -1)
        return;

    y = (index-1) * BLOCO_Y;
    
    for (x = xi+1; x < xf-1; x += 1) 
    {
        alfa = x * PI/60;
        mod_seno = fabs(cos(alfa)/relevo);

        al_draw_line(x, y + (1-mod_seno) * BLOCO_Y + 1, x, y + (1-mod_seno) * BLOCO_Y,
            COR_TEXTURA, relevo);
    }
}

void render_ilha(int index) {
    float y;
    
    if (river_map[index].inicio_ilha != -1) 
    {    
        y = (index-1) * BLOCO_Y;
        // no pixel y=100 começa a parte de terra da ilha 
        //al_draw_filled_rectangle(river_map[index].inicio_ilha * BLOCO_X, y,river_map[index].inicio_ilha * BLOCO_X+ w_ilha, y + BLOCO_Y, COR_MARGEM);
        al_draw_bitmap(ilhabmp, river_map[index].inicio_ilha * BLOCO_X, y-70, 0);
    }
}

void render_frame() {
    int i;
    float x0, x1, x2, xi, xf;

    for (i = 1; i < NROWS + 1; i++) {
        
        x0 = river_map[i-1].margem_esq * BLOCO_X;            
        x1 = river_map[i].margem_esq * BLOCO_X;
        x2 = river_map[i+1].margem_esq * BLOCO_X;
        
        render_margem_esquerda(i, x0, x1, x2);
        
        xf = MIN(x0, x1);
        xf = MIN(xf, x2);

        render_textura(i, x0, x1, x2, 0, xf);

        x0 = river_map[i-1].margem_dir * BLOCO_X;            
        x1 = river_map[i].margem_dir * BLOCO_X;
        x2 = river_map[i+1].margem_dir * BLOCO_X;

        render_margem_direita(i, x0, x1, x2);
        
        xi = MAX(x0, x1);
        xi = MAX(xi, x2);
        
        render_textura(i, x0, x1, x2, xi, DISPLAY_WEIGHT);
        
        render_ilha(i);
    }     
}

BITMAP* consome_buffer() {
    if (is_empty_bmp())
        return NULL;

    BITMAP* bmp = get_next_bmp();
    remove_bmp();

    //printf("nframes = %d\n", nframes);
    return bmp;
}


/* struct BITMAP's implementation*/


BITMAP* get_next_bmp() {
    if (is_empty_bmp())
        return NULL;

    return head_bmp->prox;
}

void init_bmp() {
    head_bmp = malloc(sizeof(BITMAP));
    
    head_bmp->prox = NULL;

    tail_bmp = head_bmp;

    if (!head_bmp) {
        printf("Malloc returned null!\n");
        exit(0);
    }

    nframes = 0;
}

void insert_bmp(ALLEGRO_BITMAP* bitmap, Node river_map[NROWS+2], float x, float angle) {
    int i;

    BITMAP* novo_bmp = malloc(sizeof(BITMAP));
    
    novo_bmp->bitmap = bitmap;
    
    for (i = 0; i < NROWS+2; i++) {
        novo_bmp->river_map[i] = river_map[i];
    }

    novo_bmp->x = x;

    novo_bmp->angle = angle;

    novo_bmp->prox = NULL;

    tail_bmp->prox = novo_bmp;

    tail_bmp = novo_bmp;

    nframes++;

    //printf("nframes = %d\n", nframes);
}

void remove_bmp() {
    if (is_empty_bmp())
        return;

    BITMAP* lixo = head_bmp->prox;

    head_bmp->prox = lixo->prox;

    if (lixo == tail_bmp)
        tail_bmp = head_bmp;

    free(lixo);

    nframes--;

    printf("nframes = %d\n", nframes);
}

short is_empty_bmp() {
    return (head_bmp == tail_bmp);
}

void remove_all_bmp() {
    while(!is_empty_bmp()) {
        remove_bmp();
    }
    
    free(head_bmp);
}
