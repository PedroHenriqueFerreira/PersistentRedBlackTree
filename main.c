#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VERSIONS 100
#define NEGRO 0
#define RUBRO 1

typedef struct no {
    int chave;
    int cor;
    struct no *esq;
    struct no *dir;
    struct no *pai;
} No;

// Criando as raizes
No *raiz[MAX_VERSIONS];
int num_versions = 0;

// Criando o nó externo
No *nulo = NULL;

// Busca o sucessor estritamente maior
No *SucessorCustom(int T, int k) {
    No *x = raiz[T];
    No *sucessor = nulo;

    while (x != nulo) {
        if (x->chave > k) {
            sucessor = x;
            x = x->esq;
        } else {
            x = x->dir;
        }
    }

    return sucessor;
}

// Algoritmo que copia recursivamente o nó passado
No *Copiar(No *x, No *pai_x) {
    if (x == nulo) return nulo;

    No *curr = (No *) malloc(sizeof(No));

    curr->chave = x->chave;
    curr->cor = x->cor;
    curr->pai = pai_x;

    curr->esq = Copiar(x->esq, curr);
    curr->dir = Copiar(x->dir, curr);

    return curr;
}

// Algoritmo para visitar um nó
void Visitar(No *x, int altura) {
    printf("%d,%d,%c ", x->chave, altura, x->cor == NEGRO ? 'N' : 'R');
}

// ------------------------------------------------------------------------
// Abaixo todos os algoritmos foram criados pelo professor durante as aulas
 
// Estruturas de Dados: 10 Percursos em Árvores (00:33:42)
void EmOrdem(No *x, int altura) {
    if (x != nulo) {
        EmOrdem(x->esq, altura + 1);
        Visitar(x, altura);
        EmOrdem(x->dir, altura + 1);
    }
}

// Estruturas de Dados: 11 Árvore Binária de Busca (00:26:46)
No *Buscar(No *x, int k) {
    if (x == nulo || x->chave == k) 
        return x;

    if (k < x->chave)
        return Buscar(x->esq, k);

    return Buscar(x->dir, k);
}

// Estruturas de Dados: 11 Árvore Binária de Busca (00:35:27)
No *Minimo(int T, No *x) {
    while (x->esq != nulo)
        x = x->esq;

    return x;
}

// Estruturas de Dados: 11 Árvore Binária de Busca (01:01:38)
No *Sucessor(int T, No *x) {
    if (x->dir != nulo)
        return Minimo(T, x->dir);

    No *y = x->pai;

    while (y != nulo && x == y->dir) {
        x = y;
        y = y->pai;
    }

    return y;
}

// Estruturas de Dados: 14 Inclusão em Árvore Rubro-Negra (00:01:13)
void RotacaoEsq(int T, No *x) {
    No *y = x->dir;
    x->dir = y->esq;

    if (y->esq != nulo)
        y->esq->pai = x;

    y->pai = x->pai;

    if (x->pai == nulo)
        raiz[T] = y;
    else if (x == x->pai->esq)
        x->pai->esq = y;
    else
        x->pai->dir = y;

    y->esq = x;
    x->pai = y;
}

// Simétrico à RotacaoEsq
void RotacaoDir(int T, No *y) {
    No *x = y->esq;
    y->esq = x->dir;

    if (x->dir != nulo)
        x->dir->pai = y;

    x->pai = y->pai;

    if (y->pai == nulo)
        raiz[T] = x;
    else if (y == y->pai->dir)
        y->pai->dir = x;
    else
        y->pai->esq = x;

    x->dir = y;
    y->pai = x;
}

// Estruturas de Dados: 14 Inclusão em Árvore Rubro-Negra (01:02:37)
void ConsertarInclusao(int T, No *z) {
    while (z->pai->cor == RUBRO) {
        if (z->pai == z->pai->pai->esq) {
            No *y = z->pai->pai->dir;

            if (y->cor == RUBRO) {
                z->pai->cor = NEGRO;
                y->cor = NEGRO;
                z = y->pai;
                z->cor = RUBRO;
            } else { 
                if (z == z->pai->dir) {
                    z = z->pai;
                    RotacaoEsq(T, z);
                }
                
                z->pai->cor = NEGRO;
                z->pai->pai->cor = RUBRO;
                RotacaoDir(T, z->pai->pai);
            }
        } else {
            No *y = z->pai->pai->esq;

            if (y->cor == RUBRO) {
                z->pai->cor = NEGRO;
                y->cor = NEGRO;
                z = y->pai;
                z->cor = RUBRO;
            } else { 
                if (z == z->pai->esq) {
                    z = z->pai;
                    RotacaoDir(T, z);
                }
                
                z->pai->cor = NEGRO;
                z->pai->pai->cor = RUBRO;
                RotacaoEsq(T, z->pai->pai);
            }
        }
    } 

    raiz[T]->cor = NEGRO;
}

// Estruturas de Dados: 14 Inclusão em Árvore Rubro-Negra (17:36)
void Incluir(int T, No *z) {
    No *y = nulo;
    No *x = raiz[T];
    
    while (x != nulo) {
        y = x;

        if (z->chave < x->chave)
            x = x->esq;
        else
            x = x->dir;   
    }

    z->pai = y;

    if (y == nulo)
        raiz[T] = z;
    else if (z->chave < y->chave)
        y->esq = z;
    else
        y->dir = z;

    z->esq = z->dir = nulo;
    z->cor = RUBRO;
    ConsertarInclusao(T, z);
}

// Estruturas de Dados: 15 Remoção em Árvore Rubro-Negra (00:03:06)
void Transplantar(int T, No *u, No *v) {
    if (u->pai == nulo)
        raiz[T] = v;
    else if (u == u->pai->esq)
        u->pai->esq = v;
    else    
        u->pai->dir = v;

    v->pai = u->pai;
}

// Estruturas de Dados: 15 Remoção em Árvore Rubro-Negra (00:56:51)
void ConsertarRemocao(int T, No *x) {
    while (x != raiz[T] && x->cor == NEGRO) {
        if (x == x->pai->esq) {
            No *w = x->pai->dir;
            if (w->cor == RUBRO) {
                w->cor = NEGRO;
                x->pai->cor = RUBRO;
                RotacaoEsq(T, x->pai);
                w = x->pai->dir;
            }

            if (w->esq->cor == NEGRO && w->dir->cor == NEGRO) {
                w->cor = RUBRO;
                x = x->pai;
            } else {
                if (w->dir->cor == NEGRO) {
                    w->esq->cor = NEGRO;
                    w->cor = RUBRO;
                    RotacaoDir(T, w); // No pseudocódigo estava RotacaoDir(T, x->pai);
                    w = x->pai->dir;
                }

                w->cor = x->pai->cor;
                x->pai->cor = NEGRO;
                w->dir->cor = NEGRO;
                RotacaoEsq(T, x->pai);

                x = raiz[T];
            }
        } else {
            No *w = x->pai->esq;
            if (w->cor == RUBRO) {
                w->cor = NEGRO;
                x->pai->cor = RUBRO;
                RotacaoDir(T, x->pai);
                w = x->pai->esq;
            }

            if (w->dir->cor == NEGRO && w->esq->cor == NEGRO) {
                w->cor = RUBRO;
                x = x->pai;
            } else {
                if (w->esq->cor == NEGRO) {
                    w->dir->cor = NEGRO;
                    w->cor = RUBRO;
                    RotacaoEsq(T, w); // No pseudocódigo estava RotacaoEsq(T, x->pai);
                    w = x->pai->esq;
                }

                w->cor = x->pai->cor;
                x->pai->cor = NEGRO;
                w->esq->cor = NEGRO;
                RotacaoDir(T, x->pai);
                x = raiz[T];
            }
        }
    }

    x->cor = NEGRO;
}

// Estruturas de Dados: 15 Remoção em Árvore Rubro-Negra (00:05:05)
void Remover(int T, No *z) {
    No *y = z; int cor_original_y = y->cor; 

    No *x; // Não tinha no pseudocódigo
    
    if (z->esq == nulo) {
        x = z->dir;
        Transplantar(T, z, x);
    } else if (z->dir == nulo) {
        x = z->esq;
        Transplantar(T, z, x);
    } else {
        y = Sucessor(T, z);
        x = y->dir; cor_original_y = y->cor;
        
        // Não tinha no pseudocódigo
        if (y->pai == z) {
            x->pai = y;
        } else {
            Transplantar(T, y, x);
            y->dir = z->dir;
            z->dir->pai = y;
        }
        Transplantar(T, z, y);
        y->esq = z->esq;
        z->esq->pai = y;
        y->cor = z->cor;

        // Estava no pseudocódigo
        // Transplantar(T, y, x);
        // y->esq = z->esq;
        // y->esq->pai = y;
        // y->dir = z->dir;
        // z->dir->pai = y;
        // Transplantar(T, z, y);
        // y->cor = z->cor;
    }

    free(z); // Não tinha no pseudocódigo

    if (cor_original_y == NEGRO)
        ConsertarRemocao(T, x);
}  

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Como usar: %s <arquivo>\n", argv[0]);
        return 1;
    }

    // Lendo o arquivo de entrada
    FILE *arquivo = fopen(argv[1], "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo");
        return 1;
    }

    // Inicializando o nó externo
    nulo = (No *) malloc(sizeof(No));
    nulo->cor = NEGRO; 
    nulo->esq = nulo->dir = nulo->pai = nulo;

    // Inicializa a primeira árvore
    raiz[num_versions] = nulo;

    char tag[4]; int a, b;
    while (fscanf(arquivo, "%s %d %d", tag, &a, &b) >= 2) { // b será opcional
        if (strcmp(tag, "INC") == 0) {
            num_versions++;

            raiz[num_versions] = Copiar(raiz[num_versions - 1], nulo);

            No *no = (No *) malloc(sizeof(No));
            no->chave = a;

            Incluir(num_versions, no);
        } else if (strcmp(tag, "REM") == 0) {
            num_versions++;

            raiz[num_versions] = Copiar(raiz[num_versions - 1], nulo);

            No *no = Buscar(raiz[num_versions], a);
            
            if (no != nulo)
                Remover(num_versions, no);
            
        } else if (strcmp(tag, "SUC") == 0) {
            printf("SUC %d %d\n", a, b);

            int v = b < 0 || b > num_versions ? num_versions : b;
            
            No *no = SucessorCustom(v, a);

            if (no == nulo) {
                printf("infinito\n");
            } else {
                printf("%d\n", no->chave);
            }
        } else if (strcmp(tag, "IMP") == 0) {
            printf("IMP %d\n", a);
            
            int v = a < 0 || a > num_versions ? num_versions : a;
            
            EmOrdem(raiz[v], 0);
            
            printf("\n");
        } else {
            fprintf(stderr, "Tag inválida: %s\n", tag);
            return 1;
        }
    }
    
    return 0;
}