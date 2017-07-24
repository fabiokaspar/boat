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

int me;

static int ME ()
{
    static int xe = MARGEM_ESQ + 2;
    randomize(clock() + time(NULL));

    if (random_real(0, 100) < 50)
        me = xe + random_integer(-2, 2);

    else if (random_real(0, 50) < 25)
        me = xe + random_integer(1, 2);
    
    else
        me = xe - random_integer(1, 2); 

    if (me < MARGEM_ESQ)
        me = MARGEM_ESQ;

    else if (me > LIM_ESQ)
        me = LIM_ESQ;

    xe = me;

    return me;
}

static int MD ()
{
    static int xd = MARGEM_DIR - 2;
    randomize(clock() + time(NULL));
    int md;

    if (random_real(0, 100) < 50)
        md = xd + random_integer(-2, 2);

    else if (random_real(0, 50) < 25)
        md = xd - random_integer(1, 2);
    
    else
        md = xd + random_integer(1, 2); 

    if (md > MARGEM_DIR)
        md = MARGEM_DIR;

    else if (md < LIM_DIR)
        md = LIM_DIR;

    xd = md;

    if (md - me > LARGURA_MAX)
        md = LARGURA_MAX + me;

    return md;
}


Node* geraRio()
{
    int i;
    
    Node* head = Queue_Init();
    fim_jogo = 0;
    passos_chegada = NROWS + 15;
    conta_passos = -30;
    chegada = al_load_bitmap("images/chegada.png");
    h_chegada = al_get_bitmap_height(chegada);
    w_chegada = al_get_bitmap_width(chegada);

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
    float me, md;
    me = ME();
    md = MD();


    node->margem_esq = me;
    node->margem_dir = md;

    
    static int folga = 0;

    folga++;

    if (folga >= FOLGA_ILHAS && sorteia_ilha()) {
        folga = 0;
        ilha(node);
    }
    
    if (score % 60 == 0) {
        node->tem_chegada = 1;
        fim_jogo = 1;
        float bloco_x = ((float)DISPLAY_WEIGHT)/NCOLS;
        float largura = (node->margem_dir - node->margem_esq);
        
        float dl = fabs(largura - w_chegada/bloco_x) * 0.5;
        x_chegada = (node->margem_esq + dl) * bloco_x;
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
    
    node->inicio_ilha = random_integer(node->margem_esq + 5, node->margem_dir - 5);
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

    node->tem_chegada = 0;


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


