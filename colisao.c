
#include "ambiente.h"
#include "queue.h"
#include "config.h"
#include "pixel.h"
#include "random.h"
#include "graphic.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

typedef enum op
{
    PLAY = 1, SET_UP, EXIT
} OPCAO;


void menu();
void play();
void jogo(SCREEN* s);
void print_nodes(Node* head);
void show_setup();
int* limite_margem_direita(Node* head);
int* limite_margem_esquerda(Node* head);
int extremantes_margem(Coordenadas* c, int* ptos_terra);
int has_colision_boat(Node *head, int Xb, int Yb, int Xm, int Ym, int Xc, int Yc);



int main()
{
OPCAO opt;
RGB cor = {0, 127, 255};
SCREEN* s;

     s = MallocSafe(sizeof(*s));
     printf("--------------------------------------------------------------------------------\n");
     printf("Game River\n");
     while(1)
     {

            menu();
            scanf("%d", &opt);
            switch(opt)
            {

            case PLAY:  graphic_initial();
                        graphic_set_display(s, cor, 480, 660);

                        /* graphic_set_title_display(s, "Game - River"); */

                        jogo(s);
                        graphic_finalize_display(s);
                          break;

            case SET_UP: show_setup();
                         break;

            case EXIT: return -1;

            }

     }


    return 0;
}


/*  fila em modo texto */
void play()
{
int i;

  Node* head = geraRio();
  Queue_Print(head);


  while(1)
  {

      stop(10);
      system("clear");
      atualizaRio(head);
      Queue_Print(head);

  }

  Queue_Free(head);
}


void menu()
{

    printf("\n1 - PLAY\n");
    printf("2 - SET UP\n");
    printf("3 - EXIT\n");
    printf("Digite uma opcao:");
}




void jogo(SCREEN* s)
{
     ALLEGRO_EVENT_QUEUE *fila_eventos =  NULL;
     int coord_x = 200, coord_y = 400, cont = 0;

     fila_eventos = graphic_create_event_queue(fila_eventos);

     graphic_refresh(s->cor);

     graphic_load_image(s, "images/boat.png");


     Node* head = geraRio();
     print_nodes(head);

     graphic_draw_bitmap(s->img, coord_x, coord_y, 0);
     graphic_update_display();


                while(1)
                {

                                ALLEGRO_EVENT event;
                                ALLEGRO_TIMEOUT time;
                                int tem_evento;

                                /* al_event_queue_is_empty(event_queue) => para lembrar */

                                al_init_timeout(&time, 0.1);
                                al_register_event_source(fila_eventos, al_get_display_event_source(s->scr));
                                al_register_event_source(fila_eventos, al_get_keyboard_event_source());


                                tem_evento = al_wait_for_event_until(fila_eventos, &event, &time);


                                if(tem_evento && event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                                break;


                                if(tem_evento)
                                       do
                                       {

                                                   atualizaRio(head);

                                                   graphic_refresh(s->cor);

                                                   print_nodes(head);

                                                     switch(event.keyboard.keycode)
                                                    {
                                                        case  ALLEGRO_KEY_LEFT: coord_x -= 10;

                                                                                break;

                                                        case  ALLEGRO_KEY_RIGHT: coord_x += 10;

                                                                                break;
                                                    }

                                                    graphic_draw_bitmap(s->img, coord_x, coord_y, 0);
                                                    graphic_update_display();

                                                    if(has_colision_boat(head, coord_x + 5, coord_y + 62, coord_x + 5, coord_y + 20, coord_x + 17, coord_y))
                                                    printf("Colidiu! = %d\n", cont++);

                                                    al_flush_event_queue(fila_eventos);
                                                    al_wait_for_event_until(fila_eventos, &event, &time);

                                          }         while(!al_is_event_queue_empty(fila_eventos) && event.keyboard.repeat);


                                    if(!tem_evento)
                                    {
                                            atualizaRio(head);

                                            graphic_refresh(s->cor);

                                            print_nodes(head);

                                            graphic_draw_bitmap(s->img, coord_x, coord_y, 0);

                                            graphic_update_display();


                                            if(has_colision_boat(head, coord_x + 5, coord_y + 62, coord_x + 5, coord_y + 20, coord_x + 17, coord_y))
                                            printf("Colidiu! = %d\n", cont++);
                                    }
                   }

                    graphic_destroy_event_queue(fila_eventos);

}



void print_nodes(Node* head)
{
int i, k, qtd;
int coord_x, coord_y = 0;
int* lim_esq = limite_margem_esquerda(head);
int* lim_dir = limite_margem_direita(head);

Node* node = head->prox;
Coordenadas* c = MallocSafe(NROWS_DEF * sizeof(*c));

             while(node != head)
             {

                    for(i = 0, coord_x = 0; i < NCOLS_DEF; i++, coord_x += 6)
                    {
                        int margem_valida = (i >= MARGEM_DIR_DEF || i <= MARGEM_ESQ_DEF );
                        int ilha = (MARGEM_ESQ_DEF + INTERVALO_ALEATORIO + 2 <=  i && i < MARGEM_DIR_DEF - INTERVALO_ALEATORIO - 2);

                        if((ilha || margem_valida) && get_type_pixel(&node->v[i]) == '#')
                        al_draw_filled_rectangle(coord_x, coord_y, coord_x + 6, coord_y + 22, al_map_rgb(250, 240, 230));

                    }


                      coord_y = coord_y + 22;
                      node = Queue_prox(node);

            }

          al_draw_line( (MARGEM_ESQ_DEF + INTERVALO_ALEATORIO )*6, 0,  (MARGEM_ESQ_DEF + INTERVALO_ALEATORIO )*6, 660, al_map_rgb(0,0,0), 2.0);
          al_draw_line((MARGEM_DIR_DEF - INTERVALO_ALEATORIO)*6, 0, (MARGEM_DIR_DEF - INTERVALO_ALEATORIO)*6, 660, al_map_rgb(0,0,0), 2.0);

           qtd = extremantes_margem(c, lim_esq);
           free(lim_esq);

       /* estudando pontos críticos das margens */
           for(i = 0; i < qtd - 1; i++)
           {
               if(c[i].x < c[i+1].x)
               {
                   al_draw_filled_triangle(c[i].x, c[i].y, c[i+1].x, c[i+1].y, c[i].x, c[i+1].y, al_map_rgb(250, 240, 230));
                   al_draw_filled_rectangle(MARGEM_ESQ_DEF * 6, c[i].y, c[i].x, c[i+1].y, al_map_rgb(250, 240, 230));
               }

               else if(c[i].x == c[i+1].x)
               al_draw_filled_rectangle(MARGEM_ESQ_DEF * 6, c[i].y, c[i+1].x, c[i+1].y, al_map_rgb(250, 240, 230));

               else if(c[i].x > c[i+1].x)
               {
                    al_draw_filled_triangle(c[i].x, c[i].y, c[i+1].x, c[i+1].y, c[i+1].x, c[i].y, al_map_rgb(250, 240, 230));
                    al_draw_filled_rectangle(MARGEM_ESQ_DEF * 6, c[i].y, c[i+1].x, c[i+1].y, al_map_rgb(250, 240, 230));
               }


               al_draw_line(c[i].x, c[i].y, c[i+1].x, c[i+1].y, al_map_rgb(0,0,0), 2.0);

           }


           qtd = extremantes_margem(c, lim_dir);
           free(lim_dir);

           for(i = 0; i < qtd - 1; i = i + 1)
           {
              if(c[i].x < c[i+1].x)
              {
                   al_draw_filled_triangle(c[i].x, c[i].y, c[i+1].x, c[i+1].y, c[i+1].x, c[i].y, al_map_rgb(250, 240, 230));
                   al_draw_filled_rectangle(MARGEM_DIR_DEF * 6, c[i+1].y, c[i+1].x, c[i].y, al_map_rgb(250, 240, 230));
              }

              else if(c[i].x == c[i+1].x)
              al_draw_filled_rectangle(MARGEM_DIR_DEF * 6, c[i+1].y, c[i].x, c[i].y, al_map_rgb(250, 240, 230));

              else if(c[i].x > c[i+1].x)
              {
                   al_draw_filled_triangle(c[i].x, c[i].y, c[i+1].x, c[i+1].y, c[i].x, c[i+1].y, al_map_rgb(250, 240, 230));
                   al_draw_filled_rectangle(MARGEM_DIR_DEF * 6, c[i+1].y, c[i].x, c[i].y, al_map_rgb(250, 240, 230));
              }

                  al_draw_line(c[i].x, c[i].y, c[i+1].x, c[i+1].y, al_map_rgb(0,0,0), 2.0);

           }

}


int has_colision_boat(Node *head, int Xb, int Yb, int Xm, int Ym, int Xc, int Yc)
{
Node *p = head;
int res, i, index;
int dist_x = (Xc - Xm) * 2;


        res = Yc;

        for(i = 0; i < (int)res / 22; i++)
        p = p->prox;

        index = (int) Xc / 6;

        if(get_type_pixel(&(p->v[index])) == '#')
        return 1;

        res += 22;
        p = p->prox;

        while(res < Ym)
        {
            int coord_x = (int)(Xm + (Ym - res)*(Xc - Xm) / (Ym - Yc))/ 6;

            if(get_type_pixel(&(p->v[coord_x])) == '#')
            return 1;

            coord_x = (int) (Xm + dist_x - (Ym - res)*(Xm + dist_x - Xc)/(Ym - Yc)) / 6;

            if(get_type_pixel(&(p->v[coord_x])) == '#')
            return 1;

            res += 22;
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

            res += 22;
            p = p->prox;
        }


return 0;
}


int extremantes_margem(Coordenadas* c, int* ptos_terra)
{
    int j;
    int cand, index = 1;
    int pto_y = 0;

        (&c[0])->x = ptos_terra[0];
        (&c[0])->y = pto_y++;
        cand = (&c[0])->x;

        for(j = 1; j < NROWS_DEF - 1; )
        {
            int flag = 0;


                     while(j < NROWS_DEF - 1 && cand > ptos_terra[j])
                     {
                         cand = ptos_terra[j++];
                         flag = 1;
                         pto_y++;
                     }

                     if(flag == 1)
                     {

                        (&c[index])->x = cand;
                        (&c[index++])->y = pto_y * 22;
                        flag = 0;
                     }

                     while(j < NROWS_DEF - 1 && cand < ptos_terra[j])
                     {
                         cand = ptos_terra[j++];
                         flag = 1;
                         pto_y++;
                     }


                     if(flag == 1)
                     {

                        (&c[index])->x = cand;
                        (&c[index++])->y = pto_y * 22;
                        flag = 0;
                     }

                     while(j < NROWS_DEF - 1 && cand == ptos_terra[j])
                     {
                         cand = ptos_terra[j++];
                         flag = 1;
                         pto_y++;
                     }


                     if(flag == 1)
                     {

                        (&c[index])->x = cand;
                        (&c[index++])->y = pto_y * 22;

                     }
        }

                        (&c[index])->x = ptos_terra[NROWS_DEF-1];
                        (&c[index++])->y = 660;



  return index;
}

int* limite_margem_esquerda(Node* head)
{
    int* limites = MallocSafe(NROWS_DEF * sizeof(int));
    int index = 0, i;
    Node* node = head->prox;

           while(node != head)
           {
               for(i = 0; i < NCOLS_DEF; i++)
                    if(i > MARGEM_ESQ_DEF && i <= MARGEM_ESQ_DEF + INTERVALO_ALEATORIO &&
                       get_type_pixel(&node->v[i]) == '#' && get_type_pixel(&node->v[i+1]) == '.' )
                    limites[index++] = i * 6;


                 node = Queue_prox(node);
           }

return limites;
}


int* limite_margem_direita(Node* head)
{
    int* limites = MallocSafe(NROWS_DEF * sizeof(int));
    int index = 0, i;
    Node* node = head->prox;

         while(node != head)
         {
             for(i = 0; i < NCOLS_DEF; i++)
                if(i < MARGEM_DIR_DEF && i >= MARGEM_DIR_DEF - INTERVALO_ALEATORIO && get_type_pixel(&node->v[i]) == '#' && get_type_pixel(&node->v[i-1]) == '.' )
                limites[index++] = i * 6;

         node = Queue_prox(node);
         }



return limites;
}

void  show_setup()
{
int op = 0;
int folga;
int larg;
float prob;
int s;
float vmin;
float vmax;



    printf("\nMenu Configuracoes\n\n");
    printf("1 - vel_max-  Define velocidade maxima dos pontos de agua\n");
    printf("2 - vel_min - Define velocidade minima dos pontos de agua\n");
    printf("3 - semente - Define semente da geracao de numeros aleatorios\n");
    printf("4 - probability - Define probabilidade de ocorrencia de uma ilha\n");
    printf("5 - largura - Define largura mínima das margens esquerda e direita do rio\n");
    printf("6 - folga - Define uma quantidade minima de linhas que separam duas ilhas\n");
    printf("7 - Restore - Restaura a configuracao default do jogo\n");
    printf("8 - Voltar ao menu principal\n");
    printf("\nEscolha o(s) item(ns) de configuracao desejados:");


          while(op != 8)
          {

              scanf("%d", &op);
              switch(op)
              {
                  case 1:
                          printf("Digite a velocidade maxima:\n");
                          scanf("%f", &vmax);
                          conf_set_vel_max(vmax);
                          printf("Feito!\n Digite novamente outra opcao do menu:");
                          break;

                  case 2:
                          printf("Digite a velocidade minima:\n");
                          scanf("%f", &vmin);
                          conf_set_vel_min(vmin);
                          printf("Feito!\n Digite novamente outra opcao do menu:");
                          break;

                  case 3:
                          printf("Digite a semente de geracao de numeros aleatorios:\n");
                          scanf("%d", &s);
                          conf_set_seed(s);
                          printf("Feito!\n Digite novamente outra opcao do menu:");
                          break;

                  case 4:
                          printf("Digite a probabilidade de ocorrencia de ilhas:\n");
                          scanf("%f", &prob);
                          conf_set_prob_ilha(prob);
                          printf("Feito!\n Digite novamente outra opcao do menu:");
                          break;

                 case 5:
                          printf("Digite a largura minima das margens:\n");
                          scanf("%d", &larg);
                          conf_set_largura_margens(larg);
                          printf("Feito!\n Digite novamente outra opcao do menu:");
                          break;

                 case 6:
                          printf("Digite a folga das ilhas:\n");
                          scanf("%d", &folga);
                          conf_set_folga_ilhas(folga);
                          printf("Feito!\n Digite novamente outra opcao do menu:");
                          break;

                 case 7: conf_restore_dafault();
                         printf("Configuracao default restaurada!\nDigite novamente outra opcao do menu:");
                         break;

                 case 8: break;

                 default: printf("Opcao invalida! Digite novamente:");
                          break;

              }


          }

}


