#include "ambiente.h"
#include "utils.h"
#include <stdio.h>
#include <time.h>
#include <math.h>


static int sorteia_ilha();

static int ME();
static int MD();

static void PreencheLinha(Node* node);
static void ilha(Node* node);
void DesalocaAmbiente(Node* head);


static int ME ()
{
    int me;
    static int xe = MARGEM_ESQ + 1.0 * INTERVALO/10;
    
    randomize(clock() + time(NULL));
    
    randomize(random_integer(0, 1000000));

    if (random_integer(0, 100) < 50)
        me = xe + random_integer(-2, 2);

    if (random_integer(50, 100) < 75)
        me = xe + random_integer(1, 2);
    
    else
        me = xe - random_integer(1, 2);

    if (me < MARGEM_ESQ)
        me = MARGEM_ESQ + 1.0 * INTERVALO/10;

    if (me > LIM_ESQ)
        me = LIM_ESQ - 1.0 * INTERVALO/10;

    xe = me;

    return me;
}

static int MD ()
{
    int md;
    static int xd = MARGEM_DIR - 1.0 * INTERVALO/10;

    randomize(clock() + time(NULL));
    
    randomize(random_integer(0, 1000000));

    if (random_integer(0, 100) < 50)
        md = xd + random_integer(-2, 2);

    else if (random_integer(50, 100) < 75)
        md = xd + random_integer(1, 2);
    
    else
        md = xd - random_integer(1, 2); 

    if (md > MARGEM_DIR)
        md = MARGEM_DIR - 1.0 * INTERVALO/10;

    if (md < LIM_DIR)
        md = LIM_DIR + 1.0 * INTERVALO/10;

    xd = md;

    return md;
}


Node* geraRio()
{
    int i;
    

    Node* head = Queue_Init();
    /* gera as margens */

    for (i = 0; i < NROWS; i++)
    {
        Node* node = Queue_Insert(head);

        PreencheLinha(node);
    }

    return head;
}

void atualizaRio(Node *head)
{
    int i;

    Node* node;

    Queue_Delete(head);

    node = Queue_Insert(head);

    PreencheLinha(node);
    
    randomize(clock());
}



static void PreencheLinha (Node* node)
{
    int me, md;
    me = ME();
    md = MD();

    if (md - me > LARGURA_MAX)
        md = LARGURA_MAX + me;
    
    if (md - me < LARGURA_MIN)
        md = LARGURA_MIN + me;

    node->margem_esq = me;
    node->margem_dir = md;

    static int index_texture = 0;

    node->index_texture = index_texture;

    index_texture += (int)(BLOCO_Y);

    if (index_texture > DISPLAY_HIGHT)
        index_texture = 0;

    
    static int folga = 0;

    folga++;

    if (score < 1000000 && folga >= FOLGA_ILHAS && sorteia_ilha()) {
        folga = 0;
        ilha(node);
    }
    
    if (node->margem_esq > INTERVALO && (NCOLS - node->margem_dir) > INTERVALO ) {
        if (score % 10 == 0) {
            fim_jogo = true;
        }
    }

}

// haverá ilha ?
static int sorteia_ilha ()
{
    randomize(SEMENTE + time(NULL));

    return (random_real(0, 1) <= PROBABILITY_ILHA);
}


static void ilha (Node* node)
{
    randomize(SEMENTE + time(NULL));
    
    node->inicio_ilha = random_integer(node->margem_esq + 15, node->margem_dir - 15);
}


void DesalocaAmbiente (Node* head)
{
    Queue_Free_All (head);
}



/*------------------------ fila de nodes -------------------------*/

Node* Queue_Init()
{
    Node* head = MallocSafe(sizeof(Node));
    
    head->prox = head;
    
    return head;
}

Node* Queue_Insert(Node *head)
{
    Node* node = (Node*) MallocSafe(sizeof(Node));

    node->prox = head->prox;

    node->ant = head;
    
    /*
    if (head->prox == head) {
        head->ant = node;
    }
    */

    head->prox->ant = node;
    
    head->prox = node;

    node->inicio_ilha = -1;

    return node;
}

void Queue_Delete(Node *head)
{
    Node* lixo = head->ant;

    lixo->ant->prox = head;

    head->ant = lixo->ant;

    free(lixo);
}

int Queue_Empty(Node *head)
{
    return head->prox == head;
}

void Queue_Free_All (Node *head)
{
    while (!Queue_Empty(head))
        Queue_Delete(head);

    free(head);
}


