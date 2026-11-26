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
        l.inicio = novo;
        l.fim = novo;
    } else if (prioridade <= l.inicio->prioridade) {
        novo->prox = l.inicio;
        l.inicio->ant = novo;
        l.inicio = novo;
    } else {
        // Volta a partir do fim ate achar alguem com prioridade <= a do novo.
        // Nunca passa do inicio: o primeiro e menor, senao cairia no caso acima.
        NoLista *atual = l.fim;
        while (atual->prioridade > prioridade) {
            atual = atual->ant;
        }
        // Encaixa o novo logo depois de 'atual'.
        novo->ant = atual;
        novo->prox = atual->prox;
        if (atual->prox != NULL) {
            atual->prox->ant = novo;
        } else {
            l.fim = novo;
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
        l.inicio->ant = NULL;
    } else {
        l.fim = NULL;                  // ficou vazia
    }
    delete removido;
    l.tamanho--;
    return true;
}

void liberarLista(ListaOrdenada &l) {
    int p, v;
    while (removerPrimeiro(l, p, v)) {
    }
}
