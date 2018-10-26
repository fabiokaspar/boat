#include "menu.h"
#include <stdio.h>
#include <stdbool.h>


static void tela_instrucoes();
static short cursor_aponta_botaox_tela_inicial();
static bool cursor_aponta_botao_tela_instr();


static short cursor_aponta_botaox_tela_inicial() {
    float coord[4] = {DISPLAY_WEIGHT/4.0, 300, 3.0 * DISPLAY_WEIGHT/4.0, 490};
    float coord_play[4] = {(coord[0]+coord[2])/2 - 40, coord[1] + 20, (coord[0]+coord[2])/2 + 40, coord[1] + 40};
    float coord_instr[4] = {(coord[0]+coord[2])/2 - 130, coord[1] + 80, (coord[0]+coord[2])/2 + 130, coord[1] + 100};
    float coord_quit[4] = {(coord[0]+coord[2])/2 - 40, coord[1] + 140, (coord[0]+coord[2])/2 + 40, coord[1] + 160};

    int xp, yp;
    int xw, yw;
    float x, y;

    al_get_window_position(screen, &xw, &yw);
    
    short botao;

    if (al_get_mouse_cursor_position(&xp, &yp)) 
    {
        x = (float)(xp) - xw;
        y = (float)(yp) - yw; 

        if (esta_contido_em(coord_play, x, y)) 
        {
            botao = bt_play;
        }
        else if (esta_contido_em(coord_instr, x, y)) {
            botao = bt_about;
        }
        else if (esta_contido_em(coord_quit, x, y)) {
            botao = bt_quit;
        }
        else { 
            botao = -1;
        }
    }

    return botao;
}

static bool cursor_aponta_botao_tela_instr() {
    int xp, yp;
    int xw, yw;
    float x, y;
    float coord_botao[4] = {
        10, DISPLAY_HIGHT - 75,
        45, DISPLAY_HIGHT - 40
    };

    al_get_window_position(screen, &xw, &yw);

    static int c = 0;
    if (al_get_mouse_cursor_position(&xp, &yp)) 
    {
        x = (float)(xp) - xw;
        y = (float)(yp) - yw;

        if (esta_contido_em(coord_botao, x, y)) {
            return true;
        }
    }

    return false;
}

static void tela_instrucoes() {
    ALLEGRO_EVENT event;
    ALLEGRO_EVENT_QUEUE* fila = NULL;

    fila = al_create_event_queue();
    al_register_event_source(fila, al_get_display_event_source(screen));

    al_register_event_source(fila, al_get_mouse_event_source());
    al_register_event_source(fila, al_get_keyboard_event_source());

    render_about();
    al_flush_event_queue(fila);

    while (1)
    {    
        al_flip_display();
        al_wait_for_event(fila, NULL);
    
        if (!al_is_event_queue_empty(fila)) 
        {
            al_get_next_event(fila, &event);

            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                exit(0);
    
            else if (event.type == ALLEGRO_EVENT_KEY_DOWN &&
                event.keyboard.keycode == ALLEGRO_KEY_LEFT) 
            {
                break;
            }

            else if (cursor_aponta_botao_tela_instr())
            {
                if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                    break;   
            }
        }

    }
    
    al_destroy_event_queue(fila);
}

void tela_inicial() {
    ALLEGRO_EVENT event;
    ALLEGRO_EVENT_QUEUE* fila = NULL;
    fila = al_create_event_queue();
    al_register_event_source(fila, al_get_display_event_source(screen));
    
    // Dizemos que vamos tratar os eventos vindos do mouse
    al_register_event_source(fila, al_get_mouse_event_source());

    // Dizemos que vamos tratar os eventos vindos do teclado
    al_register_event_source(fila, al_get_keyboard_event_source());
    
    short botao = 0;
    short sair = 0;

    al_flush_event_queue(fila);
    
    while (!sair) 
    {
        render_tela_inicial_fundo();
        render_tela_inicial_botoes(botao);
        al_flip_display();

        al_wait_for_event(fila, NULL);
        
        if (!al_is_event_queue_empty(fila))
        {
            al_get_next_event(fila, &event);
    
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) 
            {
                exit(0);
            }

            else if (event.type == ALLEGRO_EVENT_MOUSE_AXES)
            {
                botao = cursor_aponta_botaox_tela_inicial();
            }

            else if (event.type == ALLEGRO_EVENT_KEY_DOWN) 
            {    
                if (event.keyboard.keycode == ALLEGRO_KEY_UP) 
                {
                    botao--;
                }
                if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) 
                {
                    botao++;
                }
                
                if (botao < 0)
                    botao = 2;
                
                botao %= 3;
            }

            if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP ||
               (event.type == ALLEGRO_EVENT_KEY_DOWN && 
                event.keyboard.keycode == ALLEGRO_KEY_ENTER) ) 
            {
                switch (botao) 
                {
                    case bt_play:
                        al_flush_event_queue(fila);
                        sair = 1;
                        break;
                    
                    case bt_about:
                        tela_instrucoes();
                        al_flush_event_queue(fila);
                        break;

                    case bt_quit:
                        exit(0);
                        break;
                }
            }
        }
    }

    al_destroy_event_queue(fila);
}


void tela_final(enum opt_end venceu) {
    ALLEGRO_EVENT event;
    ALLEGRO_EVENT_QUEUE* fila = NULL;
    
    if (venceu == winner) 
    {
        render_winner();
    }
    else if (venceu == loser) 
    {
        al_set_audio_stream_playing(music_abertura, false);
        render_game_over();
    }
    
    fila = al_create_event_queue();
    
    al_register_event_source(fila, al_get_display_event_source(screen));
    // Dizemos que vamos tratar os eventos vindos do teclado
    al_register_event_source(fila, al_get_keyboard_event_source());
    al_rest(0.5);
    al_flush_event_queue(fila);
    
    while (1)
    {
        al_flip_display();
        al_wait_for_event(fila, NULL);

        if (!al_is_event_queue_empty(fila)) 
        {
            al_get_next_event(fila, &event);

            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                exit(0);

            if (event.type == ALLEGRO_EVENT_KEY_DOWN) 
            {
                if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) 
                {   
                    //printf("entrou ENTER\n"); 
                    break;
                }
                    
                if (event.keyboard.keycode == ALLEGRO_KEY_Q) 
                {
                    exit(0);
                }
            }
        }
        
    }
    
    //printf("saiu ENTER\n"); 
    al_destroy_event_queue(fila);
}

