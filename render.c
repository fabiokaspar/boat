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
        
        Node* next = node->prox;
           
        // margem esquerda
        al_draw_filled_rectangle(0 * bloco_x, y, node->margem_esq * bloco_x, y + bloco_y, al_map_rgb(255, 230, 128));

        // margem direita
        al_draw_filled_rectangle(node->margem_dir * bloco_x, y, NCOLS * bloco_x, y + bloco_y, al_map_rgb(255, 230, 128));
        
        
        if (next != head) {
            // margem esquerda
            
            int x1 = node->margem_esq * bloco_x;
            int x2 = next->margem_esq * bloco_x;
        
            if (x1 > x2) {
                al_draw_filled_triangle(x1, y + bloco_y, x2, y + bloco_y, x2, y + 2 * bloco_y, al_map_rgb(255, 230, 128));            
            }
            else if (x1 < x2) {
                al_draw_filled_triangle(x1, y, x1, y + bloco_y, x2, y + bloco_y, al_map_rgb(255, 230, 128));            
            }

            else {
                //al_draw_filled_rectangle(x1 - 20, y, x1, y + bloco_y, al_map_rgb(136, 100, 40));
            }                
            
            // margem direita
            x1 = node->margem_dir * bloco_x;
            x2 = next->margem_dir * bloco_x;

            if (x1 > x2) {
                al_draw_filled_triangle(x1, y, x1, y + bloco_y, x2, y + bloco_y,  al_map_rgb(255, 230, 128));            
            }
            else if (x1 < x2) {
                al_draw_filled_triangle(x1, y + bloco_y, x2, y + bloco_y, x2, y + 2 * bloco_y, al_map_rgb(255, 230, 128));            
            }
        }
        
        if (node->inicio_ilha != -1) {
            al_draw_filled_rectangle(node->inicio_ilha * bloco_x, y, (node->inicio_ilha + SIZE_ILHA) * bloco_x, y + bloco_y, al_map_rgb(255, 230, 128));
        }

        y += bloco_y;

        
    }

}




