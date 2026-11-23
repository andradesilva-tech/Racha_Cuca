// ListaOrdenada.h
// Lista DINAMICA, DUPLAMENTE ENCADEADA e ORDENADA de pares (prioridade, valor).
// Os elementos ficam sempre em ordem CRESCENTE de prioridade: o de menor
// prioridade fica no inicio.
//
// No projeto, e usada pelo A*: a prioridade e "quanto parece faltar" para
// resolver, e o A* sempre continua pelo estado mais promissor (o primeiro).
//
// Por que DUPLAMENTE encadeada? Para poder procurar o lugar de um novo
// elemento tanto a partir do inicio quanto a partir do FIM. No A*, os
// elementos novos quase sempre vao parar perto de uma das pontas, entao a
// insercao fica muito rapida (sem percorrer a lista inteira).
#ifndef LISTAORDENADA_H
#define LISTAORDENADA_H

struct NoLista {
    int prioridade;
    int valor;
    NoLista *ant;     // elemento anterior (de prioridade menor ou igual)
    NoLista *prox;    // proximo elemento (de prioridade maior ou igual)
};

struct ListaOrdenada {
    NoLista *inicio;  // elemento de MENOR prioridade
    NoLista *fim;     // elemento de MAIOR prioridade
    int tamanho;
};

// Deixa a lista pronta para uso (vazia).
void criarLista(ListaOrdenada &l);

// true se a lista nao tem nenhum elemento.
bool listaVazia(const ListaOrdenada &l);

// Insere o par na posicao certa, mantendo a lista ordenada.
// Empates: se a prioridade e menor OU IGUAL a do primeiro elemento, o novo
// entra no comeco (na frente dos iguais). Nos outros casos, o novo entra
// DEPOIS dos que ja tem a mesma prioridade.
void inserirOrdenado(ListaOrdenada &l, int prioridade, int valor);

// Tira o PRIMEIRO elemento (o de menor prioridade) e devolve seus dados.
// Devolve false se a lista estiver vazia.
bool removerPrimeiro(ListaOrdenada &l, int &prioridade, int &valor);

// Remove todos os elementos e libera a memoria.
void liberarLista(ListaOrdenada &l);

#endif
