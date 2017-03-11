#ifndef AMBIENTE_H_INCLUDED
#define AMBIENTE_H_INCLUDED 


/* Macros de configuração do jogo   */
#define MARGEM_ESQ 10
#define MARGEM_DIR 70
#define INTERVALO 4
#define SIZE_ILHA 10
#define PROBABILITY_ILHA 0.8
#define SEMENTE 5
#define FOLGA_ILHAS 15
//#define QUADRADO_PIXELS 2
#define FOLGA_OBJ 40
#define FPS 120

//#define DISPLAY_HIGHT 540
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
  float margem_esq;
  float margem_dir;
  int inicio_ilha;
  struct node* prox;
} Node;


Node* geraRio();


void atualizaRio(Node *head);


void ContaLimites(Node* node, int* xe, int* xd);


/* Inicializa a fila circular de nodes */
Node* Queue_Init();

/* Insere node (parâmetro não alocado na memória) na fila circular cuja cabeça é head */
Node* Queue_Insert(Node *head, int num_pxls);

/* Remove o mais antigo da fila circular suposta não vazia cuja cabeça é head */
void Queue_Delete(Node *head);

/* Retorna 1 se a fila circular encabeçada por head estiver vazia ou zero, caso contrário */
int Queue_Empty(Node *head);

/* Libera da memória todos os nodes da fila circular, inclusive head  */
void Queue_Free(Node *head);

/* Imprime os caracteres do vetor de pixles de todos os nodes da fila circular não vazia encabeçada por head */
void Queue_Print(Node *head);

void Queue_set_ilha(Node* node, int tem_ilha);

int Queue_get_ilha(Node* node);


#endif // AMBIENTE_H_INCLUDED