// TabelaHash.h
// Tabela hash que guarda pares (chave -> valor).
//   chave: um numero inteiro grande (unsigned long long). No projeto, e o
//          estado do tabuleiro "codificado" num unico numero.
//   valor: um inteiro. No projeto, e a posicao do estado no vetor de nos da busca.
//
// Funcao hash: metodo da DIVISAO -> posicao = chave % tamanho.
// Colisoes (duas chaves caindo na mesma posicao): tratadas por ENCADEAMENTO,
// ou seja, cada posicao da tabela guarda uma lista encadeada de pares.
//
// Serve para responder rapido: "eu ja vi esse estado antes?".
#ifndef TABELAHASH_H
#define TABELAHASH_H

struct NoHash {
    unsigned long long chave;
    int valor;
    NoHash *prox;     // proximo par que caiu na mesma posicao
};

struct TabelaHash {
    NoHash **posicoes;  // vetor de ponteiros: cada posicao e o inicio de uma lista
    int tamanho;        // quantidade de posicoes do vetor (de preferencia um primo)
    int quantidade;     // quantos pares estao guardados no total
};

// Cria a tabela com 'tamanho' posicoes, todas vazias.
void criarTabelaHash(TabelaHash &h, int tamanho);

// Calcula em qual posicao (de 0 ate tamanho-1) a chave deve ficar.
int funcaoHash(const TabelaHash &h, unsigned long long chave);

// Guarda o par (chave -> valor). Devolve false se a chave JA existia
// (nesse caso nada muda: a tabela nao guarda chaves repetidas).
bool inserirHash(TabelaHash &h, unsigned long long chave, int valor);

// Procura a chave. Se achar, coloca o valor em 'valor' e devolve true.
// Se nao achar, devolve false.
bool buscarHash(const TabelaHash &h, unsigned long long chave, int &valor);

// Libera toda a memoria da tabela.
void liberarTabelaHash(TabelaHash &h);

#endif
