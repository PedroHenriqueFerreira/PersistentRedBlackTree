# Árvore Rubro-Negra com Persistência Parcial

Este repositório contém a implementação de uma Árvore Binária de Busca balanceada (Rubro-Negra) com persistência parcial. A estrutura mantém o histórico de suas modificações (inclusões e remoções), permitindo consultas em versões anteriores.

## Linguagem de Programação Usada

* **Linguagem:** C
* **Versão:** 15.2.0 

## Como Rodar o Seu Programa

O projeto inclui um `makefile` para automatizar e facilitar a compilação e execução. O programa lê os comandos a partir de um arquivo de texto (`.txt`).

Abra o terminal no diretório onde os arquivos do código estão localizados e utilize os comandos abaixo:

1. **Compilação (Build):**
   Para compilar o código-fonte e gerar o executável (`programa.exe`), execute:
   ```bash
   make build
   ```

2. **Execução Padrão (Run):**
   Para rodar o programa utilizando o arquivo de entrada padrão (`entrada.txt`), execute:
   ```bash
   make run
   ```

3. **Execução com Arquivo Personalizado:**
   Se você quiser testar com um arquivo diferente (por exemplo, `entrada2.txt`), basta atribuir o nome do arquivo à variável `INPUT` durante o comando:
   ```bash
   make run INPUT=entrada2.txt
   ```

*(Alternativa manual: Caso não queira usar o makefile, você pode compilar manualmente com `gcc -o programa.exe main.c` e executar com `./programa.exe entrada.txt`)*

## Descrição das Estruturas e Funções

Em vez de utilizar uma `struct` tradicional com ponteiros, a estrutura dos nós da árvore foi implementada utilizando **vetores paralelos**. Isso facilita a persistência e a clonagem dos nós, referenciando-os por índices inteiros em vez de endereços de memória.

### Estruturas (Vetores Paralelos)
* `raiz[]`: Armazena o índice do nó raiz para cada versão gerada da árvore.
* `chave[]`: Armazena o valor (conteúdo) inteiro de cada nó.
* `cor[]`: Armazena a cor do nó (`RUBRO` ou `NEGRO`).
* `esq[]` e `dir[]`: Armazenam os índices dos filhos esquerdo e direito, respectivamente.
* `pai[]`: Armazena o índice do nó pai.

### Funções Principais
* `SucessorCustom(int T, int k)`: Busca o sucessor estritamente maior do nó com chave `k` na versão `T` da árvore.
* `Copiar(No *x, No *pai_x)`: Realiza uma cópia profunda recursiva de uma árvore ou subárvore, gerando novos índices para garantir a persistência da versão anterior.
* `Buscar(No *x, int k)`: Busca um nó com a chave `k` a partir do nó `x`.
* `Minimo(int T, int x)`: Retorna o índice do nó com o menor valor na subárvore `x`.
* `Sucessor(int T, int x)`: Retorna o índice do nó que é o sucessor em ordem do nó `x`.
* `Visitar(int x, int altura)`: Imprime as informações de um nó formatadas como `chave,altura,cor`.
* `EmOrdem(int x, int altura)`: Realiza o percurso em-ordem na árvore para a operação de impressão (IMP).

### Funções de Manutenção da Árvore Rubro-Negra
* `Incluir(int T, int z)`: Insere um novo nó `z` na árvore da versão `T` mantendo as regras de árvore binária de busca.
* `ConsertarInclusao(int T, int z)`: Restaura as propriedades rubro-negras após uma inserção.
* `Remover(int T, int z)`: Remove o nó `z` da árvore na versão `T`.
* `Transplantar(int T, int u, int v)`: Função auxiliar que substitui a subárvore `u` pela subárvore `v`.
* `ConsertarRemocao(int T, int x)`: Restaura as propriedades rubro-negras após uma remoção.
* `RotacaoEsq(int T, int x)` / `RotacaoDir(int T, int y)`: Executam as rotações à esquerda e à direita para o balanceamento da árvore.

## Arquivos do Projeto

Todo o código-fonte foi desenvolvido de forma monolítica para facilitar a compilação e a manipulação dos arrays globais.

* **`main.c`**: Contém **todas** as estruturas (vetores paralelos) e todas as funções listadas acima, além da função `main` responsável pelo controle de fluxo, leitura de arquivos e interpretação dos comandos (`INC`, `REM`, `SUC`, `IMP`).
* **`makefile`**: Script de automação que define as rotinas de compilação (`build`) e execução (`run`) do programa, manipulando a entrada de arquivos de forma dinâmica.