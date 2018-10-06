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
#define PLACAR (al_map_rgba_f(0, 0, 0, 0.9))


static void render_margem_esquerda(int index, float x0, float x1, float x2);
static void render_margem_direita(int index, float x0, float x1, float x2);
static void render_textura(int index);
static void render_ilha(int index);
static void carrega_imagens();
static void desaloca_imagens();


void render_placar(long int score, int vidas, double distancia) {
    char score_str[30];
    char life_str[5];
    char distancia_str[10];

    sprintf(life_str, "%d", vidas);
    sprintf(score_str, "%ld pts", score);
    sprintf(distancia_str, "%.1fm", distancia);
    
    al_draw_filled_rectangle(0, DISPLAY_HIGHT-25, DISPLAY_WEIGHT, DISPLAY_HIGHT, PLACAR);
    al_draw_bitmap(life, DISPLAY_WEIGHT/2, DISPLAY_HIGHT-25, 0);

    // distancia
    al_draw_text(fnt_score, al_map_rgb(255, 255, 255), DISPLAY_WEIGHT-20, DISPLAY_HIGHT-25, ALLEGRO_ALIGN_RIGHT, distancia_str);
    
    // score
    //al_draw_text(fnt_score, al_map_rgb(255, 255, 255), 80, DISPLAY_HIGHT-25, ALLEGRO_ALIGN_RIGHT, "score:");
    al_draw_text(fnt_score, al_map_rgb(255, 255, 255), 60, DISPLAY_HIGHT-25, ALLEGRO_ALIGN_RIGHT, score_str);
    
    // vidas    
    al_draw_text(fnt_score, al_map_rgb(255, 255, 255), DISPLAY_WEIGHT/2, DISPLAY_HIGHT-25, ALLEGRO_ALIGN_RIGHT, life_str);
}

void render_pause() {
    al_draw_text(fnt_pause, al_map_rgb(0, 0, 0), DISPLAY_WEIGHT/2, DISPLAY_HIGHT/2.5, ALLEGRO_ALIGN_CENTRE, "P A U S E");    
}

void render_margem_direita(int index, float x0, float x1, float x2) {
    float xf, x, y;

    y = (index-1) * BLOCO_Y;

    // margem retangular direita (uma parte)
    al_draw_filled_rectangle(x1, y, MARGEM_DIR * BLOCO_X, y + BLOCO_Y, COR_MARGEM);    

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

    y = (index-1) * BLOCO_Y;

    // margem retangular esquerda (completa, desconsiderando suavização)
    al_draw_filled_rectangle(MARGEM_ESQ * BLOCO_X, y, x1, y + BLOCO_Y, COR_MARGEM);
    
    if (x0 <= x1) {
        if (x1 < x2) {
            al_draw_filled_triangle(x1, y, x1, y+BLOCO_Y, x2, y+BLOCO_Y, COR_MARGEM);            
            al_draw_line(x1, y, x2, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);
        }
        else {
            al_draw_line(x1, y, x1, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);
        }
    }
    else {
        if (x1 >= x2) {
            al_draw_filled_triangle(x0, y, x1, y, x1, y+BLOCO_Y, COR_MARGEM);            
            al_draw_line(x0, y, x1, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);
        }
        else if (x0 > x2) {
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
}

void render_textura(int index) {
    float cx, cy, r, x0, x1, x2;

    // margem esquerda
    x1 = river_map[index].margem_esq * BLOCO_X;
    x2 = river_map[index+1].margem_esq * BLOCO_X;

    r = 5;
    cx = MIN(x1, x2)-r-2;
    cy = (index-1) * BLOCO_Y;

    while (cx > -r) {
        al_draw_arc(cx, cy, r, 0.0, -1.0 * PI, COR_TEXTURA, 1.5);
        cx -= (2 * r + 2);
    }

    // margem direita
    x1 = river_map[index].margem_dir * BLOCO_X;
    x2 = river_map[index+1].margem_dir * BLOCO_X;
    
    cx = MAX(x1, x2) + r + 2; 

    while (cx < DISPLAY_WEIGHT + r) {
        al_draw_arc(cx, cy, r, 0.0, -1.0 * PI, COR_TEXTURA, 1.5);
        cx += (2 * r + 2);
    }

}

void render_cenario_frame() {
    int i;
    float x0, x1, x2, xi, xf;

    al_clear_to_color(COR_AGUA);

    al_draw_filled_rectangle(0, 0, MARGEM_ESQ * BLOCO_X, DISPLAY_HIGHT, COR_MARGEM);
    al_draw_filled_rectangle(MARGEM_DIR * BLOCO_X, 0, DISPLAY_WEIGHT, DISPLAY_HIGHT, COR_MARGEM);

    // LINHAS DO MEIO: a primeira e a ultima não aparecem no cenário (não contam)
    for (i = 1; i < NROWS + 1; i++) {
        
        x0 = river_map[i-1].margem_esq * BLOCO_X;            
        x1 = river_map[i].margem_esq * BLOCO_X;
        x2 = river_map[i+1].margem_esq * BLOCO_X;
        
        render_margem_esquerda(i, x0, x1, x2);

        x0 = river_map[i-1].margem_dir * BLOCO_X;            
        x1 = river_map[i].margem_dir * BLOCO_X;
        x2 = river_map[i+1].margem_dir * BLOCO_X;

        render_margem_direita(i, x0, x1, x2);
        
        if (river_map[i].coef_relevo != -1) {
            render_textura(i);
        }
        
        render_ilha(i);
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

    //fnt_score = al_load_font("images/digiface.ttf", 18, 0);
    fnt_score = al_load_font("images/gunplay.ttf", 22, 0);
    
    fnt = al_load_font("images/argb.ttf", 16, 0);
    
    fnt_river = al_load_font("images/adventure.ttf", 30, 0);

    fnt_texto = al_load_font("images/mich.ttf", 14, 0);
}

static void carrega_imagens() {
    icon = al_load_bitmap("images/icon.png");
    chegada = al_load_bitmap("images/chegada.png");
    barco = al_load_bitmap("images/barco.png");
    life = al_load_bitmap("images/life.png");
    ilhabmp = al_load_bitmap("images/ilha.png");
    img_menu_inicial = al_load_bitmap("images/river.png");
    img_about = al_load_bitmap("images/about.png");
    img_winner = al_load_bitmap("images/winner.png");
    img_game_over = al_load_bitmap("images/gameover.png");


    w = al_get_bitmap_width(barco);
    h = al_get_bitmap_height(barco);

    /*h_chegada = al_get_bitmap_height(chegada);
    w_chegada = al_get_bitmap_width(chegada);
    */

    h_ilha = al_get_bitmap_height(ilhabmp);
    w_ilha = al_get_bitmap_width(ilhabmp);
}

static void desaloca_imagens() {
    al_destroy_bitmap(barco);
    al_destroy_bitmap(ilhabmp);
    al_destroy_bitmap(life);
    al_destroy_bitmap(icon);
    al_destroy_bitmap(img_menu_inicial);
    al_destroy_bitmap(img_about);
    al_destroy_bitmap(img_winner);
    al_destroy_bitmap(img_game_over);
}

void inicializa_janela() {
    al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
    screen = al_create_display(DISPLAY_WEIGHT, DISPLAY_HIGHT);
    al_set_window_position(screen, 400, 50);
    al_set_window_title(screen, "River");
    
    if(!screen)
    {
        fprintf(stderr, "Falha ao criar a janela\n");
        return ;
    }

    // Atribui o cursor padrão do sistema para ser usado
    if (!al_set_system_mouse_cursor(screen, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT))
    {
        fprintf(stderr, "Falha ao atribuir ponteiro do mouse.\n");
        al_destroy_display(screen);
        exit(0);
    }
    
    carrega_imagens();
    al_set_display_icon(screen, icon);
}

void desaloca_janela() {
    desaloca_imagens();
    al_destroy_display(screen);
}

