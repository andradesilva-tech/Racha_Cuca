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
    novo->prox = NULL;

    if (f.fim == NULL) {
        f.inicio = novo;            // fila vazia: o novo tambem e o primeiro
    } else {
        f.fim->prox = novo;
    }
    f.fim = novo;
    f.tamanho++;
}

bool desenfileirar(Fila &f, int &valor) {
    if (f.inicio == NULL) {
        return false;
    }
    NoFila *removido = f.inicio;
    valor = removido->valor;
    f.inicio = removido->prox;
    if (f.inicio == NULL) {
        f.fim = NULL;               // ficou vazia
    }
    delete removido;
    f.tamanho--;
    return true;
}

void liberarFila(Fila &f) {
    int descartado;
    while (desenfileirar(f, descartado)) {
    }
}
