#include "utils.h"
#include "queue.h"
#include <stdio.h>


Node* Queue_Init()
{
    Node* head = MallocSafe(sizeof(Node));
    head->prox = head;

    return head;
}

Node* Queue_Insert(Node *head, int num_pxls)
{
    Node* node = (Node*) MallocSafe(sizeof(Node));
    node->v = (PIXEL*) MallocSafe(num_pxls * sizeof(PIXEL));
    node->prox = head->prox;
    head->prox = node;

    return node;
}

void Queue_Delete(Node *head)
{
    Node *p = head;

    while(p->prox->prox != head)
        p = p->prox;

    free(p->prox);
    p->prox = head;
}


void Queue_set_ilha(Node* node, int tem_ilha)
{
    node->tem_ilha = tem_ilha;
}

int Queue_get_ilha(Node* node)
{
    return node->tem_ilha;
}

int Queue_Empty(Node *head)
{
    return head->prox == head;
}

void Queue_Free(Node *head)
{
    while(!Queue_Empty(head))
        Queue_Delete(head);

    free(head);
}

void Queue_Print(Node *head)
{
    Node *aux = head->prox;
    int i;

    while(aux != head)
    {
        for(i = 0; i < NCOLS; i++)
            printf("%c", get_type_pixel(&aux->v[i]));

        printf("\n");

        aux = aux->prox;
    }

}

