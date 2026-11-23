// TabelaHash.cpp
#include "TabelaHash.h"
#include <cstdlib>

void criarTabelaHash(TabelaHash &h, int tamanho) {
    h.tamanho = tamanho;
    h.quantidade = 0;
    h.posicoes = new NoHash*[tamanho];   // vetor de ponteiros (alocacao dinamica)
    for (int i = 0; i < tamanho; i++) {
        h.posicoes[i] = NULL;             // toda lista comeca vazia
    }
}

int funcaoHash(const TabelaHash &h, unsigned long long chave) {
    // Metodo da divisao: o resto da divisao fica sempre entre 0 e tamanho-1.
    return (int)(chave % h.tamanho);
}

bool buscarHash(const TabelaHash &h, unsigned long long chave, int &valor) {
    int pos = funcaoHash(h, chave);
    NoHash *atual = h.posicoes[pos];
    // Percorre a lista daquela posicao procurando a chave exata.
    while (atual != NULL) {
        if (atual->chave == chave) {
            valor = atual->valor;
            return true;
        }
        atual = atual->prox;
    }
    return false;
}

bool inserirHash(TabelaHash &h, unsigned long long chave, int valor) {
    int jaExiste;
    if (buscarHash(h, chave, jaExiste)) {
        return false;                     // nao guarda chave repetida
    }
    int pos = funcaoHash(h, chave);
    NoHash *novo = new NoHash;
    novo->chave = chave;
    novo->valor = valor;
    novo->prox = h.posicoes[pos];         // entra no COMECO da lista da posicao
    h.posicoes[pos] = novo;
    h.quantidade++;
    return true;
}

void liberarTabelaHash(TabelaHash &h) {
    for (int i = 0; i < h.tamanho; i++) {
        NoHash *atual = h.posicoes[i];
        while (atual != NULL) {
            NoHash *proximo = atual->prox;  // guarda antes de apagar
            delete atual;
            atual = proximo;
        }
    }
    delete[] h.posicoes;
    h.posicoes = NULL;
    h.tamanho = 0;
    h.quantidade = 0;
}
