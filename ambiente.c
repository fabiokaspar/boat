#include "ambiente.h"
#include "utils.h"
#include <stdio.h>
#include <time.h>
//#include <math.h>


static int sorteia_ilha();

static int ME();
static int MD();

static void PreencheLinha(Node* node);
static void ilha(Node* node);


static int ME ()
{
    int me;
    static int xe = MARGEM_ESQ + 1.0 * INTERVALO/10;
    static short increase = 0;
    short aux;

    randomize(clock() + time(NULL));
    
    randomize(random_integer(0, 1000000));

    aux = random_integer(-1, 1);

    me = xe + aux;
            
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
    short aux;

    randomize(clock() + time(NULL));
    
    randomize(random_integer(0, 1000000));

    aux = random_integer(-1, 1);
    
    md = xd + aux;

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

    for (i = 0; i < NROWS + 2; i++)
    {
        Node* node = Queue_Insert(head);

        PreencheLinha(node);

        river_map[i] = *node;
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

    for (i = NROWS; i >= 0; i--) {
        river_map[i+1] = river_map[i];
    }

    river_map[0] = *node;
    
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
    
    static int folga = 0;

    folga++;

    if (score < 1000000 && folga >= FOLGA_ILHAS && sorteia_ilha()) {
        folga = 0;
        ilha(node);
    }

}

// haverá ilha ?
static int sorteia_ilha ()
{
    randomize(clock() + time(NULL));
    randomize(random_integer(0, 1000000));

    return (random_real(0, 100) <= PROBABILITY_ILHA);
}


static void ilha (Node* node)
{
    randomize(clock() + time(NULL));
    randomize(random_integer(0, 1000000));
    
    node->inicio_ilha = random_integer(node->margem_esq + 5, node->margem_dir - 15);
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
    static int i = 0;

    node->prox = head->prox;

    node->ant = head;
    
    head->prox->ant = node;
    
    head->prox = node;

    node->inicio_ilha = -1;

    randomize(clock() * time(NULL));
    randomize(random_integer(0, 1000000));

    node->coef_relevo = random_integer(2, 5); 
    

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


