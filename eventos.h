#ifndef EVENTOS_H_INCLUDED
#define EVENTOS_H_INCLUDED

#include <stdbool.h>
#include "barco.h"

#define INTERVAL_CLOCK 0.01
#define INTERVAL_COLISION 0.12
#define TOTAL_LIFES 10
#define METRO_FINAL 350

#define FPS_SLOW 40
#define FPS_FAST 150

enum opt_end {
    winner=0, loser, closing
};

ALLEGRO_THREAD* thr_eventos;
ALLEGRO_THREAD* thr_timer;
ALLEGRO_TIMER *gerador_clock;
ALLEGRO_MUTEX *mutex;
ALLEGRO_COND *cond;

enum opt_end venceu;
short lifes;
bool freia, stop, fim, colision, finca_bandeira, start;
long int score;
double distance;
int fps;


void* thread_eventos(ALLEGRO_THREAD* thread, void* arg);
void* thread_timer(ALLEGRO_THREAD* thread, void* arg);

bool render_lock_on_frame_initial();
bool lock_on_frame_initial();
bool render_lock_on_stop();
bool lock_on_stop();
bool render_barco_wait_on_colision();

void trata_evento_teclado_setas();
void inicializa_valores_eventos();

#endif // EVENTOS_H_INCLUDED