/* http://www.inf.pucrs.br/~manssour/Allegro/#eventos */

#include "utils.h"
#include "render.h"
#include "ambiente.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>


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
void ajusta_barco();
int teste_oito_vizinhos (ALLEGRO_BITMAP* bmp, Ponto centro, int raio);
Ponto rotacao (Ponto p, float angle);


ALLEGRO_FONT* fnt = NULL;
ALLEGRO_FONT* fnt_score = NULL;
ALLEGRO_EVENT_QUEUE* fila = NULL;
ALLEGRO_DISPLAY* scr;
ALLEGRO_BITMAP* barco;
ALLEGRO_BITMAP* icon;
ALLEGRO_BITMAP* img_init;

int x, y;
float angle;
bool stop;

long int record;
short nlifes = 10;
Ponto borda[8];

RGB cor;
int contador;
float h, w;
int flag;
short pisca;
bool pressed_keys[2] = {false, false};

enum MYKEYS {
    KEY_LEFT, KEY_RIGHT
};


int main()
{
    InicializaAllegro();
    inicializaJogo();
    play();

    return 0;
}


void inicializaJogo () {
    contador = 0;
    //ALLEGRO_EVENT prox_event;

    x = 200, y = 400;
    angle = 0;
    stop = false;
    cor = (RGB){0, 127, 255};
    
    scr = al_create_display(800, 450);
    al_set_window_position(scr, 300, 100);
    al_draw_bitmap(al_load_bitmap("images/boattrace.png"), 0, 0, 0);
    icon = al_load_bitmap("images/icon.png");
    al_set_display_icon(scr, icon);
    al_set_window_title(scr, "Boat Trace");
    al_flip_display();

    al_rest(1);
    
    al_resize_display(scr, DISPLAY_WEIGHT, DISPLAY_HIGHT);
    al_set_window_position(scr, 400, 50);

    if(!scr)
    {
        fprintf(stderr, "Falha ao criar a janela\n");
        return ;
    }

    al_clear_to_color(al_map_rgb(cor.RED, cor.GREEN, cor.BLUE));
    
    barco = al_load_bitmap("images/canoa.png");

    w = al_get_bitmap_width(barco);
    h = al_get_bitmap_height(barco);

    fila = al_create_event_queue();
        
    al_register_event_source(fila, al_get_display_event_source(scr));
    al_register_event_source(fila, al_get_keyboard_event_source());


    if (!fila)
    {
        fprintf(stderr, "Não criou a fila de eventos!\n");
        exit(0);
    }

    al_draw_bitmap(barco, x, y, 0);

    head = geraRio();
    
    borda[0] = (Ponto) {0, 0 - h/2 + 5};    //ok

    borda[1] = (Ponto) {0, h/2 - 1};    //ok

    borda[2] = (Ponto) {0 - w/2, 0};        //ok

    borda[3] = (Ponto) {w/2 - 3, 0};    //ok


    //printf("Loading ...\n");
    //al_rest(2);
    printf("Starting ...\n");
    
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
    char life_str[5];
    Node* node;
    float bloco_y = ((float)DISPLAY_HIGHT)/NROWS;
    float bloco_x = ((float)DISPLAY_WEIGHT)/NCOLS;


    if (pisca) {
        //al_draw_tinted_bitmap(bitmap, al_map_rgba_f(1, 1, 1, 0.5), x, y, 0);
        al_draw_tinted_rotated_bitmap(barco, al_map_rgba_f(1, 1, 1, 0.5),
            w/2, h/2, x, y, angle, 0);
        
        pisca--;
    }

    else {
        al_draw_rotated_bitmap (barco, w/2, h/2, x, y, angle, 0);
    }

    if (fim_jogo && conta_passos < passos_chegada) {
        
        al_draw_bitmap_region(chegada, 0, 0, w_chegada, h_chegada, x_chegada, conta_passos * bloco_y, 0);
        conta_passos++;

        if (conta_passos == passos_chegada) {
            fim_jogo = 0;
            conta_passos = -30;
        }    
    }
    

    al_draw_filled_rectangle(0, DISPLAY_HIGHT-20, DISPLAY_WEIGHT, DISPLAY_HIGHT, al_map_rgb(0,0,0));

    sprintf(score_str, "score: %ld", score);
    score++;

    if (score > 1000000)
        score = 1000000;

    al_draw_text(fnt_score, al_map_rgb(255, 255, 255), DISPLAY_WEIGHT - 50, DISPLAY_HIGHT-20, ALLEGRO_ALIGN_RIGHT, score_str);
    al_draw_text(fnt_score, al_map_rgb(255, 255, 255), 70, DISPLAY_HIGHT-20, ALLEGRO_ALIGN_RIGHT, "record:");

    sprintf(life_str, "%d", nlifes);
    
    al_draw_text(fnt_score, al_map_rgb(255, 255, 255), DISPLAY_WEIGHT/2-10, DISPLAY_HIGHT-20, ALLEGRO_ALIGN_RIGHT, life_str);

    al_draw_bitmap(al_load_bitmap("images/life.png"), DISPLAY_WEIGHT/2, DISPLAY_HIGHT-20, 0);

    float times;

    al_flip_display();
    
    if (score < 10)
        times = 0.01;
    else 
        times = 0.0185;

    al_rest(times);
    //al_rest(0.005);
}

void play()
{
    ALLEGRO_EVENT event;
    ALLEGRO_EVENT prox_event;
    contador = 0;
    struct timeval tm_colisao;

    // inicio da contagem da primeira colisão
    gettimeofday(&tm_colisao, NULL);

    display_frame();

    while (1)
    {
        if (!al_is_event_queue_empty(fila)) {

            al_get_next_event (fila, &prox_event);

            if (prox_event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                break;

            if (prox_event.keyboard.keycode != ALLEGRO_KEY_LEFT &&
                prox_event.keyboard.keycode != ALLEGRO_KEY_RIGHT &&
                prox_event.keyboard.keycode != ALLEGRO_KEY_ENTER) {

                continue;
            }


            if (prox_event.keyboard.keycode != ALLEGRO_KEY_ENTER && stop == false) { 
                
                if (prox_event.type == ALLEGRO_EVENT_KEY_UP) {
                    contador = 0;

                    if (prox_event.keyboard.keycode == ALLEGRO_KEY_LEFT)
                        pressed_keys[KEY_LEFT] = false;
                    else if (prox_event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
                        pressed_keys[KEY_RIGHT] = false;

                }
                
                else if (prox_event.type == ALLEGRO_EVENT_KEY_DOWN) {
                    event = prox_event;            
                    contador = 1;

                    if (prox_event.keyboard.keycode == ALLEGRO_KEY_LEFT)
                        pressed_keys[KEY_LEFT] = true;
                    else if (prox_event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
                        pressed_keys[KEY_RIGHT] = true;
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
               
            if (detectaColisao() && relogio(tm_colisao) > 0.3) {
                //printf("COLISÃO! = %d\n", flag++);
                pisca = 5;
                
                nlifes--;

                if (nlifes < 0)
                    nlifes = 0;
                
                // inicio da contagem da proxima colisão
                gettimeofday(&tm_colisao, NULL);
            } 
            
        }
            
        else {
            print_pause_frame(); 
        } 
        
    }

    finalizaJogo();
}

void trataTecla (ALLEGRO_EVENT event) {
    if ( (pressed_keys[KEY_LEFT] ^ pressed_keys[KEY_RIGHT] ) ) {
        switch (event.keyboard.keycode) 
        {
            case ALLEGRO_KEY_LEFT:
                x -= 7;

                if (angle > -ALLEGRO_PI/9)
                {
                    // 10 graus
                    angle -= ALLEGRO_PI/18;
                }

                break;
             
             case ALLEGRO_KEY_RIGHT:
                x += 7;

                if (angle < ALLEGRO_PI/9)
                {
                    angle += ALLEGRO_PI/18;
                }
                
                break;
        }    
    }

    else 
    {
        ajusta_barco();
    }
}

void ajusta_barco() {
    if(angle > 0) 
    {
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

