#include "ambiente.h"
#include "config.h"
#include "random.h"
#include "pixel.h"
#include "utils.h"
#include <stdio.h>
#include <time.h>
#include <math.h>

#define FOLGA_OBJ 40

static int sorteia_ilha(int flg);
static int conta_folga(Node *head);

static int MargemEsquerdaLinha();
static int MargemDireitaLinha();

static void PreencheLinha(Node* node);
static void GeraIlha(Node* node);
static void SorteiaImagemMargens(int y);

static void InsertQueue(IMAGEM img);
static void DeleteQueue();
static void UpdateQueue();
void DesalocaAmbiente(Node* head);

static float contl = 0;
static float contr = 0;

static MARGEM lt;
static MARGEM rt;
static int contalinhas = 0;

static int freelt = 0;
static int freert = 0;

/*  */
static int MargemEsquerdaLinha()
{
    int me;
    randomize(clock());

    me = MARGEM_ESQ + INTERVALO * sin(contl);
    contl += 0.1;

    if(contl == 5) contl = 0;

    return me;
}

/*  */
static int MargemDireitaLinha()
{
    int md;
    randomize(clock());

    md = MARGEM_DIR - INTERVALO * cos(contr);

    contr += 0.1;

    if(contr == 5) contr = 0;

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
        Queue_set_ilha(node, sorteia_ilha(cont));

        PreencheLinha(node);

        if(cont > 0)
            cont--;

        else cont = FOLGA_ILHAS;

        SorteiaImagemMargens(i * DISPLAY_HIGHT/NROWS);
    }


    return head;
}


static void PreencheLinha(Node* node)
{
    int i;
    int me, md;

    me = MargemEsquerdaLinha();
    md = MargemDireitaLinha();

    for(i = 0; i <= me; i++) /* marg. esquerda */
    {
        set_type_pixel(&node->v[i], '#');
    }

    for(i = me + 1; i < md; i++)  /* água */
    {
        set_type_pixel(&node->v[i], '.');
    }

    for(i = md; i < NCOLS; i++) /* marg. direita */
    {
        set_type_pixel(&node->v[i], '#');
    }

     /* Linha tem ilha ? */

     if(Queue_get_ilha(node))
     {
        GeraIlha(node);
     }
}


void atualizaRio(Node *head)
{
    int i;
    Node* node;


    Queue_Delete(head);

    node = Queue_Insert(head, NCOLS);

    Queue_set_ilha(node, sorteia_ilha(conta_folga(head)));

     UpdateQueue();
    SorteiaImagemMargens(-65);

    randomize(clock());

    PreencheLinha(node);
}

void ContaLimites(Node* node, int* xe, int* xd)
{
    int i;
    int MAXESQ = MARGEM_ESQ + INTERVALO;
    int MINDIR = MARGEM_DIR - INTERVALO;

    (*xe) = MAXESQ + 1;
    (*xd) = MINDIR - 1;

    for(i = MARGEM_ESQ; i < MAXESQ; i++)
    {
        if(get_type_pixel(&node->v[i+1]) == '.')
        {
            (*xe) = i+1;
            break;
        }
    }

    for(i = MARGEM_DIR; i > MINDIR; i--)
    {
        if(get_type_pixel(&node->v[i-1]) == '.')
        {
            (*xd) = i-1;
            break;
        }
    }
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

    for(node = head->prox; node != head && Queue_get_ilha(node) == 0; node = node->prox)
    {
        qtd++;

        if(qtd == FOLGA_ILHAS)
            break;
    }

    return qtd;
}


static void InitQueue()
{
    inicio = fim = NULL;
}

static void InsertQueue(IMAGEM img)
{
    QueueNode* novo = MallocSafe(sizeof(*novo));
    novo->img = img;
    novo->prox = NULL;

    if(!inicio)
    {
        inicio = novo;
    }

    else
    {
        fim->prox = novo;
    }

    fim = novo;
}

static void DeleteQueue()
{
    if(inicio)
    {
        QueueNode* gb = inicio;
        inicio = inicio->prox;
        free(gb);

        if(!inicio)
        {
            fim = NULL;
        }
    }
}

QueueNode* GetNext(QueueNode* atual)
{
    return atual->prox;
}

static void UpdateQueue()
{
    QueueNode* pt;
    int i, N = 0;
    int nrows = getNumberLines();

    for(pt = inicio; pt != NULL; pt = pt->prox)
    {
        pt->img.p.y += (DISPLAY_HIGHT/nrows);

        if(pt->img.p.y > DISPLAY_HIGHT)
           N++;
    }

    for(i = 0; i < N; i++)
    {
        DeleteQueue();
    }
}

static void GeraIlha(Node* node)
{
    int j, inicio_ilha;
    int margem_esq = DevolveMargemEsquerda();
    int margem_dir = DevolveMargemDireita();

    randomize(SEMENTE + time(NULL));
    inicio_ilha = random_integer(margem_esq + INTERVALO + 2, margem_dir - INTERVALO - 2);

    for(j = inicio_ilha; j <= inicio_ilha + SIZE_ILHA; j++)
    {
        set_type_pixel(&node->v[j], '#');
    }
}

void SorteiaImagemMargens(int y)
{
    if(freelt == FOLGA_OBJ)
    {
        randomize(2 * clock());

        if(random_real(0, 10) <= 0.1)
        {
            IMAGEM esq;
            int margem_esq = DevolveMargemEsquerda();
            esq.foto = 1;
            esq.p.x = random_integer(1, margem_esq-4) * 6;
            esq.p.y = y;
            InsertQueue(esq);
            freelt = 0;
        }
    }

    else freelt++;

    if(freert == FOLGA_OBJ)
    {
        randomize(clock());

        if(random_real(0, 10) <= 0.1)
        {
            IMAGEM dir;
            int margem_dir = DevolveMargemDireita();
            dir.foto = 1;
            dir.p.x = random_integer(margem_dir+1, NCOLS-5) * 6;
            dir.p.y = y;
            InsertQueue(dir);
            freert = 0;
        }
    }

    else freert++;

}
