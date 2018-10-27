#include "render.h"
#include "eventos.h"

#include <math.h>
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#define V (-0.8)
#define PI 3.141592
#define PLACAR (al_map_rgba_f(0, 0, 0, 0.9))

static const char* msg1 = "PRESSIONE ENTER PARA RETORNAR AO MENU";
static const char* msg2 = "PRESSIONE Q (QUIT) PARA SAIR";

static void render_margem_esquerda(int index, float x0, float x1, float x2);
static void render_margem_direita(int index, float x0, float x1, float x2);
static void render_textura(int index);
static void render_ilha(int index);
static void carrega_imagens();
static void desaloca_imagens();
static void render_bandeira(float xesq, float xdir, float y);



void render_ilha(int index) {
    float y = (index-1) * BLOCO_Y;
    Sequencia_Ilhas seq = river_map[index].seq;
    int n = seq.qtd;
    short i;

    if (n > 0) 
    {    
        for (i = 0; i < n; i++) { 
            // no pixel y=48 começa a parte de terra da ilha
            al_draw_bitmap(ilhabmp, (seq.inicio + i * BLOCOS_ILHA)  * BLOCO_X, y-48, 0);
            //al_draw_bitmap(ilhabmp, (river_map[index].inicio_ilha + 9) * BLOCO_X, y-48, 0);
        }
    }
}

void render_frame_initial() {
    render_cenario_frame();
    render_placar(score, lifes, distance);    
    render_barco_wait_on_colision();
    al_draw_text(fnt_texto2, al_map_rgb(0, 0, 0), DISPLAY_WEIGHT/2, DISPLAY_HIGHT/2, ALLEGRO_ALIGN_CENTRE, "press any key to start");
}

void inicializa_fontes() {
    fnt_pause = al_load_font("images/gunplay.ttf", 40, 0);
    
    fnt_texto2 = al_load_font("images/adventure.ttf", 20, 0);

    if (!fnt_pause)
    {
        fprintf(stderr, "Falha ao carregar fonte.\n");
        exit(0);
    }

    //fnt_score = al_load_font("images/digiface.ttf", 18, 0);
    fnt_score = al_load_font("images/gunplay.ttf", 22, 0);
    
    fnt_score2 = al_load_font("images/gunplay.ttf", 18, 0);
    
    fnt = al_load_font("images/argb.ttf", 16, 0);
    
    fnt_river = al_load_font("images/adventure.ttf", 30, 0);

    fnt_texto = al_load_font("images/mich.ttf", 10, 0);
    
    fnt_texto_final_lost = al_load_font("images/digiface.ttf", 22, 0);
    
    fnt_digiface2 = al_load_font("images/digiface.ttf", 16, 0);
    
    fnt_freeserif = al_load_font("images/FreeSerif.ttf", 10, 0);
    
    fnt_freeserif2 = al_load_font("images/FreeSerif.ttf", 14, 0);
    
    fnt_texto_final_win = al_load_font("images/adventure.ttf", 24, 0);
}


void render_winner() {
    al_clear_to_color(COR_TELA);
    al_draw_bitmap(img_winner, 0, 0, 0);
    al_draw_text(fnt_texto_final_win, al_map_rgb(40, 40, 40), 235, 300, ALLEGRO_ALIGN_CENTRE, "W I N N E R");
    al_draw_text(fnt_texto, al_map_rgb(0, 0, 0), DISPLAY_WEIGHT/2, DISPLAY_HIGHT - 50, ALLEGRO_ALIGN_CENTRE, msg1);
    al_draw_text(fnt_texto, al_map_rgb(0, 0, 0), DISPLAY_WEIGHT/2, DISPLAY_HIGHT - 30, ALLEGRO_ALIGN_CENTRE, msg2);
    
    char distance_final[20];
    char score_final[30];

    sprintf(distance_final, "Distance: %.1f m", distance);
    sprintf(score_final, "Score: %ld pts", score);

    al_draw_text(fnt_freeserif2, al_map_rgb(0, 0, 0), 10, 20, ALLEGRO_ALIGN_LEFT, distance_final);
    al_draw_text(fnt_freeserif2, al_map_rgb(0, 0, 0), 10, 50, ALLEGRO_ALIGN_LEFT, score_final);
}

void render_cenario_frame() {
    int i;
    float x0_esq, x1_esq, x2_esq;
    float x0_dir, x1_dir, x2_dir;
    float y;

    al_clear_to_color(COR_AGUA);

    al_draw_filled_rectangle(0, 0, MARGEM_ESQ * BLOCO_X, DISPLAY_HIGHT, COR_MARGEM);
    al_draw_filled_rectangle(MARGEM_DIR * BLOCO_X, 0, DISPLAY_WEIGHT, DISPLAY_HIGHT, COR_MARGEM);

    // LINHAS DO MEIO: a primeira e a ultima não aparecem no cenário (não contam)
    for (i = 1; i < NROWS + 1; i++) {
        
        y = (i-1) * BLOCO_Y;
        x0_esq = river_map[i-1].margem_esq * BLOCO_X;            
        x1_esq = river_map[i].margem_esq * BLOCO_X;
        x2_esq = river_map[i+1].margem_esq * BLOCO_X;

        render_margem_esquerda(i, x0_esq, x1_esq, x2_esq);

        x0_dir = river_map[i-1].margem_dir * BLOCO_X;            
        x1_dir = river_map[i].margem_dir * BLOCO_X;
        x2_dir = river_map[i+1].margem_dir * BLOCO_X;

        render_margem_direita(i, x0_dir, x1_dir, x2_dir);
        
        if (river_map[i].tem_relevo) {
            render_textura(i);
        }
        
        if (river_map[i].eh_fim) {
            render_bandeira(MARGEM_ESQ * BLOCO_X, MARGEM_DIR * BLOCO_X, y-40);
        }   

        render_ilha(i);
    }
}

static void render_bandeira(float xesq, float xdir, float y) {
    al_draw_bitmap(bandeira, xesq - 15, y, 0);
    //al_draw_bitmap(bandeira, DISPLAY_WEIGHT-40, y, 0);
}


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
    al_draw_text(fnt_score, al_map_rgb(255, 255, 255), 80, DISPLAY_HIGHT-25, ALLEGRO_ALIGN_RIGHT, score_str);
    
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
            al_draw_line(x1+V, y, x1+V, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);
        } 
        else {
            al_draw_filled_triangle(x1, y, x1, y+BLOCO_Y, x2, y+BLOCO_Y, COR_MARGEM);
            al_draw_line(x1+V, y, x2+V, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);
        }
    }
    else {
        if (x1 <= x2) {
            al_draw_filled_triangle(x0, y, x1, y, x1, y+BLOCO_Y, COR_MARGEM);
            al_draw_line(x0+V, y, x1+V, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);   
        } 
        else if (x0 < x2) {
            al_draw_filled_rectangle(x2, y, x1, y+BLOCO_Y, COR_MARGEM);            
            al_draw_filled_triangle(x0, y, x2, y, x2, y+BLOCO_Y, COR_MARGEM);
            al_draw_line(x0+V, y, x2+V, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);   
        }
        else if (x0 == x2) {
            al_draw_filled_rectangle(x0, y, x1, y+BLOCO_Y, COR_MARGEM);            
            al_draw_line(x0+V, y, x0+V, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);   
        }
        else {
            al_draw_filled_rectangle(x0, y, x1, y+BLOCO_Y, COR_MARGEM);
            al_draw_filled_triangle(x0, y, x0, y+BLOCO_Y, x2, y+BLOCO_Y, COR_MARGEM);
            al_draw_line(x0+V, y, x2+V, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);   
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
            al_draw_line(x1-V, y, x2-V, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);
        }
        else {
            al_draw_line(x1-V, y, x1-V, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);
        }
    }
    else {
        if (x1 >= x2) {
            al_draw_filled_triangle(x0, y, x1, y, x1, y+BLOCO_Y, COR_MARGEM);            
            al_draw_line(x0-V, y, x1-V, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);
        }
        else if (x0 > x2) {
            // extende o retangulo
            al_draw_filled_rectangle(x1, y, x2, y + BLOCO_Y, COR_MARGEM);
            al_draw_filled_triangle(x2, y + BLOCO_Y, x2, y, x0, y, COR_MARGEM);            
            al_draw_line(x0-V, y, x2-V, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);
        }
        else if (x0 == x2) {
            // extende o retangulo
            al_draw_filled_rectangle(x1, y, x2, y + BLOCO_Y, COR_MARGEM);
            al_draw_line(x0-V, y, x2-V, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);
        }
        else {
            al_draw_filled_rectangle(x1, y, x0, y + BLOCO_Y, COR_MARGEM);
            al_draw_filled_triangle(x0, y, x0, y + BLOCO_Y, x2, y + BLOCO_Y, COR_MARGEM);            
            al_draw_line(x0-V, y, x2-V, y+BLOCO_Y, COR_LINHA, LARGURA_LINHA);
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

void render_about() {
    al_clear_to_color(COR_TELA);
    al_draw_bitmap(img_about, 0, 0, 0);
    al_draw_text(fnt_freeserif2, al_map_rgb(0,0,0), 10, DISPLAY_HIGHT - 25, ALLEGRO_ALIGN_LEFT, "MENU");
    al_draw_scaled_bitmap(img_prev, 0, 0, 80, 80, 5, DISPLAY_HIGHT - 80, 45, 45, 0);
}

void render_game_over() {
    al_draw_scaled_bitmap(img_game_over, 0, 0, 419, 605, 0, 0, 400, 500, 0);
    al_draw_text(fnt_texto_final_lost, al_map_rgb(30, 30, 30), DISPLAY_WEIGHT/2, DISPLAY_HIGHT/4, ALLEGRO_ALIGN_CENTRE, "GAME  OVER");
    al_draw_text(fnt_texto, al_map_rgb(0, 0, 0), DISPLAY_WEIGHT/2, DISPLAY_HIGHT - 160, ALLEGRO_ALIGN_CENTRE, msg1);
    al_draw_text(fnt_texto, al_map_rgb(0, 0, 0), DISPLAY_WEIGHT/2, DISPLAY_HIGHT - 140, ALLEGRO_ALIGN_CENTRE, msg2);
}

void desaloca_janela() {
    desaloca_imagens();
    al_destroy_display(screen);
}

void render_tela_inicial_fundo() {
    al_clear_to_color(COR_TELA);
    al_draw_scaled_bitmap(img_menu_inicial, 0, 0, 392, 365, 0, 0, 392, 365, 0);
    al_draw_text(fnt_river, COR_TEXT_MOUSE, DISPLAY_WEIGHT/2, DISPLAY_HIGHT/3, ALLEGRO_ALIGN_CENTRE, "R I V E R");
}

void render_tela_inicial_botoes(short botao) {
    float coord[4] = {DISPLAY_WEIGHT/4.0, 300, 3.0 * DISPLAY_WEIGHT/4.0, 490};
    /*
    float coord_play[4] = {(coord[0]+coord[2])/2 - 40, coord[1] + 20, (coord[0]+coord[2])/2 + 40, coord[1] + 40};
    float coord_instr[4] = {(coord[0]+coord[2])/2 - 130, coord[1] + 80, (coord[0]+coord[2])/2 + 130, coord[1] + 100};
    float coord_quit[4] = {(coord[0]+coord[2])/2 - 40, coord[1] + 140, (coord[0]+coord[2])/2 + 40, coord[1] + 160};
    */

    if (botao == bt_play)
        al_draw_text(fnt, COR_TEXT_MOUSE, (coord[0]+coord[2])/2, coord[1] + 20, ALLEGRO_ALIGN_CENTRE, "PLAY");
    else
        al_draw_text(fnt, COR_TEXT_BUTTONS, (coord[0]+coord[2])/2, coord[1] + 20, ALLEGRO_ALIGN_CENTRE, "PLAY");

    if (botao == bt_about)
        al_draw_text(fnt, COR_TEXT_MOUSE, (coord[0]+coord[2])/2, coord[1] + 80, ALLEGRO_ALIGN_CENTRE, "INSTRUCTIONS");
    else 
        al_draw_text(fnt, COR_TEXT_BUTTONS, (coord[0]+coord[2])/2, coord[1] + 80, ALLEGRO_ALIGN_CENTRE, "INSTRUCTIONS");

    if (botao == bt_quit)
        al_draw_text(fnt, COR_TEXT_MOUSE, (coord[0]+coord[2])/2, coord[1] + 140, ALLEGRO_ALIGN_CENTRE, "QUIT");
    else
        al_draw_text(fnt, COR_TEXT_BUTTONS, (coord[0]+coord[2])/2, coord[1] + 140, ALLEGRO_ALIGN_CENTRE, "QUIT");
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
    al_destroy_bitmap(img_prev);
    al_destroy_bitmap(bandeira);
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

static void carrega_imagens() {
    bandeira = al_load_bitmap("images/flag-40.png");
    icon = al_load_bitmap("images/icon.png");
    barco = al_load_bitmap("images/canoa.png");
    life = al_load_bitmap("images/life.png");
    ilhabmp = al_load_bitmap("images/ilha.png");
    img_menu_inicial = al_load_bitmap("images/river.png");
    img_about = al_load_bitmap("images/about.png");
    img_winner = al_load_bitmap("images/winner.png");
    img_game_over = al_load_bitmap("images/gameover.png");
    img_prev = al_load_bitmap("images/prev80.png");

    w = al_get_bitmap_width(barco);
    h = al_get_bitmap_height(barco);

    h_ilha = al_get_bitmap_height(ilhabmp);
    w_ilha = al_get_bitmap_width(ilhabmp);

    //printf("largura ilha = %d\n", w_ilha);
    //printf("largura barco = %d\n", w);
}