#include "render.h"


void DesenhaImagem (SCREEN* s, char* caminho, int x, int y)
{
    ALLEGRO_BITMAP* img = s->img;
    s->img = al_load_bitmap(caminho);
    al_draw_bitmap(s->img, x, y, 0);
    s->img = img;
}


void DesenhaRio (Node* head)
{
    Node* node;
    float bloco_y = ((float)DISPLAY_HIGHT)/NROWS;
    float bloco_x = ((float)DISPLAY_WEIGHT)/NCOLS;
    int i;
    float y = 0;


    for (node = head->prox; node != head; node = node->prox)
    {
        // margem esquerda

        al_draw_filled_rectangle(0, y, MARGEM_ESQ * bloco_x, y + bloco_y, al_map_rgb(255, 230, 128));
        
        al_draw_filled_rounded_rectangle(0, y, node->margem_esq * bloco_x, y + bloco_y, 
            bloco_x/2, bloco_y/2, al_map_rgb(255, 230, 128));
        
        //float x1 = (node->margem_esq - 1.2) * bloco_x;

        //al_draw_filled_circle(x1, y,  2.0 * bloco_y, al_map_rgb(250, 240, 230));
            
        // margem direita
        
        al_draw_filled_rectangle(MARGEM_DIR * bloco_x, y, NCOLS * bloco_x, y + bloco_y, al_map_rgb(255, 230, 128));
        
        al_draw_filled_rounded_rectangle(node->margem_dir * bloco_x, y, NCOLS * bloco_x, y + bloco_y,
            bloco_x/2, bloco_y/2, al_map_rgb(255, 230, 128));

        
        //float x2 = (node->margem_dir + 4.2)* bloco_x;
        
        //al_draw_filled_circle(x2, y,  3.0 * bloco_y, al_map_rgb(250, 240, 230));

        y += bloco_y;
    }

}




