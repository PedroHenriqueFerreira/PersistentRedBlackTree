# Árvore Rubro-Negra com Persistência Parcial

Este repositório contém a implementação de uma Árvore Binária de Busca Rubro-Negra com Persistência Parcial. A estrutura mantém o histórico de suas modificações (inclusões e remoções), permitindo consultas em versões anteriores utilizando o método descrito por Driscoll et al. (1989). 

---

## 1. A linguagem de programação usada (incluindo a versão)

* **Linguagem:** C
* **Versão utilizada/testada:** GCC 15.2.0

---

## 2. Como rodar o seu programa

O projeto inclui um `makefile` para automatizar e facilitar a compilação e execução. O programa lê os comandos a partir de um arquivo de texto (`.txt`).

Abra o terminal no diretório onde os arquivos do código estão localizados e utilize os comandos abaixo:

1. **Compilação:**
   Para compilar o código-fonte e gerar o executável (`programa.exe`), execute:
   ```bash
   make build
   ```

2. **Execução Padrão:**
   Para rodar o programa utilizando o arquivo de entrada padrão (ex: `entrada.txt`), execute:
   ```bash
   make run
   ```

3. **Execução com Arquivo Personalizado:**
   Se você quiser testar com um arquivo diferente (por exemplo, `entrada2.txt`), basta atribuir o nome do arquivo à variável `INPUT` durante o comando:
   ```bash
   make run INPUT=entrada2.txt
   ```

4. **Alternativa Manual (Sem Makefile):**
   Se preferir compilar e executar manualmente, você pode usar os seguintes comandos:
   ```bash
   gcc -o programa.exe main.c
   ./programa.exe entrada.txt
   ```

---

## 3. Descrição das funções e estruturas

A árvore foi implementada utilizando uma estrutura de nós encadeados suportada por um ecossistema de funções de leitura temporal (*Fat Nodes*). O controle das múltiplas versões é feito através de um vetor global de raízes.

### Estruturas Base (Equivalentes a `struct` no código)
* `enum Campo`: Enumeração que define qual campo está sendo modificado em uma versão (`COR`, `ESQ` ou `DIR`).
* `union Valor`: Estrutura flexível que armazena a alteração feita, podendo ser o ponteiro de um nó filho (`struct no *no`) ou a cor (`Cor cor`).
* `struct Mod`: Representa o histórico de uma alteração no nó. Contém a `versao` temporal da mudança, o `campo` que foi alterado e o novo `valor`.
* `struct no`: Estrutura principal que contém:
  * `chave`: Armazena o valor numérico do nó.
  * `cor`, `pai`, `esq` e `dir`: Campos base contendo o estado original do nó ao ser criado.
  * `num_mods`: Contador inteiro apontando quantas modificações extras o nó já sofreu.
  * `mods[MAX_MODS]`: Vetor que armazena as últimas modificações feitas no nó antes que ele precise ser clonado.

### Funções de Persistência (Controle de Versão)
* `ler(int T, No *no, Campo campo)`: Analisa o vetor de modificações (`mods`) e retorna o estado de um campo (`COR`, `ESQ` ou `DIR`) exatamente como ele era na versão temporal `T`.
* `escrever(int T, No *no, Campo campo, Valor valor)`: Tenta adicionar uma alteração no vetor `mods` do nó atual. Se não houver espaço, clona o nó para a nova versão e propaga recursivamente a alteração para cima até a raiz.
* **Atalhos de Acesso:** `esq(T, no)`, `dir(T, no)`, `cor(T, no)`, `set_esq(T, no, valor)`, `set_dir(T, no, valor)` e `set_cor(T, no, valor)`. Para manter o resto do algoritmo com uma sintaxe limpa e legível.

### Funções Principais (Navegação e Impressão)
* `SucessorCustom(int T, int k)`: Busca iterativamente o sucessor estritamente maior que a chave `k`, enxergando apenas a topologia da versão `T`.
* `Buscar(int T, No *x, int k)`: Busca recursivamente um nó com a chave `k` na versão `T`.
* `Minimo(int T, No *x)`: Retorna o ponteiro do nó com o menor valor na subárvore `x` na versão consultada.
* `Sucessor(int T, No *x)`: Encontra o sucessor em-ordem do nó `x`.
* `Visitar(int T, No *x, int altura, int *aux)`: Imprime as informações de um nó formatadas como `chave,altura,cor`.
* `EmOrdem(int T, No *x, int altura, int *aux)`: Realiza o percurso in-order na árvore para a operação de impressão.

### Funções de Manutenção (Árvore Rubro-Negra)
* `Incluir(int T, No *z)`: Insere o novo nó `z` na versão atual da árvore, comportando-se topologicamente como uma BST, e em seguida chama o conserto.
* `ConsertarInclusao(int T, No *z)`: Avalia os casos de violação rubro-negra de inserção e promove o rebalanceamento de cores e rotações.
* `Remover(int T, No *z)`: Substitui ou remove fisicamente o nó `z` da árvore atual. Utiliza uma técnica de *Nó Auxiliar* (Fantasma) caso o nó removido seja Negro sem filhos, para permitir que o conserto da árvore não precise lidar com ponteiros de nós nulos.
* `Transplantar(int T, No *u, No *v)`: Desconecta um nó `u` de seu pai e liga o nó `v` em seu lugar.
* `ConsertarRemocao(int T, No *x)`: Corrige desequilíbrios de "altura negra" gerados na deleção.
* `RotacaoEsq(int T, No *x)` / `RotacaoDir(int T, No *y)`: Rotacionam a estrutura para equilibrar as alturas, usando a função `escrever()` para garantir que cada rotação vire histórico persistente.

---

## 4. Arquivos onde as estruturas e funções se encontram

Todo o código-fonte principal e as estruturas de dados foram concentrados em um único arquivo monolítico. Isso evita dependências de compilação complexas e centraliza a alocação de memória do controle de versões:

* **`main.c`**: Contém a definição completa das estruturas (enums `Campo`, união `Valor`, structs `Mod` e `No`), a declaração do vetor global das versões (`raiz[MAX_VERSIONS]`), bem como todas as funções detalhadas no item anterior. Além disso, contém o ponto de entrada `main()`, que manipula a leitura/interpretação das tags do arquivo `.txt` (`INC`, `REM`, `SUC`, `IMP`).
* **`makefile`**: Script de configuração de ambiente de desenvolvimento. Responsável pelas diretrizes automatizadas de compilação do `main.c` em C e pela criação do executável.
