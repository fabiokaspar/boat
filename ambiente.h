#include "queue.h"

typedef int* MARGEM;

typedef struct
{
    float x;
    float y;
} Ponto;

typedef struct
{
    int foto;
    Ponto p;
} IMAGEM;

typedef struct no
{
    IMAGEM img;
    struct no* prox;
} QueueNode;

QueueNode *inicio, *fim;

Node* geraRio();
void atualizaRio(Node *head);
void ContaLimites(Node* node, int* xe, int* xd);

/* fila de figuras */
QueueNode* GetNext(QueueNode* atual);

