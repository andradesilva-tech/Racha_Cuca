// Tabuleiro.cpp
#include "Tabuleiro.h"
#include <iostream>
using namespace std;

// Funcao auxiliar: diz se os indices i e j sao vizinhos ortogonais (lado a
// lado) num tabuleiro n x n. Usada so aqui dentro.
static bool saoVizinhos(int n, int i, int j) {
    int dif = i - j;
    if (dif < 0) {
        dif = -dif;            // valor absoluto, feito na mao (sem biblioteca)
    }
    bool mesmaLinha = (i / n == j / n);
    if (mesmaLinha && dif == 1) {
        return true;           // vizinhos na horizontal
    }
    if (dif == n) {
        return true;           // vizinhos na vertical (uma linha de diferenca)
    }
    return false;
}

void criarTabuleiro(Tabuleiro &t, int n) {
    t.n = n;
    t.pecas = new int[n * n];
    for (int i = 0; i < n * n - 1; i++) {
        t.pecas[i] = i + 1;            // 1, 2, 3, ...
    }
    t.pecas[n * n - 1] = VAZIO;        // vazio no canto inferior direito
    t.vazio = n * n - 1;
}

void criarTabuleiroComEstado(Tabuleiro &t, int n, const int valores[]) {
    t.n = n;
    t.pecas = new int[n * n];
    t.vazio = 0;
    for (int i = 0; i < n * n; i++) {
        t.pecas[i] = valores[i];
        if (valores[i] == VAZIO) {
            t.vazio = i;               // guarda onde esta o espaco vazio
        }
    }
}

void destruirTabuleiro(Tabuleiro &t) {
    delete[] t.pecas;
    t.pecas = 0;                       // ponteiro nulo (evita uso indevido)
    t.n = 0;
    t.vazio = 0;
}

int pecaEm(const Tabuleiro &t, int linha, int coluna) {
    return t.pecas[linha * t.n + coluna];
}

int movimentosValidos(const Tabuleiro &t, int movimentos[]) {
    int n = t.n;
    int linha = t.vazio / n;           // linha do espaco vazio
    int coluna = t.vazio % n;          // coluna do espaco vazio
    int qtd = 0;

    if (linha > 0) {                   // ha uma peca acima do vazio
        movimentos[qtd] = t.pecas[(linha - 1) * n + coluna];
        qtd++;
    }
    if (linha < n - 1) {               // ha uma peca abaixo do vazio
        movimentos[qtd] = t.pecas[(linha + 1) * n + coluna];
        qtd++;
    }
    if (coluna > 0) {                  // ha uma peca a esquerda do vazio
        movimentos[qtd] = t.pecas[linha * n + (coluna - 1)];
        qtd++;
    }
    if (coluna < n - 1) {              // ha uma peca a direita do vazio
        movimentos[qtd] = t.pecas[linha * n + (coluna + 1)];
        qtd++;
    }
    return qtd;
}

bool aplicarMovimento(Tabuleiro &t, int valorPeca) {
    if (valorPeca == VAZIO) {
        return false;                  // nao se "move" o proprio vazio
    }
    int n = t.n;

    // Procura em que posicao esta a peca pedida.
    int idx = -1;
    for (int i = 0; i < n * n; i++) {
        if (t.pecas[i] == valorPeca) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        return false;                  // peca nao existe no tabuleiro
    }
    if (!saoVizinhos(n, idx, t.vazio)) {
        return false;                  // a peca nao esta ao lado do vazio
    }

    // Desliza a peca para o vazio: troca os dois.
    t.pecas[t.vazio] = valorPeca;
    t.pecas[idx] = VAZIO;
    t.vazio = idx;
    return true;
}

bool estaResolvido(const Tabuleiro &t) {
    int total = t.n * t.n;
    for (int i = 0; i < total - 1; i++) {
        if (t.pecas[i] != i + 1) {
            return false;
        }
    }
    return t.pecas[total - 1] == VAZIO;
}

void imprimirTabuleiro(const Tabuleiro &t) {
    int n = t.n;
    for (int linha = 0; linha < n; linha++) {
        for (int coluna = 0; coluna < n; coluna++) {
            int v = t.pecas[linha * n + coluna];
            if (v == VAZIO) {
                cout << " ";           // espaco vazio aparece em branco
            } else {
                cout << v;
            }
            cout << "\t";              // tabulacao para alinhar as colunas
        }
        cout << "\n";
    }
}
