// Fila encadeada de inteiros: o primeiro que entra e o primeiro que sai.
// Usada pela BFS para visitar os estados na ordem em que foram descobertos.
#ifndef FILA_H
#define FILA_H

struct NoFila {
    int valor;
    NoFila *prox;
};

struct Fila {
    NoFila *inicio;   // retira por aqui
    NoFila *fim;      // insere por aqui
    int tamanho;
};

void criarFila(Fila &f);
bool filaVazia(const Fila &f);
void enfileirar(Fila &f, int valor);
bool desenfileirar(Fila &f, int &valor);   // false se a fila estiver vazia
void liberarFila(Fila &f);

#endif
