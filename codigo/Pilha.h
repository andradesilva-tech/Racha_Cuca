// Pilha.h
// Pilha DINAMICA (encadeada) de inteiros.
// Regra da pilha: o ultimo que entra e o primeiro que sai (LIFO).
// No projeto, e usada para MONTAR a sequencia de movimentos da solucao:
// a busca descobre os movimentos de tras para frente (do fim ate o comeco);
// empilhando todos e depois desempilhando, eles saem na ordem certa.
#ifndef PILHA_H
#define PILHA_H

struct NoPilha {
    int valor;
    NoPilha *prox;    // elemento de baixo
};

struct Pilha {
    NoPilha *topo;    // elemento de cima (o unico que da para tirar)
    int tamanho;
};

// Deixa a pilha pronta para uso (vazia).
void criarPilha(Pilha &p);

// true se a pilha nao tem nenhum elemento.
bool pilhaVazia(const Pilha &p);

// Coloca 'valor' no TOPO da pilha.
void empilhar(Pilha &p, int valor);

// Tira o elemento do TOPO e devolve em 'valor'.
// Devolve false se a pilha estiver vazia (nesse caso 'valor' nao muda).
bool desempilhar(Pilha &p, int &valor);

// Remove todos os elementos e libera a memoria.
void liberarPilha(Pilha &p);

#endif
