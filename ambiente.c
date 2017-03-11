#include "ambiente.h"
#include "utils.h"
#include <stdio.h>
#include <time.h>
#include <math.h>


static int sorteia_ilha(int flg);
static int conta_folga(Node *head);

static int ME();
static int MD();

static void PreencheLinha(Node* node);
static void GeraIlha(Node* node);
void DesalocaAmbiente(Node* head);


float contl = 0;
float contr = 0;

static int ME ()
{
    float me;
    randomize(clock());
    int rn = random_integer(0, 30);
    int rf = random_integer(0, 100);

    if (rf <= 100) {
        if (rn <= 10)
            me = MARGEM_ESQ +  INTERVALO * fabs(sin(contl));
        
        else if (rn <= 20)
            me = MARGEM_ESQ + 1.2 * INTERVALO * fabs(sin(contl));
        
        else
            me = MARGEM_ESQ + 2.0 * INTERVALO * fabs(sin(contl)); 
    }
    else {
        if (rn <= 10)
            me = MARGEM_ESQ +  INTERVALO * sqrt(fabs(sin(contl)));
        
        else if (rn <= 20)
            me = MARGEM_ESQ + 1.2 * INTERVALO * sqrt(fabs(sin(contl)));
        
        else
            me = MARGEM_ESQ + 2.0 * INTERVALO * sqrt(fabs(sin(contl)));   
    }

    contl += 0.1;

    if(contl == 5) contl = 0;

    return me;
}

static int MD ()
{
    float md;
    randomize(clock());
    int rn = random_integer(0, 30);
    int rf = random_integer(0, 100);

    if (rf <= 50) {
        if (rn <= 10)
            md = MARGEM_DIR -  INTERVALO * fabs(cos(contl));
        
        else if (rn <= 20)
            md = MARGEM_DIR - 1.2 * INTERVALO * fabs(cos(contl));
        
        else
            md = MARGEM_DIR - 2.0 * INTERVALO * fabs(cos(contl));
    }

    else {
        if (rn <= 10)
            md = MARGEM_DIR -  INTERVALO * sqrt(fabs(cos(contl)));
        
        else if (rn <= 20)
            md = MARGEM_DIR - 1.2 * INTERVALO * sqrt(fabs(cos(contl)));
        
        else
            md = MARGEM_DIR - 2.0 * INTERVALO * sqrt(fabs(cos(contl)));   
    }

    contr += 0.1;

    if (contr == 5) contr = 0;

    return md;
}


Node* geraRio()
{
    int cont = FOLGA_ILHAS;
    int i;

    Node* head = Queue_Init();

    /* gera as margens */

    for(i = 0; i < NROWS; i++)
    {
        Node* node = Queue_Insert(head, NCOLS);
        //Queue_set_ilha(node, sorteia_ilha(cont));

        PreencheLinha(node);

        if(cont > 0)
            cont--;

        else cont = FOLGA_ILHAS;
    }

    return head;
}


static void PreencheLinha(Node* node)
{
    float me, md;

    me = ME();
    md = MD();

    node->margem_esq = me;
    node->margem_dir = md;
    node->inicio_ilha = -1;


    /*

     // Linha tem ilha ? 

     if (Queue_get_ilha(node))
     {
        GeraIlha(node);
     }
     */
}


void atualizaRio(Node *head)
{
    int i;
    Node* node;

    Queue_Delete(head);

    node = Queue_Insert(head, NCOLS);

    //Queue_set_ilha(node, sorteia_ilha(conta_folga(head)));

    randomize(clock());

    PreencheLinha(node);
}



void DesalocaAmbiente(Node* head)
{
    Queue_Free(head);
}

/* se 1, é ilha, se 0, é água */

static int sorteia_ilha(int flg)
{
    randomize(SEMENTE + time(NULL));

    return (random_real(0, 1) <= PROBABILITY_ILHA && flg >= FOLGA_ILHAS);
}

static int conta_folga(Node *head)
{
    int qtd = 0;
    Node *node = head->prox;

    for(node = head->prox; node != head && node->inicio_ilha == -1; node = node->prox)
    {
        qtd++;

        if(qtd == FOLGA_ILHAS)
            break;
    }

    return qtd;
}


static void GeraIlha(Node* node)
{
    /*
    int j, inicio_ilha;
    int margem_esq = DevolveMargemEsquerda();
    int margem_dir = DevolveMargemDireita();

    randomize(SEMENTE + time(NULL));
    inicio_ilha = random_integer(margem_esq + INTERVALO + 2, margem_dir - INTERVALO - 2);

    for(j = inicio_ilha; j <= inicio_ilha + SIZE_ILHA; j++)
    {
        node->v[j] = '#';
    }
    */
}



/*------------------------ fila de nodes -------------------------*/

Node* Queue_Init()
{
    Node* head = MallocSafe(sizeof(Node));
    head->prox = head;

    return head;
}

Node* Queue_Insert(Node *head, int num_pxls)
{
    Node* node = (Node*) MallocSafe(sizeof(Node));

    node->prox = head->prox;
    
    head->prox = node;

    return node;
}

void Queue_Delete(Node *head)
{
    Node *p = head;

    while (p->prox->prox != head)
        p = p->prox;

    free(p->prox);
    p->prox = head;
}

void Queue_set_ilha(Node* node, int tem_ilha)
{
    //node->tem_ilha = tem_ilha;
}

int Queue_get_ilha(Node* node)
{
    //return node->tem_ilha;
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


