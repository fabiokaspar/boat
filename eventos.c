#include "eventos.h"
#include <stdio.h>


bool pressed_keys[3] = {false, false, false};

enum MYKEYS {
    KEY_LEFT=0, KEY_RIGHT, KEY_UP
};

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