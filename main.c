/* http://www.inf.pucrs.br/~manssour/Allegro/#eventos */

#include "utils.h"


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

/* Macros de configuração do jogo   */
#define DISPLAY_HIGHT 540
#define DISPLAY_WEIGHT 480

#define NCOLS 80
#define NROWS 60
#define MARGEM_ESQ 10
#define MARGEM_DIR 70
#define INTERVALO 5
#define SIZE_ILHA 10
#define PROBABILITY_ILHA 0.8
#define SEMENTE 5
#define FOLGA_ILHAS 15
#define QUADRADO_PIXELS 2

#define FPS 120
#define RGB_SCREEN(cor)  (al_clear_to_color(al_map_rgb(cor.RED, cor.GREEN, cor.BLUE)))


typedef enum op
{
    HOWPLAY = 1, PLAY, EXIT
} OPCAO;

typedef struct conta_tecla {
    ALLEGRO_EVENT ev;
    int contador;
} CT;

typedef struct
{
    int RED;
    int GREEN;
    int BLUE;
} RGB;

/*
typedef struct
{
    RGB cor;
    ALLEGRO_DISPLAY* scr;
    ALLEGRO_BITMAP* img;
} SCREEN;

*/

int menu();
void play();
void finalizaJogo();
void inicializaJogo();
void InicializaAllegro();
void trataTecla (ALLEGRO_EVENT event);
void display_frame();
void print_pause_frame();
void limpa_buffer_teclado (int nit);


ALLEGRO_FONT* fnt = NULL;
ALLEGRO_EVENT_QUEUE* fila = NULL;
//Node* head;
ALLEGRO_DISPLAY* scr;
ALLEGRO_BITMAP* img;
ALLEGRO_BITMAP* icon;


int x, y;
float angle;
bool stop;

RGB cor;
int repeatstop;
int contador;
float h, w;


int main()
{
    InicializaAllegro();
    play();

    return 0;
}


void inicializaJogo () {
    repeatstop = 0;
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
    al_set_display_icon(scr, icon);

    img = al_load_bitmap("images/canoa.png");

    w = al_get_bitmap_width(img);
    h = al_get_bitmap_height(img);

    fila = al_create_event_queue();

    if (!fila)
    {
        fprintf(stderr, "Não criou a fila de eventos!\n");
        exit(0);
    }

    al_draw_bitmap(img, x, y, 0);

    al_register_event_source(fila, al_get_display_event_source(scr));
    al_register_event_source(fila, al_get_keyboard_event_source());
    RGB_SCREEN(cor);


    //head = geraRio();
}

void finalizaJogo() {
    //DesalocaAmbiente(head);
    al_destroy_event_queue(fila);
    al_destroy_bitmap(img);
    al_destroy_display(scr);
}


void display_frame() {
    RGB_SCREEN(cor);
            
    //DesenhaRio(head);
    al_draw_rotated_bitmap(img, w/2, h/2, x, y, angle, 0);
    //PercorreFilaImagens(s);

    al_flip_display();
    //atualizaRio(head);
}

void print_pause_frame() {
    al_draw_text(fnt, al_map_rgb(50, 50, 50), 240, 220, ALLEGRO_ALIGN_CENTRE, "pause");
    al_flip_display();
}

void limpa_buffer_teclado (int nit) {
    int i;
    for (i = 0; i < nit; i++) {
        __fpurge(stdin);
    }
}

void play()
{
    inicializaJogo();
    ALLEGRO_EVENT event;
    ALLEGRO_EVENT prox_event;
    contador = 0;

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
                    printf("UP ");                
                }
                
                else if (prox_event.type == ALLEGRO_EVENT_KEY_DOWN) {
                    
                    printf("DOWN ");                
                    event = prox_event;            
                    contador = 1;
                }

                /*
                else if (prox_event.keyboard.repeat && contador == 0) {
                    event = prox_event;            
                    contador = 1;
                }
                */

            }

            /* tecla ENTER foi pressionada */
            else if (prox_event.keyboard.keycode == ALLEGRO_KEY_ENTER &&
                prox_event.type == ALLEGRO_EVENT_KEY_DOWN) {
                stop = !stop;
                contador = 0;                    
            }

        }

        if (!stop)
        {
            //limpa_buffer_teclado(2);
            trataTecla(event);
            display_frame();
        }
            
        else {
            print_pause_frame(); 
        } 
        
    }

    finalizaJogo();
}

void trataTecla (ALLEGRO_EVENT event) {
    if (1) {
        if (contador && event.keyboard.keycode == ALLEGRO_KEY_LEFT)
        {
            
            //al_get_keyboard_state(ret_state);
            printf("esquerda %d\n", contador);
            x -= 7;

            if (angle > -ALLEGRO_PI/9)
            {
                // 10 graus
                angle -= ALLEGRO_PI/18;
            }

            //printf("TECLA ESQUERDA\n");
        }
        

        else if (contador && event.keyboard.keycode == ALLEGRO_KEY_RIGHT) {

            printf("direita %d\n", contador);
                
            x += 7;

            if (angle < ALLEGRO_PI/9)
            {
                angle += ALLEGRO_PI/18;
            }

            //printf("TECLA DIREITA\n");
        }
        else 
        {
           // al_flush_event_queue(fila);

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
    
    fnt = al_load_font("images/FreeSerif.ttf", 50, 0);
    
    if (!fnt)
    {
        fprintf(stderr, "Falha ao carregar fonte.\n");
        exit(0);
    }
}
