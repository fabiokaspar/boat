#include "ambiente.h"
#include "eventos.h"
#include "utils.h"
#include <stdio.h>
#include <time.h>
#include <math.h>


#define INCREASE_MAX 3
#define DECREASE_MAX (-3)


static int sorteia_ilha();
static void set_ilha(Node* node);

static int ME();
static int MD();

static void PreencheLinha(Node* node);


static int largura;
static int piso_qtd_ilhas;
static float probabilidade_ilha;
static int folga_ilhas;

static int ME ()
{
    int me;
    static int xe = MARGEM_ESQ + 1.0 * INTERVALO/10;
    short aux;
    static short i = 0;

    randomize(clock() + time(NULL));
    
    randomize(random_integer(0, 1000000));

    aux = random_integer(-1, 1);
   
    i += aux;
    if (i > INCREASE_MAX) {
        aux -= 2;
        i -= 2;
    }

    if (i < DECREASE_MAX) {
        aux += 2;
        i += 2;
    }
    
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
    static short i = 0;

    randomize(clock() + time(NULL));
    
    randomize(random_integer(0, 1000000));

    aux = random_integer(-1, 1);
    
    i += aux;
    if (i > INCREASE_MAX) {
        aux -= 2;
        i -= 2;
    }

    if (i < DECREASE_MAX) {
        aux += 2;
        i += 2;
    }

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
    int v;
    
    Node* head = Queue_Init();
    largura = LARGURA;
    piso_qtd_ilhas = 1;
    probabilidade_ilha = PROBABILITY_ILHA;

    /* gera as margens */

    for (i = 0; i < NROWS + 2; i++)
    {
        Node* node = Queue_Insert(head);

        PreencheLinha(node);

        river_map[i] = *node;
    }

    folga_ilhas = FOLGA_ILHAS-10;
        
    return head;
}

void atualizaRio(Node *head)
{
    int i;

    Node* node;

    Queue_Delete(head);

    node = Queue_Insert(head);

    PreencheLinha(node);

    static int cont_folga = 0;
    cont_folga++;

    if (distance > 0 && cont_folga >= folga_ilhas && sorteia_ilha()) 
    {
        cont_folga = 0;
        set_ilha(node);
    }

    if (!finca_bandeira && distance >= METRO_FINAL)
    {
        finca_bandeira = true;
        node->eh_fim = true;        
    }
    
    if (finca_bandeira && river_map[NROWS-15].eh_fim) {
        venceu = winner;
    }

    if (finca_bandeira && river_map[NROWS+1].eh_fim) {
        fim = true;
    }

    for (i = NROWS; i >= 0; i--) {
        river_map[i+1] = river_map[i];
    }

    river_map[0] = *node;
}

static void set_ilha (Node* node)
{
    Sequencia_Ilhas seq;
    
    int fit_ilhas = floor((node->largura_rio - BLOCOS_BARCO)/BLOCOS_ILHA);

    randomize(clock() + time(NULL));

    seq.qtd = random_integer(1, MIN(fit_ilhas, piso_qtd_ilhas));

    seq.largura_pxs = seq.qtd * ILHA_PXS;
    
    int a, b;
    a = node->margem_esq + 5;
    b = node->margem_dir - 5 - BLOCOS_ILHA * seq.qtd;

    float reason_free = (1.0 * seq.qtd * BLOCOS_ILHA /node->largura_rio);
    
    randomize(time(NULL) + clock());

    if (reason_free > 0.59 || largura < 40)
    {
        
        if (random_integer(0, 1) == 0) {
            randomize(clock() + time(NULL));
            int v1 = node->margem_esq + 1;
            seq.inicio = random_integer(v1, v1 + 1);
        }
        else {
            randomize(clock() + time(NULL));
            int v1 = node->margem_dir - 1 - BLOCOS_ILHA * seq.qtd;
            seq.inicio = random_integer(v1, v1 - 1);
        }
    }
    else 
    {
        seq.inicio = random_integer(a, b);
    }
    
    node->seq = seq;
}

static void PreencheLinha (Node* node)
{
    int me, md;
    me = ME();
    md = MD();
    
    static int q = 0;

    if (distance == 0) 
        q = 0;
        
    
    //printf("%d\n", largura);
    if ( ((int)(distance/30.0)) > q) 
    {        
        q++;        

        if (piso_qtd_ilhas < TETO_QTD_ILHAS)
            piso_qtd_ilhas++;

        if (distance >= 330) {
            probabilidade_ilha = 0;
        }

        randomize(clock() + time(NULL));

        if (distance < 70)
            folga_ilhas = random_integer(FOLGA_ILHAS - 15, FOLGA_ILHAS + 15);
        else if (distance < 160)
            folga_ilhas = random_integer(FOLGA_ILHAS - 10, FOLGA_ILHAS + 10);
        else if (distance < 250)
            folga_ilhas = random_integer(FOLGA_ILHAS - 5, FOLGA_ILHAS + 5);
        else
            folga_ilhas = random_integer(FOLGA_ILHAS + 20, FOLGA_ILHAS + 30);

        if (largura > 80) {
            //piso_qtd_ilhas = 3;
            largura -= 20;
        }

        else if (largura > 50) {
            //piso_qtd_ilhas = 3;
            largura -= 10;
        }

        else if (largura > 20) {
            //piso_qtd_ilhas = 2;
            largura -= 5;
        }

        printf("largura = %d\n", largura);
    }

    if (md - me > largura)
    {
        md = largura + me;
    }

    node->margem_esq = me;
    node->margem_dir = md;
    node->largura_rio = md - me;
}

// haverá ilha ?
static int sorteia_ilha ()
{
    randomize(clock() + time(NULL));
    randomize(random_integer(0, 1000000));

    return (random_real(0, 100) <= probabilidade_ilha);
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
    
    head->prox->ant = node;
    
    head->prox = node;

    node->seq.qtd = 0;

    node->eh_fim = false;
    
    node->tem_relevo = false;
    
    static short i = 0;
    
    i++;
    if (i == 4) {
        node->tem_relevo = true;
        i = 0;
    }

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
