/* http://www.inf.pucrs.br/~manssour/Allegro/#eventos */

#include "utils.h"
#include "render.h"
#include "ambiente.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>


#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>


typedef enum op
{
    HOWPLAY = 1, PLAY, EXIT
} OPCAO;


int menu();
void play();
void finalizaJogo();
void inicializaJogo();
void InicializaAllegro();
void trataTecla (ALLEGRO_EVENT event);
void display_frame();
void print_pause_frame();
void limpa_buffer_teclado (int nit);
int detectaColisao ();
int teste_oito_vizinhos (ALLEGRO_BITMAP* bmp, Ponto centro, int raio);
Ponto rotacao (Ponto p, float angle);


ALLEGRO_FONT* fnt = NULL;
ALLEGRO_FONT* fnt_score = NULL;
ALLEGRO_EVENT_QUEUE* fila = NULL;
ALLEGRO_DISPLAY* scr;
ALLEGRO_BITMAP* barco;
ALLEGRO_BITMAP* icon;
Node* head;

int x, y;
float angle;
bool stop;
long int score;
long int record;
Ponto borda[8];

RGB cor;
int contador;
float h, w;
int flag;

int main()
{
    InicializaAllegro();
    inicializaJogo();
    play();

    return 0;
}


void inicializaJogo () {
    contador = 0;

    x = 200, y = 430;
    angle = 0;
    stop = false;
    cor = (RGB){0, 127, 255};


    scr = al_create_display(DISPLAY_WEIGHT, DISPLAY_HIGHT);
    
    if(!scr)
    {
        fprintf(stderr, "Falha ao criar a janela\n");
        return ;
    }

    al_clear_to_color(al_map_rgb(cor.RED, cor.GREEN, cor.BLUE));
    al_set_window_position(scr, 400, 50);
    al_set_window_title(scr, "Boat Trace");

    icon = al_load_bitmap("images/canoa.png");
    barco = al_load_bitmap("images/canoa.png");

    al_set_display_icon(scr, barco);

    w = al_get_bitmap_width(barco);
    h = al_get_bitmap_height(barco);

    fila = al_create_event_queue();

    if (!fila)
    {
        fprintf(stderr, "Não criou a fila de eventos!\n");
        exit(0);
    }

    al_draw_bitmap(barco, x, y, 0);

    al_register_event_source(fila, al_get_display_event_source(scr));
    al_register_event_source(fila, al_get_keyboard_event_source());

    head = geraRio();
    
    borda[0] = (Ponto) {0, 0 - h/2 + 5};    //ok

    borda[1] = (Ponto) {0, h/2 - 1};    //ok

    borda[2] = (Ponto) {0 - w/2, 0};        //ok

    borda[3] = (Ponto) {w/2 - 3, 0};    //ok

    printf("Loading ...\n");
    al_rest(1);
    printf("Starting\n");
    
}

int detectaColisao () {
    ALLEGRO_BITMAP* bmp = al_get_backbuffer(scr);
    Ponto p;
    unsigned char r, g, b;
    RGB rgb;
    ALLEGRO_COLOR cor;
    short i, dr;

    for (i = 0; i < 4; i++) {
        p = rotacao(borda[i], angle);
        
        for (dr = 1; dr < 4; dr++) {
            if (teste_oito_vizinhos(bmp, p, dr)) {
                return 1;
            }
        }
    }


    //Ponto p2 = rotacao(borda[1], angle);
            
    //al_draw_circle(p2.x, p2.y, 2, al_map_rgb(0,0,0), 1);

    return 0;
}

int teste_oito_vizinhos (ALLEGRO_BITMAP* bmp, Ponto centro, int raio) {
    int i;
    unsigned char r, g, b;
    ALLEGRO_COLOR cor[8];

    cor[0] = al_get_pixel(bmp, centro.x - raio, centro.y - raio);
    cor[1] = al_get_pixel(bmp, centro.x - raio, centro.y);
    cor[2] = al_get_pixel(bmp, centro.x - raio, centro.y + raio);
    cor[3] = al_get_pixel(bmp, centro.x, centro.y - raio);
    cor[4] = al_get_pixel(bmp, centro.x, centro.y + raio);
    cor[5] = al_get_pixel(bmp, centro.x + raio, centro.y - raio);
    cor[6] = al_get_pixel(bmp, centro.x + raio, centro.y);
    cor[7] = al_get_pixel(bmp, centro.x + raio, centro.y + raio);
    
    for (i = 0; i < 8; i++) {
        al_unmap_rgb(cor[i], &r, &g, &b);
        
        // rgb da areia = (255, 230, 128)
        if (r == 255 && g == 230 && b == 128) {
            return 1;
        }
    }

    return 0;
}


// em ralação a origem do plano xoy
// matriz de rotação:
// M = [cos(beta) -sen(-beta) ]
//     [sen(-beta)  cos(beta) ]
Ponto rotacao (Ponto p, float angle) {
    Ponto p2;
    float a, b;

    a = cos(angle);
    b = sin(angle);

    p2.x = x + (int) (a * p.x - b * p.y);
    p2.y = y + (int) (b * p.x + a * p.y);

    return p2;
}

void display_frame() {
    RGB_SCREEN(cor);
    DesenhaRio(head);
    char score_str[30];
    al_draw_filled_rectangle(0, DISPLAY_HIGHT-20, DISPLAY_WEIGHT, DISPLAY_HIGHT, al_map_rgb(0,0,0));

    sprintf(score_str, "score: %ld", score);
    score++;

    if (score > 1000000)
        score = 1000000;

    al_draw_text(fnt_score, al_map_rgb(255, 255, 255), DISPLAY_WEIGHT - 50, DISPLAY_HIGHT-20, ALLEGRO_ALIGN_RIGHT, score_str);
    al_draw_text(fnt_score, al_map_rgb(255, 255, 255), 70, DISPLAY_HIGHT-20, ALLEGRO_ALIGN_RIGHT, "record:");


    al_draw_rotated_bitmap (barco, w/2, h/2, x, y, angle, 0);

    al_flip_display();
    al_rest(0.01);
}

void play()
{
    ALLEGRO_EVENT event;
    ALLEGRO_EVENT prox_event;
    contador = 0;

    display_frame();

    while (1)
    {

        if (!al_is_event_queue_empty(fila)) {
            //printf("contador, time: %d, %f\n", contador, relogio(t0));
            al_get_next_event (fila, &prox_event);

            if (prox_event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                break;

            if (prox_event.keyboard.keycode != ALLEGRO_KEY_LEFT &&
                prox_event.keyboard.keycode != ALLEGRO_KEY_RIGHT &&
                prox_event.keyboard.keycode != ALLEGRO_KEY_ENTER) {

                continue;
            }


            if (prox_event.keyboard.keycode != ALLEGRO_KEY_ENTER && stop == false) { 
                
                if (prox_event.type == ALLEGRO_EVENT_KEY_UP &&
                    prox_event.keyboard.keycode == event.keyboard.keycode) {
                    
                    contador = 0;
                }
                
                else if (prox_event.type == ALLEGRO_EVENT_KEY_DOWN) {
                    
                    event = prox_event;            
                    contador = 1;
                }

            }

            // tecla ENTER foi pressionada 
            else if (prox_event.keyboard.keycode == ALLEGRO_KEY_ENTER &&
                prox_event.type == ALLEGRO_EVENT_KEY_DOWN) {
                stop = !stop;
                contador = 0;                    
            }

        }

        if (!stop)
        {
            limpa_buffer_teclado(5);
            atualizaRio(head);
            trataTecla(event);
            
            
            display_frame();
               
            if (detectaColisao()) {
                printf("COLISÃO! = %d\n", flag++);
            } 
            
        }
            
        else {
            print_pause_frame(); 
        } 
        
    }

    finalizaJogo();
}

void trataTecla (ALLEGRO_EVENT event) {
    if (contador && event.keyboard.keycode == ALLEGRO_KEY_LEFT)
    {
        
        x -= 7;

        if (angle > -ALLEGRO_PI/9)
        {
            // 10 graus
            angle -= ALLEGRO_PI/18;
        }

    }
    
    else if (contador && event.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
            
        x += 7;

        if (angle < ALLEGRO_PI/9)
        {
            angle += ALLEGRO_PI/18;
        }

    }
    else 
    {
        if(angle > 0) {
            angle -= ALLEGRO_PI/18;
            

            if(angle < 0)
                angle = 0;
        }

        if(angle < 0)
        {
            angle += ALLEGRO_PI/18;

            if(angle > 0)
                angle = 0;
        }
    }
}

void InicializaAllegro()
{
    if (!al_init())
    {
        fprintf(stderr, "Falha ao inicializar a Allegro 5\n");
        return ;
    }

    if (!al_init_image_addon())
    {
        fprintf(stderr, "Falha ao inicializar a add-on_image Allegro 5\n");
        return ;
    }

    if (!al_init_primitives_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on de primitivas.\n");
        return ;
    }

    if (!al_install_keyboard())
    {
        fprintf(stderr, "Falha ao inicializar teclado\n");
        return ;
    }

    al_init_font_addon();

    if (!al_init_ttf_addon())
    {
        fprintf(stderr, "Allegro_ttf já inicializada!\n");
        return ;
    }
    
    fnt = al_load_font("images/FreeSerif.ttf", 30, 0);
    fnt_score = al_load_font("images/FreeSerif.ttf", 20, 0);
    
    if (!fnt)
    {
        fprintf(stderr, "Falha ao carregar fonte.\n");
        exit(0);
    }
}

void finalizaJogo() {
    DesalocaAmbiente(head);
    al_destroy_event_queue(fila);
    al_destroy_bitmap(barco);
    al_destroy_display(scr);
}

void print_pause_frame() {
    al_draw_text(fnt, al_map_rgb(0, 0, 0), DISPLAY_WEIGHT/2, DISPLAY_HIGHT/2.5, ALLEGRO_ALIGN_CENTRE, "PAUSE");
    al_flip_display();
}

void limpa_buffer_teclado (int nit) {
    int i;
    for (i = 0; i < nit; i++) {
        __fpurge(stdin);
    }
}

