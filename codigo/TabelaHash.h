// Tabela hash (chave -> valor) que responde rapido "ja vi esse estado?".
//   chave: o tabuleiro codificado num numero
//   valor: posicao desse estado no vetor de nos da busca
// Funcao hash pelo metodo da divisao (chave % tamanho). Colisoes tratadas
// por encadeamento: cada posicao guarda uma lista dos pares que cairam nela.
#ifndef TABELAHASH_H
#define TABELAHASH_H

struct NoHash {
    unsigned long long chave;
    int valor;
    NoHash *prox;
};

struct TabelaHash {
    NoHash **posicoes;  // cada posicao e o inicio de uma lista
    int tamanho;        // de preferencia primo, para espalhar melhor as chaves
    int quantidade;
};

void criarTabelaHash(TabelaHash &h, int tamanho);
int funcaoHash(const TabelaHash &h, unsigned long long chave);

// Nao guarda chave repetida: devolve false se ela ja existia.
bool inserirHash(TabelaHash &h, unsigned long long chave, int valor);

bool buscarHash(const TabelaHash &h, unsigned long long chave, int &valor);
void liberarTabelaHash(TabelaHash &h);

#endif
