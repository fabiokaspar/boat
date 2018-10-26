#include "utils.h"
#include "render.h"
#include "ambiente.h"
#include "eventos.h"
#include "barco.h"
#include "menu.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>


void finaliza_jogo();
void inicializa_jogo();
void inicializa_allegro();
void new_stream_audio();
void destroy_stream_audio();
void play();
void wait_for_rate_frame();

double inicio_contagem;


int main()
{
    inicializa_allegro();
    inicializa_janela();
    inicializa_fontes();

    al_set_target_backbuffer(screen);
        
    while (1) 
    {    
        new_stream_audio();
        tela_inicial();
        play();
        
        if (venceu != closing) 
        {
            tela_final(venceu);
        }
        
        destroy_stream_audio();
    }

    desaloca_janela();
    
    return 0;
}

void new_stream_audio() {
    music_abertura = al_load_audio_stream("musics/music_abertura.ogg", 4, 1024);
    
    if (!music_abertura) 
    {
        fprintf(stderr, "Falha ao carregar audio.\n");
    }
    
    if (!al_set_audio_stream_playmode(music_abertura, ALLEGRO_PLAYMODE_LOOP)) 
    {
        fprintf(stderr, "Falha ao definir play mode loop.\n");
    }

    al_attach_audio_stream_to_mixer(music_abertura, al_get_default_mixer());
    al_set_audio_stream_playing(music_abertura, true);
}

void destroy_stream_audio() {
    al_destroy_audio_stream(music_abertura);
}


double getTimeSinceBegin() {
    return (al_get_time() - inicio_contagem);
}

void wait_for_rate_frame() {
    if (getTimeSinceBegin() < (1.0/fps)) {
        double tempo_espera = (1.0/fps) - getTimeSinceBegin();
        al_rest(tempo_espera);
    }
}

void play()
{
    inicializa_jogo();
    
    //struct timeval tempo;

    //printf("inicia cronometro:\n");
    head = geraRio();

    render_lock_on_frame_initial();

    //gettimeofday(&tempo, NULL);

    while (!fim)
    {
        inicio_contagem = al_get_time();
        
        atualizaRio(head);
        trata_evento_teclado_setas();

        render_cenario_frame();
        render_placar(score, lifes, distance);
        
        render_barco(colision);
        render_lock_on_colision(); 

        render_pause_lock_on_stop();
        al_flip_display();

        wait_for_rate_frame();
    }

    //al_set_audio_stream_playing(music_abertura, false);

    //printf("--> %f seg\n", relogio(tempo));
    Queue_Free_All (head);
    finaliza_jogo();
}

void inicializa_jogo () {
    inicializa_configuracao_barco();
    inicializa_valores_eventos();
    thr_eventos = al_create_thread(thread_eventos, NULL);
    thr_timer = al_create_thread(thread_timer, NULL);

    al_start_thread(thr_eventos);
    al_start_thread(thr_timer);
}

void finaliza_jogo() {
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

    if (!al_install_audio())
    {
        fprintf(stderr, "Falha ao inicializar áudio.\n");
        return ;
    }
 
    if (!al_init_acodec_addon())
    {
        fprintf(stderr, "Falha ao inicializar codecs de áudio.\n");
        return ;
    }
 
    if (!al_reserve_samples(1))
    {
        fprintf(stderr, "Falha ao alocar canais de áudio.\n");
        return ;
    }

    al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
}
