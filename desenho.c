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





/*void DesenhaRio(Node* head)
{
    int i, k, qtd;
    int x, y;
    int nrows = getNumberLines();
    int bloco = DISPLAY_HIGHT/nrows;
    Ponto* l = limite_margem_esquerda();
    Ponto* r = limite_margem_direita();
    Node* node;


    for(node = head->prox, y = 0; node != head; node = node->prox, y += bloco)
    {
        for(i = 0, x = 0; i < NCOLS; i++, x += 6)
        {
            bool margem = (i <= MARGEM_ESQ);
            bool ilha = (l[i].x < i * 6 && get_type_pixel(&node->v[i]) == '#');

            if(margem)
            {
                al_draw_filled_rectangle(x, y, x + 6, y + bloco, al_map_rgb(250, 240, 230));
            }

            if(ilha)
            {
               // al_draw_filled_rectangle(x, y, x + 6, y + bloco, al_map_rgb(250, 240, 230));
            }
        }
    }

    al_draw_line((MARGEM_ESQ + INTERVALO) * 6, 0, (MARGEM_ESQ + INTERVALO) * 6, 660, al_map_rgb(0,0,0), 1.0);



    for(i = 0; i <= nrows; i++)
    {
        if(i == nrows) break;

        if(l[i].x < l[i+1].x)
        {
            al_draw_filled_triangle(l[i].x, l[i].y, l[i+1].x, l[i+1].y, l[i].x, l[i+1].y, al_map_rgb(250, 240, 230));
            al_draw_filled_rectangle(MARGEM_ESQ * 6, l[i].y, l[i].x, l[i+1].y, al_map_rgb(250, 240, 230));
        }

        else if(l[i].x == l[i+1].x)
        {
            al_draw_filled_rectangle(MARGEM_ESQ * 6, l[i].y, l[i].x, l[i+1].y, al_map_rgb(250, 240, 230));
        }

        else
        {
            al_draw_filled_triangle(l[i].x, l[i].y, l[i+1].x, l[i+1].y, l[i+1].x, l[i].y, al_map_rgb(250, 240, 230));
            al_draw_filled_rectangle(MARGEM_ESQ * 6, l[i].y, l[i+1].x, l[i+1].y, al_map_rgb(250, 240, 230));
        }

    }
}

*/

