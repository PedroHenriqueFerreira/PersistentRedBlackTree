# Árvore Rubro-Negra com Persistência Parcial

Este repositório contém a implementação de uma Árvore Binária de Busca Rubro-Negra com Persistência Parcial. A estrutura mantém o histórico de suas modificações (inclusões e remoções), permitindo consultas em versões anteriores utilizando o método descrito por Driscoll et al. (1989). 

---

## 1. Linguagem de Programação Usada

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

2. **Execução padrão:**
   Para rodar o programa utilizando o arquivo de entrada padrão (`entrada.txt`), execute:
   ```bash
   make run
   ```

3. **Execução com arquivo personalizado:**
   Para testar um arquivo diferente (por exemplo, `entrada2.txt`), execute:
   ```bash
   make run INPUT=entrada2.txt
   ```

4. **Alternativa manual (sem makefile):**
   Para compilar e executar manualmente, execute:
   ```bash
   gcc -o programa.exe main.c
   ./programa.exe entrada.txt
   ```

---

## 3. Descrição das Funções e Estruturas

A árvore foi implementada utilizando uma estrutura de nós encadeados suportada por um ecossistema de funções de leitura temporal. O controle das múltiplas versões é feito através de um vetor global de raízes.

### Estruturas Base
* `enum Campo`: Enumeração que define qual campo está sendo modificado em uma versão (`COR`, `ESQ` ou `DIR`).
* `union Valor`: Estrutura flexível que armazena a alteração feita, podendo ser o ponteiro de um nó filho (`struct no *no`) ou a cor (`Cor cor`).
* `struct Mod`: Representa o histórico de uma alteração no nó. Contém a `versao` temporal da mudança, o `campo` que foi alterado e o novo `valor`.
* `struct no`: Estrutura principal que contém:
  * `chave`: Armazena o valor numérico do nó.
  * `cor`, `pai`, `esq` e `dir`: Campos base contendo o estado original do nó ao ser criado.
  * `num_mods`: Contador inteiro apontando quantas modificações extras o nó já sofreu.
  * `mods[MAX_MODS]`: Vetor que armazena as últimas modificações feitas no nó antes que ele precise ser clonado.

### Funções de Persistência
* `ler(int T, No *no, Campo campo)`: Analisa o vetor de modificações (`mods`) e retorna o estado de um campo (`COR`, `ESQ` ou `DIR`) exatamente como ele era na versão temporal `T`.
* `escrever(int T, No *no, Campo campo, Valor valor)`: Tenta adicionar uma alteração no vetor `mods` do nó atual. Se não houver espaço, clona o nó para a nova versão e propaga recursivamente a alteração para cima até a raiz.
* **Atalhos de Acesso:** `esq(T, no)`, `dir(T, no)`, `cor(T, no)`, `set_esq(T, no, valor)`, `set_dir(T, no, valor)` e `set_cor(T, no, valor)`. Para manter o algoritmo com uma sintaxe limpa e legível.

### Funções Auxiliares
* `SucessorCustom(int T, int k)`: Busca iterativamente o sucessor estritamente maior que a chave `k`, enxergando apenas a estrutura da versão `T`.
* `Buscar(int T, No *x, int k)`: Busca recursivamente um nó com a chave `k` na versão `T`.
* `Minimo(int T, No *x)`: Retorna o ponteiro do nó com o menor valor na subárvore `x` na versão consultada.
* `Sucessor(int T, No *x)`: Encontra o sucessor em-ordem do nó `x`.
* `Visitar(int T, No *x, int altura, int *aux)`: Imprime as informações de um nó formatadas como `chave,altura,cor`.
* `EmOrdem(int T, No *x, int altura, int *aux)`: Realiza o percurso in-order na árvore para a operação de impressão.

### Funções Principais
* `Incluir(int T, No *z)`: Insere o novo nó `z` na versão atual da árvore e em seguida chama o conserto.
* `ConsertarInclusao(int T, No *z)`: Avalia os casos de violação rubro-negra de inserção e promove o rebalanceamento de cores e rotações.
* `Remover(int T, No *z)`: Substitui ou remove fisicamente o nó `z` da árvore atual e em seguida chama o conserto.
* `Transplantar(int T, No *u, No *v)`: Desconecta um nó `u` de seu pai e liga o nó `v` em seu lugar.
* `ConsertarRemocao(int T, No *x)`: Avalia os casos de violação rubro-negra de remoção e promove o rebalanceamento de cores e rotações.
* `RotacaoEsq(int T, No *x)` / `RotacaoDir(int T, No *y)`: Rotacionam a estrutura para equilibrar as alturas, usando a função `escrever()` para garantir que cada rotação vire histórico persistente.

---

## 4. Arquivos onde as estruturas e funções se encontram

Todo o código-fonte principal e as estruturas de dados foram concentrados em um único arquivo monolítico. Isso evita dependências de compilação complexas e centraliza a alocação de memória do controle de versões:

* **`main.c`**: Contém a definição completa das estruturas (enums `Campo`, união `Valor`, structs `Mod` e `No`), a declaração do vetor global das versões (`raiz[MAX_VERSIONS]`), bem como todas as funções detalhadas no item anterior. Além disso, contém o ponto de entrada `main()`, que manipula a leitura/interpretação das tags do arquivo `.txt` (`INC`, `REM`, `SUC`, `IMP`).
* **`makefile`**: Script de configuração de ambiente de desenvolvimento. Responsável pelas diretrizes automatizadas de compilação do `main.c` em C e pela criação do executável.
