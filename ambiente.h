#ifndef AMBIENTE_H_INCLUDED
#define AMBIENTE_H_INCLUDED 

#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>



/* Macros de configuração do jogo   */

#define DISPLAY_HIGHT 560
#define DISPLAY_WEIGHT 560
#define NROWS 20
#define NCOLS 80
#define MARGEM_ESQ 8
#define MARGEM_DIR 72
#define INTERVALO 20
#define LARGURA_MAX 50
#define LARGURA_MIN 25
#define LIM_ESQ (MARGEM_ESQ + INTERVALO)
#define LIM_DIR (MARGEM_DIR - INTERVALO)
#define SIZE_ILHA 10
#define PROBABILITY_ILHA 100
#define SEMENTE 5
#define FOLGA_ILHAS 10
#define FOLGA_OBJ 40
#define FPS 200
#define BLOCO_X ((float)(1.0 * DISPLAY_WEIGHT)/NCOLS)
#define BLOCO_Y ((float)(1.0 * DISPLAY_HIGHT)/NROWS)


typedef struct node
{
	int margem_esq;
	int margem_dir;
	int inicio_ilha;
	struct node* prox;
	struct node* ant;
	int coef_relevo;
} Node;

typedef struct
{
    int x;
    int y;
} Pixel;


Node* head;
Node river_map[NROWS+2];
bool fim;
long int score;


Node* geraRio();

void atualizaRio(Node *head);


/* Inicializa a fila circular de nodes */
Node* Queue_Init();

/* Insere node (parâmetro não alocado na memória) na fila circular cuja cabeça é head */
Node* Queue_Insert(Node *head);

/* Remove o mais antigo da fila circular suposta não vazia cuja cabeça é head */
void Queue_Delete(Node *head);

/* Retorna 1 se a fila circular encabeçada por head estiver vazia ou zero, caso contrário */
int Queue_Empty(Node *head);

/* Libera da memória todos os nodes da fila circular, inclusive head  */
void Queue_Free_All (Node *head);




#endif // AMBIENTE_H_INCLUDED