#include "utils.h"
#include "render.h"
#include "ambiente.h"
#include "eventos.h"
#include "barco.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


void finaliza_jogo();
void inicializa_jogo();
void inicializa_allegro();
void play();


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

void play()
{
    while (!fim)
    {
        render_cenario_frame();
        render_placar(score, lifes, distance);
        render_barco(colision);
        
        if (colision) {
            colision = false;
            al_flip_display();
            al_rest(INTERVAL_COLISION);
        }
        
        if (!render_pause_lock_on_stop()) {
            al_flip_display();
        }
        
        trata_evento_tecla_direcao();
        atualizaRio(head);
    
        //al_rest(0.012);
    }

    printf("saiu1\n");
}

void inicializa_jogo () {
    head = geraRio();
    inicializa_configuracao_barco();
    mutex = al_create_mutex();
    cond = al_create_cond();
    
    //printf("Criando e rodando a thread eventos ......\n");

    thr_eventos = al_create_thread(thread_eventos, NULL);
    thr_timer = al_create_thread(thread_timer, NULL);

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
