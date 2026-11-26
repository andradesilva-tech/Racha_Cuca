// Lista duplamente encadeada de pares (prioridade, valor), sempre em ordem
// crescente de prioridade. Usada pelo A*: o primeiro e o estado mais promissor.
// Ser duplamente encadeada permite procurar o lugar do novo a partir do fim;
// no A* ele quase sempre fica perto de uma das pontas, entao inserir e rapido.
#ifndef LISTAORDENADA_H
#define LISTAORDENADA_H

struct NoLista {
    int prioridade;
    int valor;
    NoLista *ant;
    NoLista *prox;
};

struct ListaOrdenada {
    NoLista *inicio;  // menor prioridade
    NoLista *fim;     // maior prioridade
    int tamanho;
};

void criarLista(ListaOrdenada &l);
bool listaVazia(const ListaOrdenada &l);

// Empate com o primeiro: o novo entra na frente dele.
// Qualquer outro empate: o novo entra depois dos iguais.
void inserirOrdenado(ListaOrdenada &l, int prioridade, int valor);

bool removerPrimeiro(ListaOrdenada &l, int &prioridade, int &valor);   // false se vazia
void liberarLista(ListaOrdenada &l);

#endif
