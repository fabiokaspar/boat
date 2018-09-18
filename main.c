#include "utils.h"
#include "render.h"
#include "ambiente.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define FACTOR 1.0

typedef enum op
{
    HOWPLAY = 1, PLAY, EXIT
} OPCAO;

enum MYKEYS {
    KEY_LEFT=0, KEY_RIGHT, KEY_UP
};


void play();
void finalizaJogo();
void inicializaJogo();
void inicializaAllegro();
void trata_evento_tecla_direcao();
void display_barco();

void corrige_inclinacao_barco();
int detectaColisao(Node map[NROWS+2]);
int teste_oito_vizinhos (Pixel centro, Node map[NROWS+2]);
int ehMargem(Pixel p, Node map[NROWS+2]);

void* thread_eventos(ALLEGRO_THREAD* thread, void* arg);
void* thread_play(ALLEGRO_THREAD* thread, void* arg);


static ALLEGRO_THREAD* thr_eventos = NULL;
static ALLEGRO_THREAD* thr_play1 = NULL;

float x, y, angle;
bool stop;

short nlifes = 10;
Pixel borda[8];

short pisca;
bool pressed_keys[3] = {false, false, false};

ALLEGRO_MUTEX* mutex;


int main()
{
    inicializaAllegro();
    carrega_imagens();
    
    inicializaJogo();
    inicializa_fontes();

    al_set_target_backbuffer(screen);
    
    menu_inicial();

    thr_eventos = al_create_thread(thread_eventos, NULL);
    
    thr_play1 = al_create_thread(thread_play, NULL);


    printf("Rodando as threads ......\n");
    
    al_start_thread(thr_eventos);
 
    al_start_thread(thr_play1);
    
    
    while(!fim)
        al_rest(0.1);

    finalizaJogo();

    al_join_thread(thr_eventos, NULL);

    al_join_thread(thr_play1, NULL);

    al_destroy_thread(thr_eventos);

    al_destroy_thread(thr_play1);

    al_destroy_mutex(mutex);

    al_destroy_display(screen);
    
    return 0;
}

void* thread_play(ALLEGRO_THREAD* thread, void* arg)
{
    struct timeval tm_colisao;
    
    struct timeval time1;
    // inicio da contagem da primeira colisão
    gettimeofday(&tm_colisao, NULL);

    printf("BY = %d\n", DISPLAY_HIGHT/NROWS);
    printf("BX = %d\n", DISPLAY_WEIGHT/NCOLS);
    
    al_set_target_backbuffer(screen);
    
    gettimeofday(&time1, NULL);
    
    while (!fim)
    {
        if (!stop)
        {   
            render_play_frame();
            
            display_barco();
            
            al_flip_display();

            trata_evento_tecla_direcao();
            
            atualizaRio(head);
            
            //printf("tempo = %f\n", relogio(time1));
            //gettimeofday(&time1, NULL);           
            if (detectaColisao(river_map) && relogio(tm_colisao) > 0.15) 
            {
                pisca = 10;
                
                nlifes--;

                if (nlifes < 0)
                    nlifes = 0;
                
                // inicio da contagem da proxima colisão
                gettimeofday(&tm_colisao, NULL);
            }
            
            al_rest(0.0);
        }
            
        else {
            //al_set_target_backbuffer(screen);
            render_pause_frame();
            al_flip_display();

            while (!fim && stop) al_rest(0.15);
        }
    }

    printf("saiu1\n");

    return NULL;
}

void display_barco() {
    char score_str[30];
    char life_str[5];

    if (!pisca) {
        al_draw_rotated_bitmap(barco, w/2, h/2, x, Y_INICIAL_BARCO, angle, 0);
        //al_draw_line(x+w/2, 0, x+w/2, DISPLAY_HIGHT, al_map_rgb(0,0,0), 1);
    }

    else {
        //al_draw_tinted_bitmap(bitmap, al_map_rgba_f(1, 1, 1, 0.5), x, y, 0);
        al_draw_tinted_rotated_bitmap(barco, al_map_rgba_f(1, 1, 1, 0.5),
            w/2, h/2, x, Y_INICIAL_BARCO, angle, 0);
        
        pisca--;
    }

    score++;

    if (score > 1000000)
        score = 1000000;
}

void* thread_eventos(ALLEGRO_THREAD* thread, void* arg) {
    al_set_target_backbuffer(NULL);

    al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
    
    ALLEGRO_EVENT_QUEUE* fila = NULL;
    ALLEGRO_EVENT event;
    fila = al_create_event_queue();

    if (!fila)
    {
        fprintf(stderr, "Não criou a fila de eventos!\n");
        exit(0);
    }
        
    al_register_event_source(fila, al_get_display_event_source(screen));
    al_register_event_source(fila, al_get_keyboard_event_source());

    while (1) 
    {
        if (!al_is_event_queue_empty(fila)) 
        {
            al_get_next_event(fila, &event);

            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                fim = true;
                break;
            }

            // tecla ENTER foi pressionada 
            if (event.type == ALLEGRO_EVENT_KEY_DOWN && 
                event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                
                stop = !stop;
                pressed_keys[KEY_LEFT] = false;
                pressed_keys[KEY_RIGHT] = false;
                pressed_keys[KEY_UP] = false;
                fim = false;
            }
        }

        if (!stop) 
        {
            if (event.type == ALLEGRO_EVENT_KEY_UP) 
            {
                if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
                    pressed_keys[KEY_LEFT] = false;
                
                else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
                    pressed_keys[KEY_RIGHT] = false;

                else if (event.keyboard.keycode == ALLEGRO_KEY_UP)
                    pressed_keys[KEY_UP] = false;
            }
                
            else if (event.type == ALLEGRO_EVENT_KEY_DOWN) 
            {
                if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
                    pressed_keys[KEY_LEFT] = true;
                
                else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
                    pressed_keys[KEY_RIGHT] = true;

                else if (event.keyboard.keycode == ALLEGRO_KEY_UP) {
                    pressed_keys[KEY_UP] = true;
                }
            }

            //trata_evento_tecla_velocidade();
        }
    }

    al_destroy_event_queue(fila);
    printf("saiu3\n");

    return NULL;
}

void inicializaJogo () {
    al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
    screen = al_create_display(DISPLAY_WEIGHT, DISPLAY_HIGHT);
    al_set_window_position(screen, 400, 50);
    al_set_display_icon(screen, icon);
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

    head = geraRio();
    fim = false;
    stop = false;
    x = X_INICIAL_BARCO;
    y = Y_INICIAL_BARCO;
    angle = 0;
    
    // cantos do barco
    borda[0] = (Pixel) {0, 0 - h/2};
    borda[1] = (Pixel) {0, 0 + h/2};    
    borda[2] = (Pixel) {0 - w/2, 0};       
    borda[3] = (Pixel) {0 + w/2, 0};
    mutex = al_create_mutex();
}


int detectaColisao (Node map[NROWS+2]) {
    Pixel p;
    short i;

    for (i = 0; i < 4; i++) {
        p = rotacao(borda[i], angle);
        p.x += x;
        p.y += y;

        if (teste_oito_vizinhos(p, map)) {
            return 1;
        }
    }

    return 0;
}

int teste_oito_vizinhos (Pixel centro, Node map[NROWS+2]) {    
    Pixel v;

    v.x = centro.x - 1, v.y = centro.y - 1;
    if (ehMargem(v, map)) { return 1;}

    v.x = centro.x - 1, v.y = centro.y;
    if (ehMargem(v, map)) { return 1;}
    
    v.x = centro.x - 1, v.y = centro.y + 1;
    if (ehMargem(v, map)) { return 1;}

    v.x = centro.x, v.y = centro.y - 1;
    if (ehMargem(v, map)) { return 1;}

    v.x = centro.x, v.y = centro.y + 1;
    if (ehMargem(v, map)) { return 1;}

    v.x = centro.x + 1, v.y = centro.y - 1;
    if (ehMargem(v, map)) { return 1;}

    v.x = centro.x + 1, v.y = centro.y;
    if (ehMargem(v, map)) { return 1;}

    v.x = centro.x + 1, v.y = centro.y + 1;
    if (ehMargem(v, map)) { return 1;}

    v.x = centro.x, v.y = centro.y;
    if (ehMargem(v, map)) { return 1;}

    return 0;
}

int ehMargem(Pixel p, Node map[NROWS+2]) {
    int linha = ceil((1.0 * p.y)/BLOCO_Y);
    int i;
    //Node* node = head;

    if (p.x <= MARGEM_ESQ * BLOCO_X || p.x >= MARGEM_DIR * BLOCO_X)
        return 1;

    //for (i = 0; i < linha; i++)
    //    node = node->prox;
    Node node = map[linha-1];
    
    int x = node.margem_esq * BLOCO_X;

    if (p.x <= x) return 1;

    x = node.margem_dir * BLOCO_X;

    if (p.x >= x) return 1;

    // testa se eh ilha
    
    if (node.inicio_ilha != -1) {
        x = node.inicio_ilha * BLOCO_X;

        if (p.x >= x-5 && p.x <= x + w_ilha+5 ) {
            //al_draw_bitmap(ilhabmp, x, ceil(p.y)-100, 0);
            
            return 1;
        }
    }
    
    return 0;
}

void trata_evento_tecla_direcao() {
    if (pressed_keys[KEY_LEFT] ^ pressed_keys[KEY_RIGHT]  ) 
    {
        if (pressed_keys[KEY_LEFT]) 
        {
            x -= FACTOR * BLOCO_X;

            if (angle > -ALLEGRO_PI/9)
            {
                // 10 graus
                angle -= ALLEGRO_PI/18;
            }
        }

        if (pressed_keys[KEY_RIGHT]) 
        {
            x += FACTOR * BLOCO_X;

            if (angle < ALLEGRO_PI/9)
            {
                angle += ALLEGRO_PI/18;
            }
        }
    } else {
        corrige_inclinacao_barco();
    }

}

void corrige_inclinacao_barco() {
    float module_angle = fabs(angle);

    if (module_angle > 0 && module_angle < ALLEGRO_PI/18)
        module_angle = 0;

    else if (angle > 0) 
    {
        angle -= ALLEGRO_PI/18;
    }

    else if (angle < 0)
    {
        angle += ALLEGRO_PI/18;
    }
}

void inicializaAllegro()
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

    // Torna apto o uso de mouse na aplicação
    if (!al_install_mouse())
    {
        fprintf(stderr, "Falha ao inicializar o mouse.\n");
        return ;
    }

    al_init_font_addon();

    if (!al_init_ttf_addon())
    {
        fprintf(stderr, "Allegro_ttf já inicializada!\n");
        return ;
    }
    
    al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
}

void finalizaJogo() {
    Queue_Free_All (head);
    //al_destroy_event_queue(fila);
    al_destroy_bitmap(barco);
    al_destroy_bitmap(ilhabmp);
    al_destroy_bitmap(life);
    al_destroy_bitmap(icon);
}
