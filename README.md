# Simulador Racha Cuca — SIN 216

Simulador do quebra-cabeça deslizante (Racha Cuca). O programa carrega uma
imagem, divide em N×N peças, remove o canto inferior direito, embaralha e
resolve mostrando os passos no terminal. As imagens dos estados inicial e
final são salvas em arquivo.

## Autor

João Marcos Andrade e Silva — Matrícula: `9945`
Gabriel de Sousa Siqueira - Matrícula: `99456`

## Requisitos e dependências

- Compilador C++ com suporte a C++14 (testado com **g++ 6.3.0** do MinGW)
- **CMake** 3.10 ou superior
- **Ninja** (gerador de build) — necessário no MinGW quando o caminho do
  projeto contém espaços, pois o `mingw32-make` antigo falha nesse caso
- Sistema operacional: Windows (desenvolvido no Windows 11)
- Para **ver** as imagens `.ppm`: GIMP, IrfanView ou uma extensão de
  visualização PPM no VS Code (o Visualizador de Fotos do Windows não abre PPM)

Nenhuma biblioteca externa é usada. Só a biblioteca padrão do C++
(`iostream`, `fstream`, `string`, `cstdlib`, `ctime`, `cstdio`); as estruturas
de dados são todas implementadas no próprio projeto.

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
mkdir build
cd codigo
g++ -std=c++14 -O2 -Wall -Wextra -o ../build/racha_cuca main.cpp Tabuleiro.cpp Fila.cpp Pilha.cpp TabelaHash.cpp ListaOrdenada.cpp Solucionador.cpp Imagem.cpp Entrada.cpp
```

## Como executar

Rode de dentro da pasta `build/`, para os caminhos relativos das imagens funcionarem:

```sh
cd build
./racha_cuca
```

O programa pergunta:

1. o tamanho **N** (de 2 a 6). Texto, número com vírgula ou ponto e valores
   fora de 2 a 6 são recusados com uma mensagem, e o programa pergunta de novo;
2. o **caminho da imagem** PPM, por exemplo `../imagens/exemplo.ppm`. Se o
   arquivo não existir ou não for PPM texto (P3), o programa avisa e pergunta
   de novo. Caminhos entre aspas também são aceitos.

Depois ele:

1. recorta a imagem em N×N peças e retira a do canto inferior direito;
2. **embaralha** o tabuleiro com movimentos aleatórios válidos (por isso a
   configuração sempre tem solução) e salva `estado_inicial.ppm`;
3. **resolve**, escolhendo a estratégia pelo tamanho;
4. mostra a **solução passo a passo** no terminal (qual peça mover e como fica
   o tabuleiro) e salva `estado_final.ppm`.

Os arquivos `estado_inicial.ppm` e `estado_final.ppm` ficam na pasta onde o
programa foi executado. O espaço vazio aparece preto nas duas imagens.

Exemplo (N = 3, imagem com sobra):

```
Digite o tamanho N do tabuleiro (de 2 a 6): 3
Digite o caminho da imagem PPM (formato P3): ../imagens/exemplo_sobra.ppm
Imagem de 125x97 pixels; cada peca tera 41x32.
Aviso: as dimensoes nao sao multiplas de 3. Foram descartados 2 pixel(s) a direita e 1 embaixo.

Tabuleiro embaralhado (3x3):
8	3	 	
6	1	5	
7	4	2	
Imagem do estado inicial salva em estado_inicial.ppm

Resolvendo...
Solucao encontrada com 24 movimentos, usando busca em largura (BFS).
...
Passo 24 de 24: mover a peca 6
1	2	3	
4	5	6	
7	8	 	
Tabuleiro resolvido!
Imagem do estado final salva em estado_final.ppm
```

| N | Estratégia | Estruturas usadas | Solução mínima? |
|---|---|---|---|
| 2 e 3 | Busca em largura (BFS) | Fila, Tabela Hash, Pilha | sim |
| 4 | A* com distância de Manhattan | Lista ordenada, Tabela Hash, Pilha | sim |
| 5 e 6 | Estratégia por camadas | BFS reduzida (Fila, Tabela Hash, Pilha) | não |

## Imagens

O programa lê imagens **PPM texto (P3)**. A pasta `imagens/` tem duas imagens
de teste, geradas para o projeto (degradê de cores com um círculo no meio):

| Arquivo | Tamanho | Para que serve |
|---|---|---|
| `imagens/exemplo.ppm` | 120×120 | divide exatamente por N = 2, 3, 4, 5 e 6 |
| `imagens/exemplo_sobra.ppm` | 125×97 | não é múltipla de nenhum N: mostra o tratamento da sobra |

**Dimensões não múltiplas de N:** cada peça tem `largura/N × altura/N`
(divisão inteira) e os pixels que sobram na borda direita e na de baixo são
descartados. O programa avisa quantos pixels foram descartados.

**Para usar outra imagem:** abra a foto no GIMP, use *Arquivo → Exportar como*,
escolha o nome terminado em `.ppm` e, na janela seguinte, marque **ASCII**.

## Como rodar os testes

```sh
ctest --test-dir build --output-on-failure
```

São 9 executáveis de teste:

| Teste (ctest) | Arquivo | O que confere |
|---|---|---|
| `tabuleiro` | `testes.cpp` | movimentos no centro, nas bordas e nos cantos; movimento inválido não muda nada; embaralhar (cada peça uma vez, um único vazio, nunca resolvido) |
| `fila` | `testes_fila.cpp` | ordem FIFO, fila vazia, muitos elementos |
| `pilha` | `testes_pilha.cpp` | ordem LIFO, inversão de sequência, pilha vazia |
| `tabela_hash` | `testes_hash.cpp` | inserção/busca, chave repetida, colisões, chaves enormes |
| `lista_ordenada` | `testes_lista.cpp` | ordenação, empates, ligações `ant`/`prox`, inserções sorteadas |
| `solucionador` | `testes_solucionador.cpp` | BFS (N=2,3), A* (N=4), camadas (N=5,6): todo movimento válido e tabuleiro resolvido no fim |
| `imagem` | `testes_imagem.cpp` | ler/salvar PPM, comentários, arquivo inexistente ou inválido, recorte com e sem sobra, montagem |
| `entrada` | `testes_entrada.cpp` | textos que não são número (`abc`, `3.5`, vazio...) e N não aceito (1, 0, 7, negativo...) |
| `integracao` | `testes_integracao.cpp` | fluxo completo para N = 2 a 6 com imagem 125×97 (com sobra): na imagem inicial cada peça, menos a retirada, aparece uma única vez e há um só vazio; todos os movimentos exibidos são válidos; a imagem final é a original reconstruída |

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
  Imagem.{h,cpp}         ler/salvar PPM, recortar em N×N, montar a imagem de um tabuleiro
  Entrada.{h,cpp}        validação do texto digitado e do valor de N
  main.cpp               leitura das entradas, solução passo a passo, imagens dos estados
  testes*.cpp            executáveis de teste
  CMakeLists.txt         configuração de build
imagens/                 imagens de teste
relatorio/               relatório em PDF
```

## Limitações conhecidas

- Só lê **PPM texto (P3)**. PNG, JPG e PPM binário (P6) precisam ser
  convertidos antes (veja a seção Imagens). O tamanho máximo aceito é 4000×4000.
- Caminhos de arquivo com acento podem falhar no terminal do Windows por
  causa da codificação de caracteres; prefira caminhos relativos, como
  `../imagens/exemplo.ppm`.
- **A\* no 4×4:** com o embaralhamento usado pelo programa (40 movimentos), a
  solução sai em menos de 2 segundos nos testes feitos. Em embaralhamentos
  muito longos, o A* pode passar do limite de 2 milhões de estados; nesse caso
  o programa avisa e usa a estratégia por camadas (que resolve, mas sem
  garantir o número mínimo de movimentos).
- A **estratégia por camadas** (N = 5 e 6) é rápida, mas não garante o menor
  número de movimentos.

## Materiais consultados

- *(a preencher)*
