#ifndef EVENTOS_H_INCLUDED
#define EVENTOS_H_INCLUDED

#include <stdbool.h>
#include "render.h"
#include "barco.h"

#define INTERVAL_CLOCK 0.03
#define INTERVAL_COLISION 0.12


ALLEGRO_THREAD* thr_eventos;
ALLEGRO_THREAD* thr_timer;
ALLEGRO_TIMER *gerador_clock;
ALLEGRO_MUTEX *mutex;
ALLEGRO_COND *cond;



void* thread_eventos(ALLEGRO_THREAD* thread, void* arg);
void* thread_timer(ALLEGRO_THREAD* thread, void* arg);
void trata_evento_tecla_direcao();
bool render_pause_lock_on_stop();
bool lock_on_stop();

#endif // EVENTOS_H_INCLUDED