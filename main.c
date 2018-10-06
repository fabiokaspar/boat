#include "utils.h"
#include "render.h"
#include "ambiente.h"
//#include "eventos.h"
//#include "barco.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define X_INICIAL_BARCO 200
#define Y_INICIAL_BARCO ((DISPLAY_HIGHT * 4)/5)
#define BLOCK_TRANSLATION 1.0

#define INTERVAL_CLOCK 0.03
#define INTERVAL_COLISION 0.12

bool pressed_keys[3] = {false, false, false};

enum MYKEYS {
    KEY_LEFT=0, KEY_RIGHT, KEY_UP
};

ALLEGRO_THREAD* thr_eventos;
ALLEGRO_THREAD* thr_timer;
ALLEGRO_TIMER *gerador_clock;
ALLEGRO_MUTEX *mutex = NULL;
ALLEGRO_COND *cond = NULL;

bool freia, stop, fim;
long int score;
float x, y, angle;
double velocidade, distance;
short lifes;
Pixel borda[4];
bool colision;


void inicializa_configuracao_barco();
bool houveColisao();
int detectaColisao (Node map[NROWS+2]);
int teste_oito_vizinhos (Pixel centro, Node map[NROWS+2]);
int ehMargem(Pixel p, Node map[NROWS+2]);
void corrige_inclinacao_barco();
void movimenta_barco(char direcao);
void desconta_score_por_colisao();
Pixel rotacao (Pixel p, float angle);

void* thread_eventos(ALLEGRO_THREAD* thread, void* arg);
void* thread_timer(ALLEGRO_THREAD* thread, void* arg);

void finaliza_jogo();
void inicializa_jogo();
void inicializa_allegro();
void play();
void render_barco();

void trata_evento_tecla_direcao();
bool render_pause_lock_on_stop();
bool lock_on_stop();



int main()
{
    inicializa_allegro();
    inicializa_janela();
    inicializa_fontes();

    al_set_target_backbuffer(screen);
    
    while (1) {
        menu_inicial();
        inicializa_jogo();
        play();
        finaliza_jogo();
    }

    desaloca_janela();
    
    return 0;
}

void* thread_eventos(ALLEGRO_THREAD* thread, void* arg) {
    al_set_target_backbuffer(NULL);
    
    ALLEGRO_EVENT event;
    ALLEGRO_EVENT_QUEUE* fila = NULL;
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
        al_wait_for_event(fila, NULL);

        if (!al_is_event_queue_empty(fila)) 
        {
            al_get_next_event(fila, &event);

            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                fim = true;
                al_broadcast_cond(cond);
                break;
            }

            // tecla ENTER foi pressionada 
            if (event.type == ALLEGRO_EVENT_KEY_DOWN && 
                event.keyboard.keycode == ALLEGRO_KEY_ENTER) 
            {
                
                stop = !stop;
                pressed_keys[KEY_LEFT] = false;
                pressed_keys[KEY_RIGHT] = false;
                pressed_keys[KEY_UP] = false;
                fim = false;

                if (!stop) {
                    al_broadcast_cond(cond);
                }
                
                continue;
            }

            if (!stop) 
            {
                if (event.type == ALLEGRO_EVENT_KEY_UP) 
                {
                    switch (event.keyboard.keycode) 
                    {
                        case ALLEGRO_KEY_LEFT:
                            pressed_keys[KEY_LEFT] = false;
                            break;

                        case ALLEGRO_KEY_RIGHT:
                            pressed_keys[KEY_RIGHT] = false;
                            break;

                        case ALLEGRO_KEY_UP:
                            pressed_keys[KEY_UP] = false;
                            break;

                        default: break;
                    }
                }
                    
                else if (event.type == ALLEGRO_EVENT_KEY_DOWN) 
                {
                    switch (event.keyboard.keycode) 
                    {
                        case ALLEGRO_KEY_LEFT:
                            pressed_keys[KEY_LEFT] = true;
                            break;

                        case ALLEGRO_KEY_RIGHT:
                            pressed_keys[KEY_RIGHT] = true;
                            break;

                        case ALLEGRO_KEY_UP:
                            pressed_keys[KEY_UP] = true;
                            break;

                        default: break;
                    }
                }
            }
        }
    }

    al_destroy_event_queue(fila);
    printf("saiu ev.\n");

    return NULL;
}

void play()
{

    while (!fim)
    {
        render_cenario_frame();
        render_placar(score, lifes, distance);
        render_barco();
        
        if (!render_pause_lock_on_stop()) {
            al_flip_display();
        }
        
        trata_evento_tecla_direcao();
        atualizaRio(head);
    
        //al_rest(0.012);
    }

    printf("saiu1\n");
}

void render_barco() {
    if (colision) {
        al_draw_tinted_rotated_bitmap(barco, al_map_rgba_f(1, 1, 1, 0.5),
            w/2, h/2, x, Y_INICIAL_BARCO, angle, 0);         
    
        al_flip_display();
        al_rest(INTERVAL_COLISION);
    
        colision = false;
    }
    else {
        al_draw_rotated_bitmap(barco, w/2, h/2, x, Y_INICIAL_BARCO, angle, 0);
    }
}

bool render_pause_lock_on_stop() {
    if (stop && !fim) {
        render_pause();
        
        al_flip_display();
    
        al_lock_mutex(mutex);

        //while (stop) 
        al_wait_cond(cond, mutex);

        al_unlock_mutex(mutex);

        return true;
    }

    return false;
}

bool lock_on_stop() {
    if (stop && !fim) {
        al_lock_mutex(mutex);

        //while (stop) 
        al_wait_cond(cond, mutex);

        al_unlock_mutex(mutex);

        return true;
    }

    return false;
}


void inicializa_jogo () {
    head = geraRio();
    inicializa_configuracao_barco();
    mutex = al_create_mutex();
    
    //printf("Criando e rodando a thread eventos ......\n");

    thr_eventos = al_create_thread(thread_eventos, NULL);
    thr_timer = al_create_thread(thread_timer, NULL);
    cond = al_create_cond();

    al_start_thread(thr_eventos);
    al_start_thread(thr_timer);
}

void finaliza_jogo() {
    Queue_Free_All (head);
    //printf("Finalizando threads ......\n");
    
    al_join_thread(thr_eventos, NULL);
    al_join_thread(thr_timer, NULL);
    
    al_destroy_thread(thr_eventos);
    al_destroy_thread(thr_timer);
    
    al_destroy_mutex(mutex);
    al_destroy_cond(cond);
}

void trata_evento_tecla_direcao() {
    if (pressed_keys[KEY_LEFT] ^ pressed_keys[KEY_RIGHT]) 
    {
        if (pressed_keys[KEY_LEFT]) 
        {
            movimenta_barco('L');
        }

        if (pressed_keys[KEY_RIGHT]) 
        {
            movimenta_barco('R');
        }
    } else {
        corrige_inclinacao_barco();
    }
}

void inicializa_configuracao_barco() {
    fim = false;
    freia = false;
    stop = false;
    colision = false;
    x = X_INICIAL_BARCO;
    y = Y_INICIAL_BARCO;
    angle = 0;
    lifes = 10;
    score = 0;
    velocidade = 1.5;
    distance = 0;

    // cantos do barco
    borda[0] = (Pixel) {0, 0 - h/2};
    borda[1] = (Pixel) {0, 0 + h/2};    
    borda[2] = (Pixel) {0 - w/2, 0};       
    borda[3] = (Pixel) {0 + w/2, 0};
}

void desconta_score_por_colisao() {
    if (score > 0) 
    {
        if (score < 10)
            score = 0;

        else score -= 10;
    }
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

    //v.x = centro.x - 1, v.y = centro.y - 1;
    //if (ehMargem(v, map)) { return 1;}

    v.x = centro.x - 1, v.y = centro.y;
    if (ehMargem(v, map)) { return 1;}
    
    //v.x = centro.x - 1, v.y = centro.y + 1;
    //if (ehMargem(v, map)) { return 1;}

    v.x = centro.x, v.y = centro.y - 1;
    if (ehMargem(v, map)) { return 1;}

    v.x = centro.x, v.y = centro.y + 1;
    if (ehMargem(v, map)) { return 1;}

    //v.x = centro.x + 1, v.y = centro.y - 1;
    //if (ehMargem(v, map)) { return 1;}

    v.x = centro.x + 1, v.y = centro.y;
    if (ehMargem(v, map)) { return 1;}

    //v.x = centro.x + 1, v.y = centro.y + 1;
    //if (ehMargem(v, map)) { return 1;}

    v.x = centro.x, v.y = centro.y;
    if (ehMargem(v, map)) { return 1;}

    return 0;
}

int ehMargem(Pixel p, Node map[NROWS+2]) {
    int linha = ceil((1.0 * p.y)/BLOCO_Y);
    int i;

    if (p.x <= MARGEM_ESQ * BLOCO_X || p.x >= MARGEM_DIR * BLOCO_X)
        return 1;

    Node node = map[linha-1];
    
    int x = node.margem_esq * BLOCO_X;

    if (p.x <= x) return 1;

    x = node.margem_dir * BLOCO_X;

    if (p.x >= x) return 1;

    // testa se eh ilha
    
    if (node.inicio_ilha != -1) {
        x = node.inicio_ilha * BLOCO_X;

        if (p.x >= x-5 && p.x <= x + w_ilha+5 ) {            
            return 1;
        }
    }
    
    return 0;
}


void movimenta_barco(char direcao) {
    if (direcao == 'L') 
    {
        x -= BLOCK_TRANSLATION * BLOCO_X;

        if (angle > -ALLEGRO_PI/9)
        {
            // 10 graus
            angle -= ALLEGRO_PI/18;
        }
    }
    else if (direcao == 'R') 
    {
        x += BLOCK_TRANSLATION * BLOCO_X;

        if (angle < ALLEGRO_PI/9)
        {
            angle += ALLEGRO_PI/18;
        }
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

// em ralação a origem do plano xoy
// matriz de rotação:
// M = [cos(beta) -sen(-beta) ]
//     [sen(-beta)  cos(beta) ]
Pixel rotacao (Pixel p, float angle) {
    Pixel p2;
    float a, b;

    a = cos(angle);
    b = sin(angle);

    p2.x = (int) (a * p.x - b * p.y);
    p2.y = (int) (b * p.x + a * p.y);

    return p2;
}

bool houveColisao() {
    if (detectaColisao(river_map)) 
    {
        
        return true;
    }

    return false;
}

void inicializa_allegro()
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

void* thread_timer(ALLEGRO_THREAD* thread, void* arg) {
    al_set_target_backbuffer(NULL);

    short slot_colision = 0, slot_score = 0;
    double acrescimo_distancia;    
    
    ALLEGRO_EVENT event;
    ALLEGRO_EVENT_QUEUE* queue_timer = NULL;

    queue_timer = al_create_event_queue();
    
    if (!queue_timer)
    {
        fprintf(stderr, "Não criou a fila de eventos queue_timer!\n");
        exit(0);
    }

    gerador_clock = al_create_timer(INTERVAL_CLOCK);
    
    if (!gerador_clock) {
        fprintf(stderr, "Falha ao inicializa gerador_clock.\n");
    }

    al_register_event_source(queue_timer, al_get_timer_event_source(gerador_clock));
    al_start_timer(gerador_clock);

    while (!fim) 
    {
        if (lock_on_stop()) 
        {
            al_flush_event_queue(queue_timer);
            continue;
        }

        al_wait_for_event(queue_timer, NULL);

        if (!al_is_event_queue_empty(queue_timer)) 
        {
            al_get_next_event(queue_timer, &event);

            if (event.type == ALLEGRO_EVENT_TIMER) 
            {

                slot_score++;
                slot_colision++;


                if (slot_colision >= 10 && houveColisao())
                {
                 
                    slot_colision = 0;
                    slot_score = 0;
                    colision = true;
                    
                    lifes--;

                    if (lifes < 0)
                        lifes = 10;

                    desconta_score_por_colisao();

                    
                    al_stop_timer(gerador_clock);
                    
                    //al_rest(INTERVAL_COLISION);

                    al_flush_event_queue(queue_timer);
                    
                    al_start_timer(gerador_clock);
                    
                }

                else {
                    acrescimo_distancia = (velocidade * INTERVAL_CLOCK);
                    distance += acrescimo_distancia;
                    
                    if (slot_score >= 5) 
                    {
                        slot_score = 0;
                        score++;
                    }
                }
            }
        }
    }

    al_destroy_timer(gerador_clock);
    al_destroy_event_queue(queue_timer);
    printf("saiu timer.\n");

    return NULL;
}