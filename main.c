#include "ambiente.h"
#include "queue.h"
#include "config.h"
#include "pixel.h"
#include "random.h"
#include "utils.h"
#include "desenho.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>


#define FPS 120


typedef enum op
{
    HOWPLAY = 1, PLAY, EXIT
} OPCAO;


int menu();
void play();
int has_colision_boat(Node *head, int Xb, int Yb, int Xm, int Ym, int Xc, int Yc);
void PercorreFilaImagens(SCREEN* s);


ALLEGRO_EVENT_QUEUE* CriaFilaEventos();
void InicializaAllegro();
ALLEGRO_FONT* CarregaFonte(SCREEN* s, const char* way, int flag1, int flag2);
void CarregaImagem(SCREEN* screen, char* caminho_img);
void FinalizaJanela(SCREEN* screen);
int ListenerEvent();
void RegistraEventos(SCREEN* s, ALLEGRO_EVENT_QUEUE* fila);
void CriaJanela(SCREEN* screen, int dim_x, int dim_y);


ALLEGRO_FONT* fnt = NULL;
ALLEGRO_EVENT_QUEUE* fila = NULL;
ALLEGRO_EVENT event;

int x, y;
float angle;
bool stop;
int cont = 0;
int repeatstop;
int state;

int main()
{
    InicializaAllegro();

    while(1)
    {
        int opt = menu();

        switch(opt)
        {
            case HOWPLAY:


                break;

            case PLAY:

                play();

                break;

            case EXIT:

                al_destroy_font(fnt);

                return -1;
        }
    }

    return 0;
}



int menu()
{
    int seta = 1;
    ALLEGRO_EVENT e;

    SCREEN* s = MallocSafe(sizeof(SCREEN));
    ALLEGRO_EVENT_QUEUE* fila_menu = CriaFilaEventos();

    CriaJanela(s, 480, 540);

    CarregaImagem(s, "images/inicio.png");
    al_set_window_title(s->scr, "Homem Ao Mar");

    fnt = CarregaFonte(s, "files/FreeSerif.ttf", 25, 0);

    RegistraEventos(s, fila_menu);
    bool flag = true;
    int repeat = 0;
    state = 0;

    do
    {
        int y;

        al_draw_bitmap(s->img, 0, 0, 0);

        al_draw_text(fnt, al_map_rgb(255, 100, 0), 410, 300, ALLEGRO_ALIGN_CENTRE, "Como Jogar");
        al_draw_text(fnt, al_map_rgb(255, 100, 0), 380, 340, ALLEGRO_ALIGN_CENTRE, "Jogar");
        al_draw_text(fnt, al_map_rgb(255, 100, 0), 370, 380, ALLEGRO_ALIGN_CENTRE, "Sair");

        if(seta == 1)
        {
            y = 300;
        }

        if(seta == 2)
        {
            y = 340;
        }

        if(seta == 3)
        {
            y = 380;
        }

        al_draw_text(fnt, al_map_rgb(255, 100, 0), 335, y, ALLEGRO_ALIGN_CENTRE, ">");
        al_flip_display();

        if(al_get_next_event(fila_menu, &e))
        {
            if(e.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                exit(0);

            if(!e.keyboard.repeat)
            {
                repeat++;

                if(repeat == 1)
                {
                    switch(e.keyboard.keycode)
                    {
                        case ALLEGRO_KEY_UP:

                             if(seta > 1)
                               seta--;

                             break;

                        case ALLEGRO_KEY_DOWN:

                             if(seta < 3)
                               seta++;

                             break;

                        case ALLEGRO_KEY_ENTER:

                             flag = false;

                             break;
                    }
                }

                if(repeat == 3)
                   repeat = 0;
            }
        }

    } while(flag);

    FinalizaJanela(s);
    al_destroy_event_queue(fila_menu);
    state++;

    return seta;
}

void play()
{
    x = 200, y = 430;
    angle = 0;
    stop = false;
    repeatstop = 0;
    RGB cor = {0, 127, 255};

    SCREEN* s = MallocSafe(sizeof(SCREEN));

    CriaJanela(s, DISPLAY_WEIGHT, DISPLAY_HIGHT);
    al_set_window_title(s->scr, "Homem Ao Mar");
    CarregaImagem(s, "files/canoa.png");

    float w = al_get_bitmap_width(s->img);
    float h = al_get_bitmap_height(s->img);

    fila = CriaFilaEventos();
    RegistraEventos(s, fila);
    fnt = CarregaFonte(s, "files/FreeSerif.ttf", 50, 0);

    Node* head = geraRio();
    int rev = 0;


    while(1)
    {
        if(!stop)
        {
            RGB_SCREEN(cor);
            DesenhaRio(head);
            al_draw_rotated_bitmap(s->img, w/2, h/2, x, y, angle, 0);
            PercorreFilaImagens(s);

            if(rev != 1)
            {
                if(angle > 0)
                {
                    angle -= ALLEGRO_PI/18;

                    if(angle < 0)
                        angle = 0;
                }

                if(angle < 0)
                {
                    angle += ALLEGRO_PI/18;

                    if(angle > 0)
                        angle = 0;
                }
            }

            al_flip_display();
            atualizaRio(head);
        }

        rev = ListenerEvent();

        if(rev == -1)
            break;
    }

    al_destroy_event_queue(fila);
    DesalocaAmbiente(head);
    FinalizaJanela(s);
    free(s);
}


int ListenerEvent()
{
    if (al_get_next_event(fila, &event))
    {
        if(state == 1)
        {
            state = 0;
            al_flush_event_queue(fila);
            return 0;
        }

        if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            return -1;

        switch (event.keyboard.keycode)
        {
            case ALLEGRO_KEY_LEFT:

                    if(!stop)
                    {
                        x -= 10;

                        if(angle > -ALLEGRO_PI/9)
                        {
                            // 10 graus
                            angle -= ALLEGRO_PI/18;
                        }

                        return 1;
                    }
                    break;

            case  ALLEGRO_KEY_RIGHT:

                    if(!stop)
                    {
                        x += 10;

                        if(angle < ALLEGRO_PI/9)
                        {
                            angle += ALLEGRO_PI/18;
                        }

                        return 1;
                    }
                    break;

            case ALLEGRO_KEY_ENTER:

                if(!event.keyboard.repeat)
                {
                    repeatstop++;

                    if(repeatstop == 3)
                        repeatstop = 0;

                    if(repeatstop == 1)
                    {
                        stop = !stop;

                        if(stop)
                        {
                            //printf("pause\n");
                            al_draw_text(fnt, al_map_rgb(255, 100, 0), 240, 250, ALLEGRO_ALIGN_CENTRE, "PAUSE");
                            al_flip_display();
                            return 2;
                        }

                        else
                        {
                            //printf("continue\n");
                            return 3;
                        }
                    }
                }
                break;

                default: break;
        }
    }

    return 0;
}


int has_colision_boat(Node *head, int Xb, int Yb, int Xm, int Ym, int Xc, int Yc)
{
    Node* p = head;
    int res, i, index;
    int dist_x = (Xc - Xm) * 2;
    int nrows = getNumberLines();
    int bloco = (int) DISPLAY_HIGHT / nrows;

    res = Yc;

    for(i = 0; i < (int)res / bloco; i++)
        p = p->prox;

    index = (int) Xc / 6;

    if(get_type_pixel(&(p->v[index])) == '#')
        return 1;

    res += bloco;
    p = p->prox;

    while(res < Ym)
    {
        int coord_x = (int)(Xm + (Ym - res)*(Xc - Xm) / (Ym - Yc))/ 6;

        if(get_type_pixel(&(p->v[coord_x])) == '#')
            return 1;

        coord_x = (int) (Xm + dist_x - (Ym - res) * (Xm + dist_x - Xc)/(Ym - Yc)) / 6;

        if(get_type_pixel(&(p->v[coord_x])) == '#')
            return 1;

        res += bloco;
        p = p->prox;
    }

    res = Ym;

    while(res <= Yb)
    {
        index = (int) Xm/6;

        if(get_type_pixel(&(p->v[index])) == '#')
            return 1;

        index = (int)(Xm + dist_x)/6;

        if(get_type_pixel(&(p->v[index])) == '#')
            return 1;

        res += bloco;
        p = p->prox;
    }

    return 0;
}




/************************************* Ambiente Gráfico *************************************************/

/* http://www.inf.pucrs.br/~manssour/Allegro/#eventos */

void PercorreFilaImagens(SCREEN* s)
{
    QueueNode* pt;
    ALLEGRO_BITMAP* img = s->img;

    for(pt = inicio; pt != NULL; pt = pt->prox)
    {
        switch(pt->img.foto)
        {
            case 0: CarregaImagem(s, "images/peixe.png");

            case 1: CarregaImagem(s, "images/cabana.png");

            default: break;
        }

        al_draw_bitmap(s->img, pt->img.p.x, pt->img.p.y, 0);
    }

    s->img = img;
}


ALLEGRO_EVENT_QUEUE* CriaFilaEventos()
{
    fila = al_create_event_queue();

    if(fila == NULL)
    {
        fprintf(stderr, "Não criou a fila de eventos!\n");
        return ;
    }

    return fila;
}

void RegistraEventos(SCREEN* s, ALLEGRO_EVENT_QUEUE* fila)
{
    al_register_event_source(fila, al_get_display_event_source(s->scr));
    al_register_event_source(fila, al_get_keyboard_event_source());
}


ALLEGRO_FONT* CarregaFonte(SCREEN* s, const char* way, int flag1, int flag2)
{
    fnt = NULL;
    fnt = al_load_font(way, flag1, flag2);

    if (!fnt)
    {
        fprintf(stderr, "Falha ao carregar fonte.\n");
        exit(0);
    }

    return fnt;
}

void CarregaImagem(SCREEN* screen, char* caminho_img)
{
    ALLEGRO_BITMAP* img = al_load_bitmap(caminho_img);
    screen->img = img;
}

void CriaJanela(SCREEN* screen, int dim_x, int dim_y)
{
    screen->scr = al_create_display(dim_x, dim_y);
    al_set_window_position(screen->scr, 400, 50);

    ALLEGRO_BITMAP* img = al_load_bitmap("images/cabana.png");
    al_set_display_icon(screen->scr, img);

    if(!screen->scr)
    {
        fprintf(stderr, "Falha ao criar a janela\n");
        return ;
    }
}

void FinalizaJanela(SCREEN* screen)
{
    al_destroy_bitmap(screen->img);
    al_destroy_display(screen->scr);
}

void InicializaAllegro()
{
    if(!al_init())
    {
        fprintf(stderr, "Falha ao inicializar a Allegro 5\n");
        return ;
    }

    if(!al_init_image_addon())
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

    al_init_font_addon();

    if (!al_init_ttf_addon())
    {
        fprintf(stderr, "Allegro_ttf já inicializada!\n");
        return ;
    }
}



