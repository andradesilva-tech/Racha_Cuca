// Fila.cpp
#include "Fila.h"
#include <cstdlib>

void criarFila(Fila &f) {
    f.inicio = NULL;
    f.fim = NULL;
    f.tamanho = 0;
}

bool filaVazia(const Fila &f) {
    return f.inicio == NULL;
}

void enfileirar(Fila &f, int valor) {
    NoFila *novo = new NoFila;
    novo->valor = valor;
    novo->prox = NULL;              // o novo sera o ultimo, entao nao tem proximo

    if (f.fim == NULL) {
        f.inicio = novo;            // fila estava vazia: o novo e o primeiro
    } else {
        f.fim->prox = novo;         // engata o novo depois do ultimo atual
    }
    f.fim = novo;                   // e o novo passa a ser o ultimo
    f.tamanho++;
}

bool desenfileirar(Fila &f, int &valor) {
    if (f.inicio == NULL) {
        return false;               // nada para tirar
    }
    NoFila *removido = f.inicio;
    valor = removido->valor;
    f.inicio = removido->prox;      // o segundo vira o primeiro
    if (f.inicio == NULL) {
        f.fim = NULL;               // a fila ficou vazia
    }
    delete removido;
    f.tamanho--;
    return true;
}

void liberarFila(Fila &f) {
    int descartado;
    while (desenfileirar(f, descartado)) {
        // so esvazia; desenfileirar ja libera cada no
    }
}
