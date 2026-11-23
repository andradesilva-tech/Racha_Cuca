// ListaOrdenada.cpp
#include "ListaOrdenada.h"
#include <cstdlib>

void criarLista(ListaOrdenada &l) {
    l.inicio = NULL;
    l.fim = NULL;
    l.tamanho = 0;
}

bool listaVazia(const ListaOrdenada &l) {
    return l.inicio == NULL;
}

void inserirOrdenado(ListaOrdenada &l, int prioridade, int valor) {
    NoLista *novo = new NoLista;
    novo->prioridade = prioridade;
    novo->valor = valor;
    novo->ant = NULL;
    novo->prox = NULL;

    if (l.inicio == NULL) {
        // Caso 1: lista vazia -> o novo e o unico elemento.
        l.inicio = novo;
        l.fim = novo;
    } else if (prioridade <= l.inicio->prioridade) {
        // Caso 2: o novo e tao bom ou melhor que o primeiro -> entra no comeco.
        novo->prox = l.inicio;
        l.inicio->ant = novo;
        l.inicio = novo;
    } else {
        // Caso 3: procura o lugar vindo do FIM, voltando enquanto o elemento
        // tiver prioridade MAIOR que a do novo. Para no ultimo com prioridade
        // menor ou igual. (Nunca passa do inicio, porque o primeiro tem
        // prioridade menor que a do novo, senao teria caido no caso 2.)
        NoLista *atual = l.fim;
        while (atual->prioridade > prioridade) {
            atual = atual->ant;
        }
        // Encaixa o novo logo DEPOIS de 'atual'.
        novo->ant = atual;
        novo->prox = atual->prox;
        if (atual->prox != NULL) {
            atual->prox->ant = novo;
        } else {
            l.fim = novo;              // 'atual' era o ultimo: o novo vira o fim
        }
        atual->prox = novo;
    }
    l.tamanho++;
}

bool removerPrimeiro(ListaOrdenada &l, int &prioridade, int &valor) {
    if (l.inicio == NULL) {
        return false;
    }
    NoLista *removido = l.inicio;
    prioridade = removido->prioridade;
    valor = removido->valor;

    l.inicio = removido->prox;
    if (l.inicio != NULL) {
        l.inicio->ant = NULL;          // o novo primeiro nao tem anterior
    } else {
        l.fim = NULL;                  // a lista ficou vazia
    }
    delete removido;
    l.tamanho--;
    return true;
}

void liberarLista(ListaOrdenada &l) {
    int p, v;
    while (removerPrimeiro(l, p, v)) {
        // so esvazia; removerPrimeiro ja libera cada no
    }
}
