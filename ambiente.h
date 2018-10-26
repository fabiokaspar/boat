#ifndef AMBIENTE_H_INCLUDED
#define AMBIENTE_H_INCLUDED 


#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>




/* Macros de configuração do jogo   */

#define DISPLAY_HIGHT 500
#define DISPLAY_WEIGHT 400
#define NROWS 75
#define NCOLS 80
#define MARGEM_ESQ 8
#define MARGEM_DIR 72
#define INTERVALO 15
#define LIM_ESQ (MARGEM_ESQ + INTERVALO)
#define LIM_DIR (MARGEM_DIR - INTERVALO)
#define SIZE_ILHA 10
#define PROBABILITY_ILHA 100
#define FOLGA_ILHAS 40
#define SEMENTE 5
#define BLOCO_X ((float)(1.0 * DISPLAY_WEIGHT)/NCOLS)
#define BLOCO_Y ((float)(1.0 * DISPLAY_HIGHT)/NROWS)

#define TETO_QTD_ILHAS 4
#define BLOCOS_BARCO 12
#define BLOCOS_ILHA 9
#define ILHA_PXS (BLOCOS_ILHA * BLOCO_X)
#define LARGURA 90


typedef struct seq_ilha
{
	int inicio;
	int qtd;
	int largura_pxs;
} Sequencia_Ilhas;


typedef struct node
{
	int margem_esq;
	int margem_dir;
	int largura_rio;
	struct node* prox;
	struct node* ant;
	bool tem_relevo;
	bool eh_fim;
	Sequencia_Ilhas seq;
} Node;


Node* head;
Node river_map[NROWS+2];


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