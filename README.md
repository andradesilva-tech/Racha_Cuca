# Simulador Racha Cuca — SIN 216

Simulador do quebra-cabeça deslizante (Racha Cuca). O programa carrega uma
imagem, divide em N×N peças, remove o canto inferior direito, embaralha e
resolve mostrando os passos no terminal.

> **Status:** Etapa 1 (Base) concluída — estrutura, CMake, `Tabuleiro`
> (representação, movimentos válidos, aplicação de movimento, verificação de
> solução, impressão) e testes de movimentos no centro, nas bordas e nos cantos.

## Autor

João Marcos Andrade e Silva — Matrícula: `9945`
Gabriel de Sousa Siqueira - Matrícula: `99456`

## Requisitos e dependências

- Compilador C++ com suporte a C++14 (testado com **g++ 6.3.0** do MinGW)
- **CMake** 3.10 ou superior
- **Ninja** (gerador de build) — necessário no MinGW quando o caminho do
  projeto contém espaços, pois o `mingw32-make` antigo falha nesse caso
- Sistema operacional: Windows (desenvolvido no Windows 11)

## Como compilar

### Opção A — CMake + Ninja (recomendada)

```sh
cmake -S codigo -B build -G Ninja
cmake --build build
```

Os executáveis `racha_cuca` e `testes` são gerados em `build/`.

> Em um caminho **sem espaços** também funciona o gerador padrão
> (`cmake -S codigo -B build -G "MinGW Makefiles"`). Com espaços no caminho,
> use Ninja.

### Opção B — g++ direto (sem CMake)

```sh
g++ -std=c++14 -Wall -Wextra -o build/racha_cuca codigo/main.cpp codigo/Tabuleiro.cpp
g++ -std=c++14 -Wall -Wextra -o build/testes     codigo/testes.cpp codigo/Tabuleiro.cpp
```

## Como executar

```sh
build/racha_cuca
```

Na Etapa 1 o programa apenas imprime `ok` (confirma que a base compila e roda).
A leitura de imagem, o embaralhamento e a solução passo a passo entram nas
próximas etapas.

## Como rodar os testes

Com CMake:

```sh
ctest --test-dir build --output-on-failure
```

Ou executando o binário diretamente:

```sh
build/testes
```

A suíte retorna código de saída `0` quando todos os testes passam.

## Estrutura do projeto

```
codigo/
  Tabuleiro.{h,cpp}     estado, movimentos válidos, aplicar movimento,
                        verificar solução, imprimir (não conhece imagem)
  Imagem.{h,cpp}        carregar e recortar a imagem em N×N  (próximas etapas)
  Solucionador.{h,cpp}  lista de movimentos da solução       (próximas etapas)
  main.cpp              ponto de entrada do simulador
  testes.cpp            executável de testes
  CMakeLists.txt        configuração de build
imagens/                imagens de teste
relatorio/              relatório em PDF
```

## Limitações conhecidas

- Etapa 1: apenas o núcleo lógico do tabuleiro está implementado.

## Materiais consultados

- *(a preencher)*
