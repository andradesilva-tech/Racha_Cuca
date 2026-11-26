// Acha os movimentos que resolvem o tabuleiro. Cada movimento e o valor da
// peca que desliza para o vazio (o mesmo que aplicarMovimento recebe).
//   N = 2 e 3 -> BFS (Fila + Hash)
//   N = 4     -> A* (Lista ordenada + Hash)
//   N = 5 e 6 -> camadas (arruma linha de cima e coluna da esquerda e repete)
// Todas usam a Pilha para montar a sequencia final.
#ifndef SOLUCIONADOR_H
#define SOLUCIONADOR_H

#include "Tabuleiro.h"

// Vetor dinamico de movimentos que dobra de tamanho quando enche.
struct Solucao {
    int *movimentos;
    int quantidade;
    int capacidade;
};

void criarSolucao(Solucao &s);
void adicionarMovimento(Solucao &s, int movimento);
void liberarSolucao(Solucao &s);

// As tres funcoes abaixo nao alteram 't' e devolvem true se acharam a solucao.

// Menor numero de movimentos. So para N <= 4 (usada em N = 2 e 3).
bool resolverBFS(const Tabuleiro &t, Solucao &s);

// Menor numero de movimentos, olhando primeiro os estados mais promissores.
// So para N <= 4.
bool resolverAEstrela(const Tabuleiro &t, Solucao &s);

// Qualquer N >= 2. Rapida, mas nao garante o menor numero de movimentos.
bool resolverCamadas(const Tabuleiro &t, Solucao &s);

const int ESTRATEGIA_NENHUMA = 0;
const int ESTRATEGIA_BFS = 1;
const int ESTRATEGIA_AESTRELA = 2;
const int ESTRATEGIA_CAMADAS = 3;

// Escolhe a estrategia pelo N e devolve o codigo da que resolveu.
// Se o A* estourar o limite de estados, usa camadas no lugar.
int resolver(const Tabuleiro &t, Solucao &s);

const char *nomeEstrategia(int estrategia);

#endif
