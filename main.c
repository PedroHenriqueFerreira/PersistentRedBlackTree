#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NULO 0
#define MAX_NOS 5000
#define MAX_VERSOES 100
#define NEGRO 0
#define RUBRO 1

int raiz[MAX_VERSOES];

int chave[MAX_NOS];
int cor[MAX_NOS];
int esq[MAX_NOS];
int dir[MAX_NOS];
int pai[MAX_NOS];

int nos = 0;
int versoes = 0;

/* Algoritmo que copia a última árvore por completo (retorna a nova raiz) */
int Copiar(int x, int pai_x) {
    if (x == NULO) return NULO;

    int curr = ++nos;
    chave[curr] = chave[x];
    cor[curr] = cor[x];
    pai[curr] = pai_x;

    esq[curr] = Copiar(esq[x], curr);
    dir[curr] = Copiar(dir[x], curr);
    
    return curr;
}

/* Algoritmo para visitar um nó */
void Visitar(int x, int altura) {
    printf("%d,%d,%c ", chave[x], altura, cor[x] == RUBRO ? 'R' : 'N');
}

/* ------------------------------------------------------------------------ */
/* Abaixo todos os algoritmos foram criados pelo professor durante as aulas */

/* Estruturas de Dados: 11 Árvore Binária de Busca (00:26:46) */
int Buscar(int x, int k) {
    if (x == NULO || chave[x] == k) {
        return x;
    }

    if (k < chave[x]) {
        return Buscar(esq[x], k);
    }
    return Buscar(dir[x], k);
}

/* Estruturas de Dados: 10 Percursos em Árvores (00:33:42) */
void EmOrdem(int x, int altura) {
    if (x != NULO) {
        EmOrdem(esq[x], altura + 1);
        Visitar(x, altura);
        EmOrdem(dir[x], altura + 1);
    }
}

/* Estruturas de Dados: 11 Árvore Binária de Busca (00:35:27) */
int Minimo(int T, int x) {
    while (esq[x] != NULO) {
        x = esq[x];
    }

    return x;
}

/* Estruturas de Dados: 11 Árvore Binária de Busca (01:01:38) */
int Sucessor(int T, int x) {
    if (dir[x] != NULO) {
        return Minimo(T, dir[x]);
    }

    int y = pai[x];

    while (y != NULO && x == dir[x]) {
        x = y;
        y = pai[x];
    }

    return y;
}

/* Estruturas de Dados: 14 Inclusão em Árvore Rubro-Negra (00:01:13) */
void RotacaoEsq(int T, int x) {
    int y = dir[x];
    dir[x] = esq[y];
    
    if (esq[y] != NULO) {
        pai[esq[y]] = x;
    }

    pai[y] = pai[x];

    if (pai[x] == NULO) {
        raiz[T] = y;
    } else if (x == esq[pai[x]]) {
        esq[pai[x]] = y;
    } else {
        dir[pai[x]] = y;
    }

    esq[y] = x;
    pai[x] = y;
}

/* Simétrico à RotacaoEsq */
void RotacaoDir(int T, int y) {
    int x = esq[y];
    esq[y] = dir[x];

    if (dir[x] != NULO) {
        pai[dir[x]] = y;
    }

    pai[x] = pai[y];

    if (pai[y] == NULO) {
        raiz[T] = x;
    } else if (y == dir[pai[y]]) {
        dir[pai[y]] = x;
    } else {
        esq[pai[y]] = x;
    }

    dir[x] = y;
    pai[y] = x;
}

/* Estruturas de Dados: 14 Inclusão em Árvore Rubro-Negra (01:02:37) */
void ConsertarInclusao(int T, int z) {
    while (cor[pai[z]] == RUBRO) {
        if (pai[z] == esq[pai[pai[z]]]) {
            int y = dir[pai[pai[z]]];

            if (cor[y] == RUBRO) {
                cor[pai[z]] = NEGRO;
                cor[y] = NEGRO;
                z = pai[y];
                cor[z] = RUBRO;
            } else { 
                if (z == dir[pai[z]]) {
                    z = pai[z];
                    RotacaoEsq(T, z);
                }
                
                cor[pai[z]] = NEGRO;
                cor[pai[pai[z]]] = RUBRO;
                RotacaoDir(T, pai[pai[z]]);
            }
        } else {
            int y = esq[pai[pai[z]]];

            if (cor[y] == RUBRO) {
                cor[pai[z]] = NEGRO;
                cor[y] = NEGRO;
                z = pai[y];
                cor[z] = RUBRO;
            } else { 
                if (z == esq[pai[z]]) {
                    z = pai[z];
                    RotacaoDir(T, z);
                }
                
                cor[pai[z]] = NEGRO;
                cor[pai[pai[z]]] = RUBRO;
                RotacaoEsq(T, pai[pai[z]]);
            }
        }
    } 

    cor[raiz[T]] = NEGRO;
}

/* Estruturas de Dados: 14 Inclusão em Árvore Rubro-Negra (17:36) */
void Incluir(int T, int z) {
    int y = NULO;
    int x = raiz[T];

    while (x != NULO) {
        y = x;

        if (chave[z] < chave[x]) {
            x = esq[x];
        } else {
            x = dir[x];
        }
    }

    pai[z] = y;

    if (y == NULO) {
        raiz[T] = z;
    } else if (chave[z] < chave[y]) {
        esq[y] = z;
    } else {
        dir[y] = z;
    }

    esq[z] = dir[z] = NULO;
    cor[z] = RUBRO;
    ConsertarInclusao(T, z);
}

/* Estruturas de Dados: 15 Remoção em Árvore Rubro-Negra (00:03:06) */
void Transplantar(int T, int u, int v) {
    if (pai[u] == NULO) {
        raiz[T] = v;
    } else if (u == esq[pai[u]]) {
        esq[pai[u]] = v;
    } else {
        dir[pai[u]] = v;
    }

    pai[v] = pai[u];
}

/* Estruturas de Dados: 15 Remoção em Árvore Rubro-Negra (00:56:51) */
void ConsertarRemocao(int T, int x) {
    while (x != raiz[T] && cor[x] == NEGRO) {
        if (x == esq[pai[x]]) {
            int w = dir[pai[x]];
            if (cor[w] == RUBRO) {
                cor[w] = NEGRO;
                cor[pai[x]] = RUBRO;
                RotacaoEsq(T, pai[x]);
                w = dir[pai[x]];
            }

            if (cor[esq[w]] == NEGRO && cor[dir[w]] == NEGRO) {
                cor[w] = RUBRO;
                x = pai[x];
            } else {
                if (cor[dir[w]] == NEGRO) {
                    cor[esq[w]] = NEGRO;
                    cor[w] = RUBRO;
                    RotacaoDir(T, pai[x]);
                    w = dir[pai[x]];
                }

                cor[w] = cor[pai[x]];
                cor[pai[x]] = NEGRO;
                cor[dir[w]] = NEGRO;
                RotacaoEsq(T, pai[x]);
                x = raiz[T];
            }
        } else {
            int w = esq[pai[x]];
            if (cor[w] == RUBRO) {
                cor[w] = NEGRO;
                cor[pai[x]] = RUBRO;
                RotacaoDir(T, pai[x]);
                w = esq[pai[x]];
            }

            if (cor[dir[w]] == NEGRO && cor[esq[w]] == NEGRO) {
                cor[w] = RUBRO;
                x = pai[x];
            } else {
                if (cor[esq[w]] == NEGRO) {
                    cor[dir[w]] = NEGRO;
                    cor[w] = RUBRO;
                    RotacaoEsq(T, pai[x]);
                    w = esq[pai[x]];
                }

                cor[w] = cor[pai[x]];
                cor[pai[x]] = NEGRO;
                cor[esq[w]] = NEGRO;
                RotacaoDir(T, pai[x]);
                x = raiz[T];
            }
        }

        cor[x] = NEGRO;
    }
}

/* Estruturas de Dados: 15 Remoção em Árvore Rubro-Negra (00:05:05) */
void Remover(int T, int z) {
    int y = z; int cor_original_y = cor[y]; int x;
    if (esq[z] == NULO) {
        x = dir[z];
        Transplantar(T, z, x);
    } else if (dir[z] == NULO) {
        x = esq[z];
        Transplantar(T, z, x);
    } else {
        y = Sucessor(T, z);
        x = dir[y]; cor_original_y = cor[y];
        Transplantar(T, y, x);
        esq[y] = esq[z];
        pai[esq[z]] = y;
        dir[y] = dir[z];
        pai[dir[z]] = y;
        Transplantar(T, z, y);
        cor[y] = cor[z];
    }

    if (cor_original_y == NEGRO) {
        ConsertarRemocao(T, x);
    }
}  

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Como usar: %s <arquivo>\n", argv[0]);
        return 1;
    }

    /* Lendo o arquivo de entrada */
    FILE *arquivo = fopen(argv[1], "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo");
        return 1;
    }

    /* Inicializa o nó externo */
    cor[0] = NEGRO;
    esq[0] = NULO;
    dir[0] = NULO;
    pai[0] = NULO;

    /* Inicializa a primeira árvore */
    raiz[0] = NULO;
    
    char tag[3]; int a, b;
    while (fscanf(arquivo, "%s %d %d", tag, &a, &b) >= 2) { /* b será opcional */
        if (strcmp(tag, "INC") == 0) {
            versoes++;

            raiz[versoes] = Copiar(raiz[versoes - 1], NULO);

            nos++;
            chave[nos] = a;
            
            Incluir(versoes, nos);
        } else if (strcmp(tag, "REM") == 0) {
            versoes++;

            raiz[versoes] = Copiar(raiz[versoes - 1], NULO);

            int no = Buscar(raiz[versoes], a);
            
            if (no != NULO) {
                Remover(versoes, no);
            }
            
        } else if (strcmp(tag, "SUC") == 0) {
            printf("SUC %d %d\n", a, b);

            int v = b < 0 || b > versoes ? versoes : b;
            
            int no = Sucessor(v, Buscar(raiz[v], a));

            if (no == NULO) {
                printf("infinito\n");
            } else {
                printf("%d\n", chave[no]);
            }
        } else if (strcmp(tag, "IMP") == 0) {
            printf("IMP %d\n", a);
            
            int v = a < 0 || a > versoes ? versoes : a;
            
            EmOrdem(raiz[v], 0);
            
            printf("\n");
        } else {
            fprintf(stderr, "Tag inválida: %s\n", tag);
            return 1;
        }
    }
    
    return 0;
}