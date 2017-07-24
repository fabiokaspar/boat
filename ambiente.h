#ifndef AMBIENTE_H_INCLUDED
#define AMBIENTE_H_INCLUDED 


#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>


/* Macros de configuração do jogo   */
#define MARGEM_ESQ 4
#define MARGEM_DIR 76
#define INTERVALO 20
#define LARGURA_MAX 50
#define LIM_ESQ (MARGEM_ESQ + INTERVALO)
#define LIM_DIR (MARGEM_DIR - INTERVALO)
#define SIZE_ILHA 10
#define PROBABILITY_ILHA 0.8
#define SEMENTE 5
#define FOLGA_ILHAS 30
#define FOLGA_OBJ 40
#define FPS 120


//#define DISPLAY_HIGHT 540
//#define QUADRADO_PIXELS 2
//#define DISPLAY_WEIGHT 480

#define DISPLAY_HIGHT 500
#define NROWS 30

#define DISPLAY_WEIGHT 560
#define NCOLS 80

typedef struct
{
	int x;
	int y;
} Ponto;

typedef struct node
{
	int margem_esq;
	int margem_dir;
	int inicio_ilha;
	struct node* prox;
	struct node* ant;
	int tem_chegada;
} Node;

Node* head;
long int score;
int passos_chegada;
int conta_passos;
int fim_jogo;
float x_chegada;

ALLEGRO_BITMAP* chegada;
int h_chegada;
int w_chegada;



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