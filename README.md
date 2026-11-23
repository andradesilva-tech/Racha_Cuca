# Simulador Racha Cuca — SIN 216

Simulador do quebra-cabeça deslizante (Racha Cuca). O programa carrega uma
imagem, divide em N×N peças, remove o canto inferior direito, embaralha e
resolve mostrando os passos no terminal.

> **Status:** Etapa 2 concluída. Jogo no terminal (leitura e validação de N,
> embaralhamento com movimentos aleatórios válidos, solução passo a passo),
> estruturas de dados próprias (Fila, Pilha, Tabela Hash, Lista ordenada) e
> solucionadores (BFS para N = 2 e 3, A* para N = 4, camadas para N = 5 e 6).
> A parte da imagem entra numa próxima etapa.

## Autor

João Marcos Andrade e Silva — Matrícula: `9945`
Gabriel de Sousa Siqueira - Matrícula: `99456`

## Requisitos e dependências

- Compilador C++ com suporte a C++14 (testado com **g++ 6.3.0** do MinGW)
- **CMake** 3.10 ou superior
- **Ninja** (gerador de build) — necessário no MinGW quando o caminho do
  projeto contém espaços, pois o `mingw32-make` antigo falha nesse caso
- Sistema operacional: Windows (desenvolvido no Windows 11)

Nenhuma biblioteca externa é usada: as estruturas de dados são todas
implementadas no próprio projeto.

## Como compilar

### Opção A — CMake + Ninja (recomendada)

```sh
cmake -S codigo -B build -G Ninja
cmake --build build
```

O executável `racha_cuca` e os executáveis de teste são gerados em `build/`.

> Em um caminho **sem espaços** também funciona o gerador padrão
> (`cmake -S codigo -B build -G "MinGW Makefiles"`). Com espaços no caminho,
> use Ninja.

### Opção B — g++ direto (sem CMake)

```sh
cd codigo
g++ -std=c++14 -O2 -Wall -Wextra -o ../build/racha_cuca main.cpp Tabuleiro.cpp Fila.cpp Pilha.cpp TabelaHash.cpp ListaOrdenada.cpp Solucionador.cpp
```

## Como executar

```sh
build/racha_cuca
```

O programa pede o tamanho N (de 2 a 6). Entradas inválidas (texto, número
com vírgula/ponto, valores fora de 2 a 6) são recusadas com uma mensagem e o
programa pede de novo. Depois:

1. cria o tabuleiro resolvido e o **embaralha** com movimentos aleatórios
   válidos (por isso a configuração sempre tem solução);
2. **resolve** escolhendo a estratégia pelo tamanho;
3. mostra a **solução passo a passo**: qual peça mover e como fica o tabuleiro.

Exemplo (N = 3):

```
Digite o tamanho N do tabuleiro (de 2 a 6): 3

Tabuleiro embaralhado (3x3):
 	4	2
7	6	3
1	5	8

Resolvendo...
Solucao encontrada com 18 movimentos, usando busca em largura (BFS).

Passo 1 de 18: mover a peca 7
7	4	2
 	6	3
1	5	8
...
Tabuleiro resolvido!
```

| N | Estratégia | Estruturas usadas | Solução mínima? |
|---|---|---|---|
| 2 e 3 | Busca em largura (BFS) | Fila, Tabela Hash, Pilha | sim |
| 4 | A* com distância de Manhattan | Lista ordenada, Tabela Hash, Pilha | sim |
| 5 e 6 | Estratégia por camadas | BFS reduzida (Fila, Tabela Hash, Pilha) | não |

## Como rodar os testes

```sh
ctest --test-dir build --output-on-failure
```

São 6 executáveis de teste; cada estrutura é testada **sozinha**:

| Teste (ctest) | Arquivo | O que confere |
|---|---|---|
| `tabuleiro` | `testes.cpp` | movimentos no centro, bordas e cantos; embaralhar (cada peça uma vez, um único vazio, nunca resolvido) |
| `fila` | `testes_fila.cpp` | ordem FIFO, fila vazia, muitos elementos |
| `pilha` | `testes_pilha.cpp` | ordem LIFO, inversão de sequência, pilha vazia |
| `tabela_hash` | `testes_hash.cpp` | inserção/busca, chave repetida, colisões, chaves enormes |
| `lista_ordenada` | `testes_lista.cpp` | ordenação, empates, ligações `ant`/`prox`, inserções sorteadas |
| `solucionador` | `testes_solucionador.cpp` | BFS (N=2,3), A* (N=4), camadas (N=5,6): todo movimento válido e tabuleiro resolvido no fim |

Cada executável devolve código de saída `0` quando todos os testes passam.

> **Windows 11 com "Controle Inteligente de Aplicativos" (Smart App Control)
> ligado:** às vezes o Windows bloqueia um executável recém-compilado e o
> `ctest` mostra `Not Run` / `BAD_COMMAND`. Não é falha do código. Compilar
> uma versão Debug em outra pasta costuma resolver:
> `cmake -S codigo -B build/debug -G Ninja -DCMAKE_BUILD_TYPE=Debug`,
> `cmake --build build/debug` e `ctest --test-dir build/debug`.

## Estrutura do projeto

```
codigo/
  Tabuleiro.{h,cpp}      estado, movimentos válidos, aplicar movimento,
                         verificar solução, imprimir, embaralhar (não conhece imagem)
  Fila.{h,cpp}           fila dinâmica (encadeada) de inteiros
  Pilha.{h,cpp}          pilha dinâmica (encadeada) de inteiros
  TabelaHash.{h,cpp}     tabela hash (método da divisão, colisões por encadeamento)
  ListaOrdenada.{h,cpp}  lista ordenada duplamente encadeada (usada pelo A*)
  Solucionador.{h,cpp}   BFS, A*, estratégia por camadas
  Imagem.{h,cpp}         carregar e recortar a imagem em N×N  (próxima etapa)
  main.cpp               leitura e validação de N, jogo no terminal
  testes*.cpp            executáveis de teste
  CMakeLists.txt         configuração de build
imagens/                 imagens de teste
relatorio/               relatório em PDF
```

## Limitações conhecidas

- A parte da **imagem** ainda não foi implementada (próxima etapa).
- **A\* no 4×4:** com o embaralhamento usado pelo programa (40 movimentos), a
  solução sai em menos de 2 segundos nos testes feitos. Em embaralhamentos
  muito longos, o A* pode passar do limite de 2 milhões de estados; nesse caso
  o programa avisa e usa a estratégia por camadas (que resolve, mas sem
  garantir o número mínimo de movimentos).
- A **estratégia por camadas** (N = 5 e 6) é rápida, mas não garante o menor
  número de movimentos.

## Materiais consultados

- *(a preencher)*
