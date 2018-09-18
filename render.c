#include "render.h"
#include "utils.h"
#include "ambiente.h"
#include <math.h>
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#define PI 3.141592
#define MIN(a, b) ( (a) <= (b) ? (a) : (b)  )
#define MAX(a, b) ( (a) >= (b) ? (a) : (b)  )
#define FORMAT_PIXEL ALLEGRO_PIXEL_FORMAT_ANY
#define LOCK_PIXEL ALLEGRO_LOCK_READWRITE



static void render_margem_esquerda(int index, float x0, float x1, float x2);
static void render_margem_direita(int index, float x0, float x1, float x2);
static void render_textura(int index, float x0, float x1, float x2, float xi, float xf);
static void render_ilha(int index);


void render_margem_direita(int index, float x0, float x1, float x2) {
    float xf, x, y;

    y = (index-1) * BLOCO_Y;

    // margem retangular direita (uma parte)
    al_draw_filled_rectangle(x1, y, MARGEM_DIR * BLOCO_X, y + BLOCO_Y, COR_MARGEM);    

    if (x0 >= x1) {
        if (x2 >= x1) {
            //al_draw_line(x1, y, x1, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);
        } 
        else {
            al_draw_filled_triangle(x1, y, x1, y+BLOCO_Y, x2, y+BLOCO_Y, COR_MARGEM);
            //al_draw_line(x1, y, x2, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);
        }
    }
    else {
        if (x1 <= x2) {
            al_draw_filled_triangle(x0, y, x1, y, x1, y+BLOCO_Y, COR_MARGEM);
            //al_draw_line(x0, y, x1, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);   
        } 
        else if (x0 < x2) {
            al_draw_filled_rectangle(x2, y, x1, y+BLOCO_Y, COR_MARGEM);            
            al_draw_filled_triangle(x0, y, x2, y, x2, y+BLOCO_Y, COR_MARGEM);
            //al_draw_line(x0, y, x2, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);   
        }
        else if (x0 == x2) {
            al_draw_filled_rectangle(x0, y, x1, y+BLOCO_Y, COR_MARGEM);            
            //al_draw_line(x0, y, x0, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);   
        }
        else {
            al_draw_filled_rectangle(x0, y, x1, y+BLOCO_Y, COR_MARGEM);
            al_draw_filled_triangle(x0, y, x0, y+BLOCO_Y, x2, y+BLOCO_Y, COR_MARGEM);
            //al_draw_line(x0, y, x2, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);   
        }
    }
}

void render_margem_esquerda(int index, float x0, float x1, float x2)
{
    float xf, x, y;


    y = (index-1) * BLOCO_Y;

    // margem retangular esquerda (uma parte)
    al_draw_filled_rectangle(MARGEM_ESQ * BLOCO_X, y, x1, y + BLOCO_Y, COR_MARGEM);
    
    //al_draw_line(0, y, DISPLAY_WEIGHT, y, COR_TEXTURA, 1);
    // LINHAS DO MEIO: a primeira e a ultima não aparecem no cenário (não contam)
    
    if (x0 > x1 && x2 > x1) 
    {
        if (x0 > x2) {
            // extende o retangulo
            al_draw_filled_rectangle(x1, y, x2, y + BLOCO_Y, COR_MARGEM);
            al_draw_filled_triangle(x2, y + BLOCO_Y, x2, y, x0, y, COR_MARGEM);            
            //al_draw_line(x0, y, x2, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);
        }
        else if (x0 == x2) {
            // extende o retangulo
            al_draw_filled_rectangle(x1, y, x2, y + BLOCO_Y, COR_MARGEM);
            //al_draw_line(x0, y, x2, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);
        }
        else {
            al_draw_filled_rectangle(x1, y, x0, y + BLOCO_Y, COR_MARGEM);
            al_draw_filled_triangle(x0, y, x0, y + BLOCO_Y, x2, y + BLOCO_Y, COR_MARGEM);            
            //al_draw_line(x0, y, x2, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);
        }
    }

    else if (x0 <= x1) {
        if (x1 < x2) {
            al_draw_filled_triangle(x1, y, x1, y + BLOCO_Y, x2, y + BLOCO_Y, COR_MARGEM);            
            //al_draw_line(x1, y, x2, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA );   
        }
        else {
            //al_draw_line(x1, y, x1, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);   
        }
    }

    else if (x0 > x1) {
        al_draw_filled_triangle(x1, y, x1, y + BLOCO_Y, x0, y, COR_MARGEM);            
        //al_draw_line(x0, y, x1, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);
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
    
    for (x = xi+1; x < xf-1; x += 2) 
    {
        alfa = x * PI/60;
        mod_seno = fabs(cos(alfa)/relevo);

        al_draw_line(x, y + (1-mod_seno) * BLOCO_Y + 1, x, y + (1-mod_seno) * BLOCO_Y,
            COR_TEXTURA, river_map[index].coef_relevo);
    }
}

void render_ilha(int index) {
    float y;
    
    if (river_map[index].inicio_ilha != -1) 
    {    
        y = (index-1) * BLOCO_Y;
        // no pixel y=48 começa a parte de terra da ilha 
        //al_draw_filled_rectangle(river_map[index].inicio_ilha * BLOCO_X, y,river_map[index].inicio_ilha * BLOCO_X+ w_ilha, y + BLOCO_Y, COR_MARGEM);
        al_draw_bitmap(ilhabmp, river_map[index].inicio_ilha * BLOCO_X, y-48, 0);
    }
}

void inicializa_fontes() {
    fnt_pause = al_load_font("images/gunplay.ttf", 40, 0);

    if (!fnt_pause)
    {
        fprintf(stderr, "Falha ao carregar fonte.\n");
        exit(0);
    }

    fnt_score = al_load_font("images/digiface.ttf", 15, 0);
    
    fnt = al_load_font("images/argb.ttf", 20, 0);
    
    fnt_river = al_load_font("images/adventure.ttf", 30, 0);

    fnt_texto = al_load_font("images/mich.ttf", 15, 0);
}

void carrega_imagens() {
    icon = al_load_bitmap("images/icon.png");
    chegada = al_load_bitmap("images/chegada.png");
    barco = al_load_bitmap("images/barco.png");
    life = al_load_bitmap("images/life.png");
    ilhabmp = al_load_bitmap("images/ilha.png");

    w = al_get_bitmap_width(barco);
    h = al_get_bitmap_height(barco);

    /*h_chegada = al_get_bitmap_height(chegada);
    w_chegada = al_get_bitmap_width(chegada);
    */

    h_ilha = al_get_bitmap_height(ilhabmp);
    w_ilha = al_get_bitmap_width(ilhabmp);
}


void render_play_frame() {
    int i;
    float x0, x1, x2, xi, xf;

    al_clear_to_color(COR_AGUA);

    al_draw_filled_rectangle(0, 0, MARGEM_ESQ * BLOCO_X, DISPLAY_HIGHT, COR_MARGEM);
    al_draw_filled_rectangle(MARGEM_DIR * BLOCO_X, 0, DISPLAY_WEIGHT, DISPLAY_HIGHT, COR_MARGEM);

    for (i = 1; i < NROWS + 1; i++) {
        
        x0 = river_map[i-1].margem_esq * BLOCO_X;            
        x1 = river_map[i].margem_esq * BLOCO_X;
        x2 = river_map[i+1].margem_esq * BLOCO_X;
        
        render_margem_esquerda(i, x0, x1, x2);
        
        xf = MIN(x0, x1);
        xf = MIN(xf, x2);

        //render_textura(i, x0, x1, x2, 0, xf);

        x0 = river_map[i-1].margem_dir * BLOCO_X;            
        x1 = river_map[i].margem_dir * BLOCO_X;
        x2 = river_map[i+1].margem_dir * BLOCO_X;

        render_margem_direita(i, x0, x1, x2);
        
        xi = MAX(x0, x1);
        xi = MAX(xi, x2);
        
        //render_textura(i, x0, x1, x2, xi, DISPLAY_WEIGHT);
        
        render_ilha(i);
    }

    al_draw_bitmap(life, DISPLAY_WEIGHT/2, DISPLAY_HIGHT-25, 0);
    //al_draw_text(fnt_score, al_map_rgb(0,0,0), DISPLAY_WEIGHT-50, DISPLAY_HIGHT-25, ALLEGRO_ALIGN_RIGHT, "score:");
    //al_draw_text(fnt_score, al_map_rgb(0,0,0), 70, DISPLAY_HIGHT-25, ALLEGRO_ALIGN_RIGHT, "record:");     
}

void render_pause_frame() {
    //al_draw_filled_rectangle(DISPLAY_WEIGHT/2.0, DISPLAY_HIGHT/2.5, DISPLAY_WEIGHT/2.0 + 100, DISPLAY_HIGHT/2.5 + 100, al_map_rgb(255,255,255));
    al_draw_text(fnt_pause, al_map_rgb(0, 0, 0), DISPLAY_WEIGHT/2, DISPLAY_HIGHT/2.5, ALLEGRO_ALIGN_CENTRE, "P A U S E");    
}

