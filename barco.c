#include "barco.h"
#include "ambiente.h"
#include <math.h>

#define BLOCK_TRANSLATION 1.0

void inicializa_configuracao_barco() {
    x = X_INICIAL;
    y = Y_INICIAL;
    velocidade = VEL_MIN;
    angle = 0;

    // cantos do barco
    borda[0] = (Pixel) {0, 0 - h/2};
    borda[1] = (Pixel) {0, 0 + h/2};    
    borda[2] = (Pixel) {0 - w/2, 0};       
    borda[3] = (Pixel) {0 + w/2, 0};
}

int detectaColisao (Node map[NROWS+2]) {
    Pixel p;
    short i;

    for (i = 0; i < 4; i++) {
        p = rotacao(borda[i], angle);
        p.x += x;
        p.y += y;

        if (teste_quatro_vizinhos(p, map)) {
            return 1;
        }
    }

    return 0;
}

int teste_quatro_vizinhos (Pixel centro, Node map[NROWS+2]) {    
    Pixel v;

    v.x = centro.x - 1, v.y = centro.y;
    if (ehMargem(v, map)) { return 1;}
    
    v.x = centro.x, v.y = centro.y - 1;
    if (ehMargem(v, map)) { return 1;}

    v.x = centro.x, v.y = centro.y + 1;
    if (ehMargem(v, map)) { return 1;}

    v.x = centro.x + 1, v.y = centro.y;
    if (ehMargem(v, map)) { return 1;}

    v.x = centro.x, v.y = centro.y;
    if (ehMargem(v, map)) { return 1;}

    return 0;
}

int ehMargem(Pixel p, Node map[NROWS+2]) {
    int linha = ceil((1.0 * p.y)/BLOCO_Y);
    int i;

    if (p.x <= MARGEM_ESQ * BLOCO_X || p.x >= MARGEM_DIR * BLOCO_X)
        return 1;

    Node node = map[linha-1];
    
    int x = node.margem_esq * BLOCO_X;

    if (p.x <= x) return 1;

    x = node.margem_dir * BLOCO_X;

    if (p.x >= x) return 1;

    // testa se eh ilha
    Sequencia_Ilhas seq = node.seq;

    if (seq.qtd > 0) {
        x = seq.inicio * BLOCO_X;

        if (p.x > x+5 && p.x < x + seq.largura_pxs-5) {            
            return 1;
        }
    }
    
    return 0;
}


void movimenta_barco(char direcao) {
    if (direcao == 'L') 
    {
        x -= BLOCK_TRANSLATION * BLOCO_X;

        if (angle > -ALLEGRO_PI/9)
        {
            // 10 graus
            angle -= ALLEGRO_PI/18;
        }
    }
    else if (direcao == 'R') 
    {
        x += BLOCK_TRANSLATION * BLOCO_X;

        if (angle < ALLEGRO_PI/9)
        {
            angle += ALLEGRO_PI/18;
        }
    }
}


void corrige_inclinacao_barco() {
    float module_angle = fabs(angle);

    if (module_angle > 0 && module_angle < ALLEGRO_PI/18)
        module_angle = 0;

    else if (angle > 0) 
    {
        angle -= ALLEGRO_PI/18;
    }

    else if (angle < 0)
    {
        angle += ALLEGRO_PI/18;
    }
}

// em ralação a origem do plano xoy
// matriz de rotação:
// M = [cos(beta) -sen(-beta) ]
//     [sen(-beta)  cos(beta) ]
Pixel rotacao (Pixel p, float angle) {
    Pixel p2;
    float a, b;

    a = cos(angle);
    b = sin(angle);

    p2.x = (int) (a * p.x - b * p.y);
    p2.y = (int) (b * p.x + a * p.y);

    return p2;
}

bool houveColisao() {
    if (detectaColisao(river_map)) 
    {
        
        return true;
    }

    return false;
}