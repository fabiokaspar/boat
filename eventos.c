#include "eventos.h"
//#include "menu.h"
#include <stdio.h>
#include <stdbool.h>


bool pressed_keys[3] = {false, false, false};

enum MYKEYS {
    KEY_LEFT=0, KEY_RIGHT, KEY_UP
};

static void reset_pressed_keys();
static void trata_evento_teclado_velocidade();
static void trata_evento_teclado_direcao();
static void inicializa_valores();
static void debita_score();
static void credita_score(const float vel_atual);
static bool percore_distancia(const float vel_atual, const float delta_time);
static bool debita_vidas();


void inicializa_valores_eventos() {
    fim = false;
    freia = false;
    stop = false;
    start = false;
    colision = false;
    finca_bandeira = false;
    distance = 0;
    score = 0;
    fps = FPS_SLOW;
    venceu = closing;
    lifes = TOTAL_LIFES;
    gerador_clock = al_create_timer(INTERVAL_CLOCK);

    mutex = al_create_mutex();
    cond = al_create_cond();
    reset_pressed_keys();
    
    //printf("Criando e rodando a thread eventos ......\n");
}

void trata_evento_teclado_setas() {
    trata_evento_teclado_velocidade();
    trata_evento_teclado_direcao();
}

static void trata_evento_teclado_velocidade() {
    if (pressed_keys[KEY_UP]) {
        velocidade = VEL_MAX;
        fps = FPS_FAST;
    }
    else {
        velocidade = VEL_MIN;
        fps = FPS_SLOW;
    }
}

void* thread_timer(ALLEGRO_THREAD* thread, void* arg) {
    al_set_target_backbuffer(NULL);

    short check_colision = 0, check_score = 0;
    
    ALLEGRO_EVENT event;
    ALLEGRO_EVENT_QUEUE* queue_timer = NULL;

    queue_timer = al_create_event_queue();
    
    if (!queue_timer)
    {
        fprintf(stderr, "Não criou a fila de eventos queue_timer!\n");
        exit(0);
    }

    al_register_event_source(queue_timer, al_get_timer_event_source(gerador_clock));
    al_start_timer(gerador_clock);

    while (!fim) 
    {
        if (lock_on_stop()) 
        {
            // desconsidera eventos do relogio enquanto pausado
            al_flush_event_queue(queue_timer);
            continue;
        }

        al_wait_for_event(queue_timer, NULL);

        if (!al_is_event_queue_empty(queue_timer)) 
        {
            al_get_next_event(queue_timer, &event);

            if (event.type == ALLEGRO_EVENT_TIMER) 
            {
                check_score++;
                check_colision++;

                if (check_colision > 30 && houveColisao())
                {   
                    //al_play_sample(sample_crash, 3.0, 0.0, 6.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    debita_score();

                    if (venceu != winner && debita_vidas())
                        break;
                    
                    check_colision = 0;
                    //check_score = 0;                    
                }
                else if (check_score > 100 && !colision)
                {
                    const float vel_atual = velocidade;
                    const short count_clock = check_score;
                    
                    if (check_colision >= 30)
                        credita_score(vel_atual);

                    percore_distancia(vel_atual, INTERVAL_CLOCK * count_clock);

                    check_score = 0;
                }                
            }
        }
    }

    al_destroy_timer(gerador_clock);
    al_destroy_event_queue(queue_timer);
    //printf("saiu timer.\n");

    return NULL;
}

static void debita_score() {
    if (score > 0) 
    {
        if (score < 10)
            score = 0;

        else score -= 10;
    }
}

static void credita_score(const float vel_atual) {
    if (vel_atual == VEL_MIN)
    {
        score++;
    }
    else 
    {
        score += 5;
    }
}

// retorna se chegou ao metro final
static bool percore_distancia(const float vel_atual, const float delta_time) {
    double acrescimo_distancia = (vel_atual * delta_time);
    distance += acrescimo_distancia;

    if (distance >= METRO_FINAL) 
    {
        //fim = true;
        //venceu = winner; 

        return true;
    }

    return false;
}

// retorna true se vidas se esgotaram
static bool debita_vidas() {
    colision = true;
    lifes--;
    
    if (lifes == 0) 
    {
        venceu = loser;
        fim = true;
    
        return true;
    }

    return false;
}

static void trata_evento_teclado_direcao() {
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

    while (!fim) 
    {
        al_wait_for_event_timed(fila, NULL, 1.5);

        if (!al_is_event_queue_empty(fila)) 
        {
            al_get_next_event(fila, &event);

            if (!start && event.type == ALLEGRO_EVENT_KEY_DOWN) 
            {
                start = true;
                al_signal_cond(cond);
                continue;
            }

            else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                fim = true;
                al_broadcast_cond(cond);
                break;
            }

            //se start e se tecla ENTER foi pressionada 
            else if (event.type == ALLEGRO_EVENT_KEY_DOWN &&
                event.keyboard.keycode == ALLEGRO_KEY_ENTER) 
            {                
                stop = !stop;
                reset_pressed_keys();

                if (!stop) 
                {
                    al_broadcast_cond(cond);
                }
            }

            else if (!stop) 
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
    //printf("saiu ev.\n");

    return NULL;
}

static void reset_pressed_keys() {
    pressed_keys[KEY_LEFT] = false;
    pressed_keys[KEY_RIGHT] = false;
    pressed_keys[KEY_UP] = false;
}

bool render_lock_on_colision() {    
    if (colision) {
        al_flip_display();
        al_rest(INTERVAL_COLISION);
        colision = false;
        
        return true;
    }
    
    return false;
}

void render_lock_on_frame_initial() {
    render_frame_initial();
    al_flip_display();
    
    if (!start) 
    {
        al_lock_mutex(mutex);

        //while (!start)
        al_wait_cond(cond, mutex);    
        
        al_unlock_mutex(mutex);
    }
}

// retorna true se pausou
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