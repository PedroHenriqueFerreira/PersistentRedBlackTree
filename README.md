# Árvore Rubro-Negra com Persistência Parcial

Este repositório contém a implementação de uma Árvore Binária de Busca Rubro-Negra com Persistência Parcial. A estrutura mantém o histórico de suas modificações (inclusões e remoções), permitindo consultas em versões anteriores utilizando o método de cópia de caminho (*path copying*).

## Linguagem de Programação Usada

* **Linguagem:** C
* **Versão:** GCC 15.2.0 (ou equivalente)

## Como Rodar o Seu Programa

O projeto inclui um `makefile` para automatizar e facilitar a compilação e execução. O programa lê os comandos a partir de um arquivo de texto (`.txt`).

Abra o terminal no diretório onde os arquivos do código estão localizados e utilize os comandos abaixo:

1. **Compilação:**
   Para compilar o código-fonte e gerar o executável (`programa.exe`), execute:
   ```bash
   make build
   ```

2. **Execução Padrão:**
   Para rodar o programa utilizando o arquivo de entrada padrão (`entrada.txt`), execute:
   ```bash
   make run
   ```

3. **Execução com Arquivo Personalizado:**
   Se você quiser testar com um arquivo diferente (por exemplo, `entrada2.txt`), basta atribuir o nome do arquivo à variável `INPUT` durante o comando:
   ```bash
   make run INPUT=entrada2.txt
   ```

4. **Alternativa manual:**
   Se preferir compilar e executar manualmente sem usar o `makefile`, você pode usar os seguintes comandos:
   ```bash
   gcc -o programa.exe main.c
   ./programa.exe entrada.txt
   ```

## Descrição das Estruturas e Funções

A árvore foi implementada utilizando uma estrutura de nós encadeados por ponteiros. O controle das múltiplas versões é feito através de um vetor global de raízes.

### Estrutura Base (`struct no`)
Cada nó da árvore é definido pela estrutura `No`, que contém:
* `chave`: Armazena o valor (conteúdo) inteiro do nó.
* `cor`: Armazena a cor do nó (`RUBRO` ou `NEGRO`).
* `pai`, `esq` e `dir`: Ponteiros para o nó pai, filho esquerdo e filho direito, respectivamente.
* `versao`: Armazena o número da versão em que o nó foi criado (utilizado para otimizar a função de cópia e evitar duplicações desnecessárias).

Variáveis globais importantes:
* `raiz[MAX_VERSIONS]`: Um vetor que armazena os ponteiros para a raiz de cada versão gerada (limitado a 100 versões).

### Funções Principais
* `SucessorCustom(int T, int k)`: Busca o sucessor estritamente maior do nó com chave `k` na versão `T` da árvore.
* `Copiar(No *x)`: Realiza uma cópia do nó `x` e propaga a cópia para os seus ancestrais até a raiz, garantindo a persistência parcial através da técnica de *path copying*.
* `Buscar(No *x, int k)`: Busca um nó com a chave `k` a partir do nó `x`.
* `Minimo(int T, No *x)`: Retorna o ponteiro do nó com o menor valor na subárvore `x`.
* `Sucessor(int T, No *x)`: Retorna o ponteiro do nó que é o sucessor em ordem do nó `x`.
* `Visitar(No *x, int altura, int *aux)`: Imprime as informações de um nó formatadas como `chave,altura,cor`.
* `EmOrdem(No *x, int altura, int *aux)`: Realiza o percurso em-ordem na árvore para a operação de impressão (IMP).

### Funções de Manutenção da Árvore Rubro-Negra
* `Incluir(int T, No *z)`: Insere um novo nó `z` na árvore da versão `T`, mantendo as propriedades de busca.  
* `ConsertarInclusao(int T, No *z)`: Restaura as propriedades rubro-negras após uma inserção.
* `Remover(int T, No *z)`: Remove o nó `z` da árvore na versão atual.
* `Transplantar(int T, No *u, No *v)`: Função auxiliar que substitui a subárvore enraizada em `u` pela subárvore enraizada em `v`.
* `ConsertarRemocao(int T, No *x)`: Restaura as propriedades rubro-negras após uma remoção.
* `RotacaoEsq(int T, No *x)` / `RotacaoDir(int T, No *y)`: Executam as rotações à esquerda e à direita para o balanceamento da árvore.

## Arquivos do Projeto

Todo o código-fonte foi desenvolvido de forma monolítica para facilitar a compilação e a gestão da memória.

* **`main.c`**: Contém a definição da `struct`, o vetor de versões globais, todas as funções listadas acima e a função `main` responsável pelo controle de fluxo, leitura de arquivos e interpretação dos comandos (`INC`, `REM`, `SUC`, `IMP`).
* **`makefile`**: Script de automação que define as rotinas de compilação (`build`) e execução (`run`) do programa, manipulando a entrada de arquivos de forma dinâmica.