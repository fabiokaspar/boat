#include "desenho.h"
#include "config.h"
#include "ambiente.h"

void DesenhaImagem(SCREEN* s, char* caminho, int x, int y)
{
    ALLEGRO_BITMAP* img = s->img;
    s->img = al_load_bitmap(caminho);
    al_draw_bitmap(s->img, x, y, 0);
    s->img = img;
}


void DesenhaRio(Node* head)
{
    Node* node;
    int bloco_y = DISPLAY_HIGHT/NROWS;
    int bloco_x = DISPLAY_WEIGHT/NCOLS;
    int i;
    int x, y = 0;
    int me, md, me_prox, md_prox;


    for(node = head->prox; node != head; node = node->prox)
    {
        x = 0;

        for(i = 0; i < NCOLS; i++)
        {
            if(get_type_pixel(&node->v[i]) == '#')
            {
                al_draw_filled_rectangle(x, y, x + bloco_x, y + bloco_y, al_map_rgb(250, 240, 230));
            }

            x += bloco_x;
        }

        if(node->prox != head)
        {
            int x1, y1, x2, y2, x3, y3;

            if(node == head->prox)
            {
                ContaLimites(node, &me, &md);
            }

            else
            {
                me = me_prox;
                md = md_prox;
            }

            ContaLimites(node->prox, &me_prox, &md_prox);

            x1 = me * bloco_x;
            y1 = y;

            x2 = me_prox * bloco_x;
            y2 = y + bloco_y;

            if(me < me_prox)
            {
                x3 = x1;
                y3 = y2;
            }

            if(me > me_prox)
            {
                x3 = x2;
                y3 = y1;
            }

            if(me != me_prox)
            {
                al_draw_filled_triangle(x1, y1, x2, y2, x3, y3 ,al_map_rgb(0, 0, 0));
            }

            x1 = md * bloco_x;
            x2 = md_prox * bloco_x;

            if(md < md_prox)
            {
                x3 = x2;
                y3 = y1;
            }

            if(md > md_prox)
            {
                x3 = x1;
                y3 = y2;
            }

            if(md != md_prox)
            {
                al_draw_filled_triangle(x1, y1, x2, y2, x3, y3 ,al_map_rgb(250, 240, 230));
            }
        }

        y += bloco_y;
    }

}




