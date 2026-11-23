// Pilha.cpp
#include "Pilha.h"
#include <cstdlib>

void criarPilha(Pilha &p) {
    p.topo = NULL;
    p.tamanho = 0;
}

bool pilhaVazia(const Pilha &p) {
    return p.topo == NULL;
}

void empilhar(Pilha &p, int valor) {
    NoPilha *novo = new NoPilha;
    novo->valor = valor;
    novo->prox = p.topo;            // o novo fica em cima do antigo topo
    p.topo = novo;
    p.tamanho++;
}

bool desempilhar(Pilha &p, int &valor) {
    if (p.topo == NULL) {
        return false;               // nada para tirar
    }
    NoPilha *removido = p.topo;
    valor = removido->valor;
    p.topo = removido->prox;        // o de baixo vira o topo
    delete removido;
    p.tamanho--;
    return true;
}

void liberarPilha(Pilha &p) {
    int descartado;
    while (desempilhar(p, descartado)) {
        // so esvazia; desempilhar ja libera cada no
    }
}
