// Solucionador.h
// Encontra a sequencia de movimentos que leva um tabuleiro embaralhado ate o
// estado resolvido. Cada movimento e o VALOR da peca que desliza para o vazio
// (o mesmo formato usado por aplicarMovimento do Tabuleiro).
//
// Estrategias (escolhidas pelo tamanho N):
//   N = 2 e 3 -> busca em largura (BFS), com Fila + Tabela Hash
//   N = 4     -> A* com distancia de Manhattan, com Lista ordenada + Tabela Hash
//   N = 5 e 6 -> estrategia por camadas (resolve linha de cima e coluna da
//                esquerda, encolhe o problema e repete)
// Em todas, a Pilha e usada para montar a sequencia de movimentos no final.
#ifndef SOLUCIONADOR_H
#define SOLUCIONADOR_H

#include "Tabuleiro.h"

// Resultado: um vetor dinamico de movimentos que cresce conforme precisa.
struct Solucao {
    int *movimentos;   // valores das pecas a mover, na ordem
    int quantidade;    // quantos movimentos tem
    int capacidade;    // quantos cabem no vetor alocado no momento
};

// Deixa a solucao vazia e pronta para receber movimentos.
void criarSolucao(Solucao &s);

// Acrescenta um movimento no final (aumenta o vetor se estiver cheio).
void adicionarMovimento(Solucao &s, int movimento);

// Libera a memoria da solucao.
void liberarSolucao(Solucao &s);

// Cada funcao abaixo recebe o tabuleiro (que NAO e alterado) e uma solucao
// criada com criarSolucao. Devolve true se achou a solucao; os movimentos
// ficam em 's'.

// Busca em largura: acha o MENOR numero de movimentos. So aceita N <= 4
// (na pratica, usada para N = 2 e 3).
bool resolverBFS(const Tabuleiro &t, Solucao &s);

// A*: tambem acha o menor numero de movimentos, mas olhando primeiro os
// estados que parecem mais perto da solucao. So aceita N <= 4.
bool resolverAEstrela(const Tabuleiro &t, Solucao &s);

// Estrategia por camadas: funciona para qualquer N >= 2. Nao garante o menor
// numero de movimentos, mas resolve tabuleiros grandes rapidamente.
bool resolverCamadas(const Tabuleiro &t, Solucao &s);

// Codigos das estrategias (devolvidos por resolver).
const int ESTRATEGIA_NENHUMA = 0;
const int ESTRATEGIA_BFS = 1;
const int ESTRATEGIA_AESTRELA = 2;
const int ESTRATEGIA_CAMADAS = 3;

// Escolhe a estrategia certa pelo N do tabuleiro e resolve:
//   N = 2 e 3 -> BFS;  N = 4 -> A*;  N = 5 e 6 -> camadas.
// Se o A* passar do limite de estados (embaralhamentos muito dificeis), usa
// a estrategia por camadas no lugar, para nunca ficar sem resposta.
// Devolve o CODIGO da estrategia que resolveu (ESTRATEGIA_NENHUMA se falhou).
int resolver(const Tabuleiro &t, Solucao &s);

// Nome de uma estrategia a partir do seu codigo (para mostrar na tela).
const char *nomeEstrategia(int estrategia);

#endif
