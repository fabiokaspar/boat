
#include "config.h"

int linhas = NROWS;
int margem_esq = MARGEM_ESQ;
int margem_dir = MARGEM_DIR;

void setNumberLines(int n)
{
    linhas = n;
}

int getNumberLines()
{
    return linhas;
}

void DefineMargemEsquerda(int mesq)
{
    margem_esq = mesq;
}

int DevolveMargemEsquerda()
{
    return margem_esq;
}

void DefineMargemDireita(int mdir)
{
    margem_dir = mdir;
}

int DevolveMargemDireita()
{
    return margem_dir;
}
