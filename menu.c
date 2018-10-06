#include "menu.h"


#define COR_TELA (al_map_rgb(255, 255, 255))
//#define COR_TELA (al_map_rgb(51, 153, 255))
#define COR_TEXT_BUTTONS (al_map_rgb(70, 200, 70))
#define COR_TEXT_MOUSE (al_map_rgb(0, 0, 0))
#define COR_TEXT_INSTRUCTIONS (al_map_rgb(0, 0, 0))


void menu_inicial() {
    ALLEGRO_EVENT event;
    ALLEGRO_EVENT_QUEUE* fila = NULL;
    fila = al_create_event_queue();
    al_register_event_source(fila, al_get_display_event_source(screen));
    // Dizemos que vamos tratar os eventos vindos do mouse
    al_register_event_source(fila, al_get_mouse_event_source());

    float coord[4] = {DISPLAY_WEIGHT/4.0, 300, 3.0 * DISPLAY_WEIGHT/4.0, 490};
    
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
        al_draw_scaled_bitmap(img_menu_inicial, 0, 0, 392, 365, 0, 0, 392, 365, 0);
        al_draw_text(fnt_river, COR_TEXT_MOUSE, DISPLAY_WEIGHT/2, DISPLAY_HIGHT/3, ALLEGRO_ALIGN_CENTRE, "R I V E R");

        if (botoes[0])
            al_draw_text(fnt, COR_TEXT_MOUSE, (coord[0]+coord[2])/2, coord[1] + 20, ALLEGRO_ALIGN_CENTRE, "PLAY");
        else
            al_draw_text(fnt, COR_TEXT_BUTTONS, (coord[0]+coord[2])/2, coord[1] + 20, ALLEGRO_ALIGN_CENTRE, "PLAY");

        if (botoes[1])
            al_draw_text(fnt, COR_TEXT_MOUSE, (coord[0]+coord[2])/2, coord[1] + 80, ALLEGRO_ALIGN_CENTRE, "INSTRUCTIONS");
        else 
            al_draw_text(fnt, COR_TEXT_BUTTONS, (coord[0]+coord[2])/2, coord[1] + 80, ALLEGRO_ALIGN_CENTRE, "INSTRUCTIONS");

        if (botoes[2])
            al_draw_text(fnt, COR_TEXT_MOUSE, (coord[0]+coord[2])/2, coord[1] + 140, ALLEGRO_ALIGN_CENTRE, "QUIT");
        else
            al_draw_text(fnt, COR_TEXT_BUTTONS, (coord[0]+coord[2])/2, coord[1] + 140, ALLEGRO_ALIGN_CENTRE, "QUIT");

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


void menu_instrucoes() {
    ALLEGRO_EVENT event;
    ALLEGRO_EVENT_QUEUE* fila = NULL;

    fila = al_create_event_queue();
    al_register_event_source(fila, al_get_display_event_source(screen));
    // Dizemos que vamos tratar os eventos vindos do mouse
    al_register_event_source(fila, al_get_mouse_event_source());
    /*  
    const char* msg1 = "Controle a canoa usando as teclas de direção.";
    const char* msg2 = "Cuidado com os obstáculos, para não perder pontos";
    const char* msg3 = "e esgotar suas vidas.";
    const char* msg4 = "Chegue ao destino sinalizado com uma bandeira.";
  
    al_draw_text(fnt_texto, COR_TEXT_INSTRUCTIONS, DISPLAY_WEIGHT/40, DISPLAY_HIGHT/6, ALLEGRO_ALIGN_LEFT, msg1);    
    al_draw_text(fnt_texto, COR_TEXT_INSTRUCTIONS, DISPLAY_WEIGHT/40, DISPLAY_HIGHT/6 + 25, ALLEGRO_ALIGN_LEFT, msg2);    
    al_draw_text(fnt_texto, COR_TEXT_INSTRUCTIONS, DISPLAY_WEIGHT/40, DISPLAY_HIGHT/6 + 50, ALLEGRO_ALIGN_LEFT, msg3);    
    al_draw_text(fnt_texto, COR_TEXT_INSTRUCTIONS, DISPLAY_WEIGHT/40, DISPLAY_HIGHT/6 + 75, ALLEGRO_ALIGN_LEFT, msg4);    
    */

    al_clear_to_color(COR_TELA);
    al_draw_bitmap(img_about, 0, 0, 0);

    //al_draw_scaled_bitmap(img_game_over, 0, 0, 419, 605, 0, 0, 400, 500, 0);
    
    //al_clear_to_color(COR_TELA);
    //al_draw_bitmap(img_winner, 0, 0, 0);
    
    float x, y;
    float coord_botao[4] = {DISPLAY_WEIGHT/9.0, 7 * DISPLAY_HIGHT/8.0, DISPLAY_WEIGHT/9.0 + 140, 7 * DISPLAY_HIGHT/8.0 + 30};
    float coord[4] = {coord_botao[0]-30, coord_botao[1]-60, coord_botao[2]+30, coord_botao[3]+20};

    while(1)
    {
        al_draw_filled_rectangle(coord_botao[0], coord_botao[1], coord_botao[2], coord_botao[3], COR_TELA);
        al_draw_text(fnt, COR_TEXT_BUTTONS, coord_botao[0], coord_botao[1], ALLEGRO_ALIGN_LEFT, "GO BACK");

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
