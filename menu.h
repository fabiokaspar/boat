#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <stdbool.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "utils.h"
#include "render.h"
#include "eventos.h"

ALLEGRO_AUDIO_STREAM *music_abertura;

void tela_inicial();
void tela_final(enum opt_end venceu);

#endif // MENU_H_INCLUDED