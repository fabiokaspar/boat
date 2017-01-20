#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#include "pixel.h"
#include "config.h"

typedef struct node
{
  PIXEL* v;
  int tem_ilha;
  struct node *prox;
} Node;


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




#endif // QUEUE_H_INCLUDED
