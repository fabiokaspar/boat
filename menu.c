#include "menu.h"
#include "utils.h"
#include "render.h"

#define COR_TELA (al_map_rgb(51, 153, 255))
#define COR_TEXT (al_map_rgb(255,255,255))
#define COR_TEXT_MOUSE (al_map_rgb(0,0,0))

void menu_instrucoes() {
    ALLEGRO_EVENT event;
    ALLEGRO_EVENT_QUEUE* fila = NULL;

    fila = al_create_event_queue();
    al_register_event_source(fila, al_get_display_event_source(screen));
    // Dizemos que vamos tratar os eventos vindos do mouse
    al_register_event_source(fila, al_get_mouse_event_source());
      
    const char* msg1 = "Controle a canoa usando as teclas de direção para chegar ao destino.";
    const char* msg2 = "Cuidado com os obstáculos.";
    const char* msg3 = "Tecla para cima acelera a canoa.";
    const char* msg4 = "Tecla Enter pausa o jogo.";

    al_clear_to_color(COR_TELA);    
    al_draw_text(fnt_texto, COR_TEXT, DISPLAY_WEIGHT/40, DISPLAY_HIGHT/10, ALLEGRO_ALIGN_LEFT, msg1);    
    al_draw_text(fnt_texto, COR_TEXT, DISPLAY_WEIGHT/40, DISPLAY_HIGHT/10 + 25, ALLEGRO_ALIGN_LEFT, msg2);    
    al_draw_text(fnt_texto, COR_TEXT, DISPLAY_WEIGHT/40, DISPLAY_HIGHT/10 + 50, ALLEGRO_ALIGN_LEFT, msg3);    
    al_draw_text(fnt_texto, COR_TEXT, DISPLAY_WEIGHT/40, DISPLAY_HIGHT/10 + 75, ALLEGRO_ALIGN_LEFT, msg4);
    
    float x, y;
    float coord_botao[4] = {DISPLAY_WEIGHT/9.0, 7 * DISPLAY_HIGHT/8.0, DISPLAY_WEIGHT/9.0 + 140, 7 * DISPLAY_HIGHT/8.0 + 30};
    float coord[4] = {coord_botao[0]-30, coord_botao[1]-60, coord_botao[2]+30, coord_botao[3]+20};

    while(1)
    {
        al_draw_filled_rectangle(coord_botao[0], coord_botao[1], coord_botao[2], coord_botao[3], COR_TELA);
        al_draw_text(fnt, COR_TEXT, coord_botao[0], coord_botao[1], ALLEGRO_ALIGN_LEFT, "GO BACK");

        if (!al_is_event_queue_empty(fila)) 
        {
            al_get_next_event(fila, &event);

            x = event.mouse.x;
            y = event.mouse.y;

            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                exit(0);
    
            //al_draw_text(fnt, COR_TEXT, coord_botao[0], coord_botao[1], ALLEGRO_ALIGN_LEFT, "GO BACK");

            if (esta_contido_em(coord_botao, x, y)) 
            {

                al_draw_filled_rectangle(coord_botao[0], coord_botao[1], coord_botao[2], coord_botao[3], COR_TELA);
                al_draw_text(fnt, COR_TEXT_MOUSE, coord_botao[0], coord_botao[1], ALLEGRO_ALIGN_LEFT, "GO BACK");

                if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
                    //al_flush_event_queue(fila);
                    break;        
                }
            }            
        }
        
        al_flip_display();

        al_wait_for_event(fila, NULL);
    }
    
    al_destroy_event_queue(fila);
}

void menu_inicial() {
    ALLEGRO_EVENT event;
    ALLEGRO_EVENT_QUEUE* fila = NULL;
    fila = al_create_event_queue();
    al_register_event_source(fila, al_get_display_event_source(screen));
    // Dizemos que vamos tratar os eventos vindos do mouse
    al_register_event_source(fila, al_get_mouse_event_source());

    float coord[4] = {DISPLAY_WEIGHT/4.0, DISPLAY_HIGHT/2, 3.0 * DISPLAY_WEIGHT/4.0,  7.0 * DISPLAY_HIGHT/8.0};
    
    float coord_play[4] = {(coord[0]+coord[2])/2 - 40, coord[1] + 20, (coord[0]+coord[2])/2 + 40, coord[1] + 40};
    float coord_instr[4] = {(coord[0]+coord[2])/2 - 130, coord[1] + 80, (coord[0]+coord[2])/2 + 130, coord[1] + 100};
    float coord_quit[4] = {(coord[0]+coord[2])/2 - 40, coord[1] + 140, (coord[0]+coord[2])/2 + 40, coord[1] + 160};

    float x, y;
    int botoes[3] = {0,0,0};
    bool espera = true;
    
    while (1) 
    {
        //if (botoes[0] || botoes[1] || botoes[2]) {}
        espera = true;
        int xp, yp;
        int xw, yw;
        al_get_window_position(screen, &xw, &yw);
        
        if (al_get_mouse_cursor_position(&xp, &yp)) {
            x = (float)(xp) - xw;
            y = (float)(yp) - yw; 

            if (esta_contido_em(coord_play, x, y)) 
            {
                botoes[0] = 1;
                botoes[1] = botoes[2] = 0;
            }
            else if (esta_contido_em(coord_instr, x, y)) {
                botoes[1] = 1;
                botoes[0] = botoes[2] = 0;
            }
            else if (esta_contido_em(coord_quit, x, y)) {
                botoes[2] = 1;
                botoes[0] = botoes[1] = 0;
            }
            else { 
                botoes[0] = botoes[1] = botoes[2] = 0;
            }
        }

        al_clear_to_color(COR_TELA);
        al_draw_text(fnt_river, COR_TEXT, DISPLAY_WEIGHT/2, DISPLAY_HIGHT/4, ALLEGRO_ALIGN_CENTRE, "R I V E R");

        if (botoes[0])
            al_draw_text(fnt, COR_TEXT_MOUSE, (coord[0]+coord[2])/2, coord[1] + 20, ALLEGRO_ALIGN_CENTRE, "PLAY");
        else
            al_draw_text(fnt, COR_TEXT, (coord[0]+coord[2])/2, coord[1] + 20, ALLEGRO_ALIGN_CENTRE, "PLAY");

        if (botoes[1])
            al_draw_text(fnt, COR_TEXT_MOUSE, (coord[0]+coord[2])/2, coord[1] + 80, ALLEGRO_ALIGN_CENTRE, "INSTRUCTIONS");
        else 
            al_draw_text(fnt, COR_TEXT, (coord[0]+coord[2])/2, coord[1] + 80, ALLEGRO_ALIGN_CENTRE, "INSTRUCTIONS");

        if (botoes[2])
            al_draw_text(fnt, COR_TEXT_MOUSE, (coord[0]+coord[2])/2, coord[1] + 140, ALLEGRO_ALIGN_CENTRE, "QUIT");
        else
            al_draw_text(fnt, COR_TEXT, (coord[0]+coord[2])/2, coord[1] + 140, ALLEGRO_ALIGN_CENTRE, "QUIT");

        al_flip_display();

        if (!al_is_event_queue_empty(fila))
        {
            al_get_next_event(fila, &event);

            x = event.mouse.x;
            y = event.mouse.y;
            
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                exit(0);
            }

            if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) 
            {    
                if (esta_contido_em(coord_play, x, y)) {
                    break;
                } 
                else if (esta_contido_em(coord_instr, x, y)) { 
                    menu_instrucoes();
                    al_flush_event_queue(fila);
                    espera = false;                    
                }
                else if (esta_contido_em(coord_quit, x, y)) 
                {                
                    exit(0);                    
                }
            }
        }

        if (espera)
            al_wait_for_event(fila, NULL);
    }

    al_destroy_event_queue(fila);
}
