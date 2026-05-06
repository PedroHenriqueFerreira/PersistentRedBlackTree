#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MODS 2
#define MAX_VERSIONS 100

// Definindo as cores NEGRO e RUBRO
typedef enum { NEGRO, RUBRO } Cor;

// Definindo os campos COR, ESQ, DIR
typedef enum { COR, ESQ, DIR } Campo;

// Definindo o valor como No *ou Cor
typedef union {
    struct no *no;
    Cor cor;
} Valor;

// Estrutura de uma modificação
typedef struct {
    int versao;
    Campo campo;
    Valor valor;
} Mod;

// Estrutura de um nó com persistência parcial
typedef struct no {
    int chave;

    Cor cor;
    struct no *pai;
    struct no *esq;
    struct no *dir;

    int num_mods;
    Mod mods[MAX_MODS];
} No;

// Criando as raizes
No *raiz[MAX_VERSIONS];
int num_versoes = 0;

// Criando o nó externo
No *nulo = NULL;

// Função para ler o valor de um campo em uma versão específica
Valor ler(int T, No *no, Campo campo) {
    Valor valor; valor.no = nulo; valor.cor = NEGRO;

    if (no == nulo) return valor;
    
    // Lê o valor do campo especificado
    if (campo == COR) valor.cor = no->cor;
    else if (campo == ESQ) valor.no = no->esq;
    else if (campo == DIR) valor.no = no->dir;

    // Atualiza o valor do campo com a modificação mais recente
    for (int i = 0; i < no->num_mods; i++) {
        if (no->mods[i].campo != campo || no->mods[i].versao > T) continue;
        valor = no->mods[i].valor;
    }
    
    return valor;
}

// Funções auxiliares para acessar os campos de um nó
No *esq(int T, No *no) { return ler(T, no, ESQ).no; }
No *dir(int T, No *no) { return ler(T, no, DIR).no; }
Cor cor(int T, No *no) { return ler(T, no, COR).cor; }

// Função para escrever o valor de um campo em uma versão específica
No* escrever(int T, No *no, Campo campo, Valor valor) {
    if (no == nulo) return nulo;

    // Caso haja espaço para modificações, adiciona a modificação ao nó
    if (no->num_mods < MAX_MODS) {
        no->mods[no->num_mods].versao = T;
        no->mods[no->num_mods].campo = campo;
        no->mods[no->num_mods].valor = valor;
        no->num_mods++;

        if ((campo == ESQ || campo == DIR) && valor.no != nulo) 
            valor.no->pai = no;
        
        return no;
    }

    // Criando uma copia do nó para a nova versão
    No *novo = (No*) malloc(sizeof(No));
    novo->chave = no->chave;
    novo->cor = cor(T, no);
    novo->pai = no->pai;
    novo->esq = esq(T, no);
    novo->dir = dir(T, no);
    novo->num_mods = 0;

    // Reconecta os filhos para o novo nó
    if (novo->esq != nulo) novo->esq->pai = novo;
    if (novo->dir != nulo) novo->dir->pai = novo;

    // Aplica a modificação no novo nó
    if (campo == COR) novo->cor = valor.cor;
    else {
        if (campo == ESQ) novo->esq = valor.no;
        else if (campo == DIR) novo->dir = valor.no;

        if (valor.no != nulo) valor.no->pai = novo;
    }
    
    // Atualiza recursivamente o pai para apontar para o novo nó
    if (no->pai == nulo) {
        raiz[T] = novo;
    } else {
        Campo campo_pai = (esq(T, no->pai) == no) ? ESQ : DIR;
        Valor valor_pai; valor_pai.no = novo;
    
        novo->pai = escrever(T, no->pai, campo_pai, valor_pai);
    }

    return novo;
}

// Funções auxiliares para escrever os campos de um nó
No* set_esq(int T, No *no, No *valor) { return escrever(T, no, ESQ, (Valor){ .no = valor }); }
No* set_dir(int T, No *no, No *valor) { return escrever(T, no, DIR, (Valor){ .no = valor }); }
No* set_cor(int T, No *no, Cor valor) { return escrever(T, no, COR, (Valor){ .cor = valor }); }

// Busca o sucessor estritamente maior
No *SucessorCustom(int T, int k) {
    No *x = raiz[T];
    No *sucessor = nulo;

    while (x != nulo) {
        if (x->chave > k) {
            sucessor = x;
            x = esq(T, x);
        } else {
            x = dir(T, x);
        }
    }

    return sucessor;
}

// Algoritmo para visitar um nó
void Visitar(int T, No *x, int altura, int *aux) {
    if (*aux) *aux = 0;
    else printf(" ");

    printf("%d,%d,%c", x->chave, altura, cor(T, x) == NEGRO ? 'N' : 'R');
}

// ------------------------------------------------------------------------
// Abaixo todos os algoritmos foram criados pelo professor durante as aulas
 
// Estruturas de Dados: 10 Percursos em Árvores (00:33:42)
void EmOrdem(int T, No *x, int altura, int *aux) { // No pseudo-código T e aux não existe
    if (x != nulo) {
        EmOrdem(T, esq(T, x), altura + 1, aux);
        Visitar(T, x, altura, aux);
        EmOrdem(T, dir(T, x), altura + 1, aux);
    }
}

// Estruturas de Dados: 11 Árvore Binária de Busca (00:26:46)
No *Buscar(int T, No *x, int k) { // No pseudo-código T não existe
    if (x == nulo || x->chave == k) return x;
    if (k < x->chave) return Buscar(T, esq(T, x), k);
    return Buscar(T, dir(T, x), k);
}

// Estruturas de Dados: 11 Árvore Binária de Busca (00:35:27)
No *Minimo(int T, No *x) {
    while (esq(T, x) != nulo) 
        x = esq(T, x);

    return x;
}

// Estruturas de Dados: 11 Árvore Binária de Busca (01:01:38)
No *Sucessor(int T, No *x) {
    if (dir(T, x) != nulo)
        return Minimo(T, dir(T, x));

    No *y = x->pai;

    while (y != nulo && x == dir(T, y)) {
        x = y; y = y->pai;
    }

    return y;
}

// Estruturas de Dados: 14 Inclusão em Árvore Rubro-Negra (00:01:13)
No* RotacaoEsq(int T, No *x) { // No pseudo-código não existe retorno
    No *y = dir(T, x);
    x = set_dir(T, x, esq(T, y)); // Atualizando x (Não estava no pseudo-código)

    No *y_esq = esq(T, y);
    if (y_esq != nulo) y_esq->pai = x;

    y->pai = x->pai;

    if (x->pai == nulo)
        raiz[T] = y;
    else if (x == esq(T, x->pai))
        set_esq(T, x->pai, y);
    else
        set_dir(T, x->pai, y);

    y = set_esq(T, y, x);
    x->pai = y;

    return x;
}

// Simétrico à RotacaoEsq
No* RotacaoDir(int T, No *y) { // No pseudo-código não existe retorno
    No *x = esq(T, y);
    y = set_esq(T, y, dir(T, x)); // Atualizando y (Não estava no pseudo-código)

    No *x_dir = dir(T, x);
    if (x_dir != nulo) x_dir->pai = y;

    x->pai = y->pai;

    if (y->pai == nulo)
        raiz[T] = x;
    else if (y == dir(T, y->pai))
        set_dir(T, y->pai, x);
    else
        set_esq(T, y->pai, x);

    x = set_dir(T, x, y);
    y->pai = x;

    return y;
}

// Estruturas de Dados: 14 Inclusão em Árvore Rubro-Negra (01:02:37)
void ConsertarInclusao(int T, No *z) {
    while (cor(T, z->pai) == RUBRO) {
        if (z->pai == esq(T, z->pai->pai)) {
            No *y = dir(T, z->pai->pai);

            if (cor(T, y) == RUBRO) {
                set_cor(T, z->pai, NEGRO);
                set_cor(T, y, NEGRO);
                z = y->pai;
                z = set_cor(T, z, RUBRO);
            } else { 
                if (z == dir(T, z->pai)) {
                    z = z->pai;
                    z = RotacaoEsq(T, z);
                }
                
                set_cor(T, z->pai, NEGRO);
                set_cor(T, z->pai->pai, RUBRO);
                RotacaoDir(T, z->pai->pai);
            }
        } else {
            No *y = esq(T, z->pai->pai);
            
            if (cor(T, y) == RUBRO) {
                set_cor(T, z->pai, NEGRO);
                set_cor(T, y, NEGRO);
                z = y->pai;
                z = set_cor(T, z, RUBRO);
            } else { 
                if (z == esq(T, z->pai)) {
                    z = z->pai;
                    z = RotacaoDir(T, z);
                }
                
                set_cor(T, z->pai, NEGRO);
                set_cor(T, z->pai->pai, RUBRO);
                RotacaoEsq(T, z->pai->pai);
            }
        }
    } 

    set_cor(T, raiz[T], NEGRO);
}

// Estruturas de Dados: 14 Inclusão em Árvore Rubro-Negra (17:36)
void Incluir(int T, No *z) {
    No *y = nulo;
    No *x = raiz[T];
    
    while (x != nulo) {
        y = x;

        if (z->chave < x->chave) 
            x = esq(T, x);
        else 
            x = dir(T, x);
    }

    z->pai = y;

    if (y == nulo) raiz[T] = z;
    else {
        if (z->chave < y->chave) 
            set_esq(T, y, z);
        else 
            set_dir(T, y, z);
    }

    z->esq = z->dir = nulo; z->cor = RUBRO;

    ConsertarInclusao(T, z);
}

// Estruturas de Dados: 15 Remoção em Árvore Rubro-Negra (00:03:06)
void Transplantar(int T, No *u, No *v) {
    if (u->pai == nulo)
        raiz[T] = v;
    else if (u == esq(T, u->pai))
        set_esq(T, u->pai, v);
    else    
        set_dir(T, u->pai, v);
    
    v->pai = u->pai;
}

// Estruturas de Dados: 15 Remoção em Árvore Rubro-Negra (00:56:51)
void ConsertarRemocao(int T, No *x) {
    while (x != raiz[T] && cor(T, x) == NEGRO) {
        if (x == esq(T, x->pai)) { 
            No *w = dir(T, x->pai);
            if (cor(T, w) == RUBRO) {
                set_cor(T, w, NEGRO);
                set_cor(T, x->pai, RUBRO);
                RotacaoEsq(T, x->pai);
                w = dir(T, x->pai);
            }
            
            if (cor(T, esq(T, w)) == NEGRO && cor(T, dir(T, w)) == NEGRO) {
                set_cor(T, w, RUBRO);
                x = x->pai;
            } else {
                if (cor(T, dir(T, w)) == NEGRO) {
                    set_cor(T, esq(T, w), NEGRO);
                    w = set_cor(T, w, RUBRO);
                    RotacaoDir(T, w); // No pseudo-código estava: RotacaoDir(T, x->pai);
                    w = dir(T, x->pai);
                }

                w = set_cor(T, w, cor(T, x->pai));
                set_cor(T, x->pai, NEGRO);
                set_cor(T, dir(T, w), NEGRO);
                RotacaoEsq(T, x->pai);

                x = raiz[T];
            }
        } else {
            No *w = esq(T, x->pai);
            if (cor(T, w) == RUBRO) {
                set_cor(T, w, NEGRO);
                set_cor(T, x->pai, RUBRO);
                RotacaoDir(T, x->pai);
                w = esq(T, x->pai);
            }

            if (cor(T, dir(T, w)) == NEGRO && cor(T, esq(T, w)) == NEGRO) {
                set_cor(T, w, RUBRO);
                x = x->pai;
            } else {
                if (cor(T, esq(T, w)) == NEGRO) {
                    set_cor(T, dir(T, w), NEGRO);
                    w = set_cor(T, w, RUBRO);
                    RotacaoEsq(T, w); // No pseudo-código estava: RotacaoEsq(T, x->pai);
                    w = esq(T, x->pai);
                }

                w = set_cor(T, w, cor(T, x->pai));
                set_cor(T, x->pai, NEGRO);
                set_cor(T, esq(T, w), NEGRO);
                RotacaoDir(T, x->pai);

                x = raiz[T];
            }
        }
    }

    set_cor(T, x, NEGRO);
}

// Estruturas de Dados: 15 Remoção em Árvore Rubro-Negra (00:05:05)
void Remover(int T, No *z) {
    No *y = z; int cor_original_y = cor(T, y); 
    No *x; // Não estava no pseudo-código
    No *x_pai; Campo campo; // Não estava no pseudo-código

    if (esq(T, z) == nulo) {
        x_pai = z->pai; campo = (z == esq(T, z->pai)) ? ESQ : DIR; // Não estava no pseudo-código
        x = dir(T, z);
        Transplantar(T, z, x); 
    } else if (dir(T, z) == nulo) {
        x_pai = z->pai; campo = (z == esq(T, z->pai)) ? ESQ : DIR; // Não estava no pseudo-código
        x = esq(T, z);
        Transplantar(T, z, x);
    } else {
        y = Sucessor(T, z);
        x = dir(T, y); cor_original_y = cor(T, y);

        int y_pai_eq_z = (y->pai == z);

        // Não estava no pseudo-código

        if (!y_pai_eq_z) {
            x_pai = y->pai; campo = (y == esq(T, y->pai)) ? ESQ : DIR; // Não estava no pseudo-código
            Transplantar(T, y, x);
            y = set_dir(T, y, dir(T, z));
        }
        
        Transplantar(T, z, y);
        y = set_esq(T, y, esq(T, z));
        y = set_cor(T, y, cor(T, z));

        if (y_pai_eq_z) { x_pai = y; campo = DIR; }

        // Estava no pseudo-código

        // Transplantar(T, y, x);
        // y->esq = z->esq;
        // y->esq->pai = y;
        // y->dir = z->dir; // Caso (y->pai = z) então y->dir = y (ciclo);
        // z->dir->pai = y;
        // Transplantar(T, z, y);
        // y->cor = z->cor;
    }

    if (cor_original_y == NEGRO) {
        // Caso especial quando x é nulo (Não estava no pseudo-código)
        if (x == nulo && x_pai != nulo) {
            No *aux = (No*) malloc(sizeof(No));
            *aux = *nulo;
        
            if (campo == ESQ) x_pai = set_esq(T, x_pai, aux);
            else x_pai = set_dir(T, x_pai, aux);

            ConsertarRemocao(T, aux);

            x_pai = aux->pai;

            if (aux == esq(T, x_pai)) x_pai = set_esq(T, x_pai, nulo);
            else x_pai = set_dir(T, x_pai, nulo);

            free(aux);
        } else
            ConsertarRemocao(T, x);
    }
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
    nulo->cor = NEGRO; nulo->num_mods = 0;
    nulo->pai = nulo->esq = nulo->dir = nulo;

    // Inicializa a primeira árvore
    raiz[num_versoes] = nulo;

    char tag[4]; int a, b;
    while (fscanf(arquivo, "%s %d %d", tag, &a, &b) >= 2) { // b será opcional
        if (strcmp(tag, "INC") == 0) {
            num_versoes++;
            raiz[num_versoes] = raiz[num_versoes - 1];

            No *no = (No *) malloc(sizeof(No));
            no->chave = a; no->num_mods = 0;

            Incluir(num_versoes, no);
        } else if (strcmp(tag, "REM") == 0) {
            num_versoes++;
            raiz[num_versoes] = raiz[num_versoes - 1];

            No *no = Buscar(num_versoes, raiz[num_versoes], a);
            
            if (no != nulo) Remover(num_versoes, no);
            
        } else if (strcmp(tag, "SUC") == 0) {
            printf("SUC %d %d\n", a, b);

            int v = b < 0 || b > num_versoes ? num_versoes : b;
            
            No *no = SucessorCustom(v, a);

            if (no == nulo) {
                printf("infinito\n");
            } else {
                printf("%d\n", no->chave);
            }
        } else if (strcmp(tag, "IMP") == 0) {
            printf("IMP %d\n", a);
            
            int v = a < 0 || a > num_versoes ? num_versoes : a;
            
            int aux = 1;
            EmOrdem(v, raiz[v], 0, &aux);
            
            printf("\n");
        } else {
            // fprintf(stderr, "Tag inválida: %s\n", tag);
            // return 1;
        }
    }
    
    return 0;
}