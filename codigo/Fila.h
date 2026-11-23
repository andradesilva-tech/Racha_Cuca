// Fila.h
// Fila DINAMICA (encadeada) de inteiros.
// Regra da fila: o primeiro que entra e o primeiro que sai (FIFO).
// No projeto, e usada pela busca em largura (BFS): os estados sao visitados
// na ordem em que foram descobertos.
#ifndef FILA_H
#define FILA_H

// Cada elemento da fila e um "no" que aponta para o proximo.
struct NoFila {
    int valor;
    NoFila *prox;
};

struct Fila {
    NoFila *inicio;   // primeiro da fila (e por aqui que se RETIRA)
    NoFila *fim;      // ultimo da fila (e por aqui que se INSERE)
    int tamanho;      // quantos elementos tem na fila
};

// Deixa a fila pronta para uso (vazia).
void criarFila(Fila &f);

// true se a fila nao tem nenhum elemento.
bool filaVazia(const Fila &f);

// Coloca 'valor' no FIM da fila.
void enfileirar(Fila &f, int valor);

// Tira o elemento do INICIO da fila e devolve em 'valor'.
// Devolve false se a fila estiver vazia (nesse caso 'valor' nao muda).
bool desenfileirar(Fila &f, int &valor);

// Remove todos os elementos e libera a memoria.
void liberarFila(Fila &f);

#endif
