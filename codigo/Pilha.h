// Pilha encadeada de inteiros: o ultimo que entra e o primeiro que sai.
// Usada para montar a solucao: a busca acha os movimentos do fim para o
// comeco, e desempilhando eles saem na ordem certa.
#ifndef PILHA_H
#define PILHA_H

struct NoPilha {
    int valor;
    NoPilha *prox;    // elemento de baixo
};

struct Pilha {
    NoPilha *topo;
    int tamanho;
};

void criarPilha(Pilha &p);
bool pilhaVazia(const Pilha &p);
void empilhar(Pilha &p, int valor);
bool desempilhar(Pilha &p, int &valor);   // false se a pilha estiver vazia
void liberarPilha(Pilha &p);

#endif
