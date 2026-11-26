#include "Tabuleiro.h"
#include <iostream>
#include <cstdlib>
using namespace std;

// Diz se as casas i e j estao lado a lado (horizontal ou vertical).
static bool saoVizinhos(int n, int i, int j) {
    int dif = i - j;
    if (dif < 0) {
        dif = -dif;
    }
    bool mesmaLinha = (i / n == j / n);
    if (mesmaLinha && dif == 1) {
        return true;
    }
    if (dif == n) {
        return true;           // uma linha acima ou abaixo
    }
    return false;
}

void criarTabuleiro(Tabuleiro &t, int n) {
    t.n = n;
    t.pecas = new int[n * n];
    for (int i = 0; i < n * n - 1; i++) {
        t.pecas[i] = i + 1;
    }
    t.pecas[n * n - 1] = VAZIO;
    t.vazio = n * n - 1;
}

void criarTabuleiroComEstado(Tabuleiro &t, int n, const int valores[]) {
    t.n = n;
    t.pecas = new int[n * n];
    t.vazio = 0;
    for (int i = 0; i < n * n; i++) {
        t.pecas[i] = valores[i];
        if (valores[i] == VAZIO) {
            t.vazio = i;
        }
    }
}

void destruirTabuleiro(Tabuleiro &t) {
    delete[] t.pecas;
    t.pecas = NULL;
    t.n = 0;
    t.vazio = 0;
}

int pecaEm(const Tabuleiro &t, int linha, int coluna) {
    return t.pecas[linha * t.n + coluna];
}

int movimentosValidos(const Tabuleiro &t, int movimentos[]) {
    int n = t.n;
    int linha = t.vazio / n;
    int coluna = t.vazio % n;
    int qtd = 0;

    if (linha > 0) {                   // peca acima do vazio
        movimentos[qtd] = t.pecas[(linha - 1) * n + coluna];
        qtd++;
    }
    if (linha < n - 1) {               // abaixo
        movimentos[qtd] = t.pecas[(linha + 1) * n + coluna];
        qtd++;
    }
    if (coluna > 0) {                  // a esquerda
        movimentos[qtd] = t.pecas[linha * n + (coluna - 1)];
        qtd++;
    }
    if (coluna < n - 1) {              // a direita
        movimentos[qtd] = t.pecas[linha * n + (coluna + 1)];
        qtd++;
    }
    return qtd;
}

bool aplicarMovimento(Tabuleiro &t, int valorPeca) {
    if (valorPeca == VAZIO) {
        return false;
    }
    int n = t.n;

    int idx = -1;
    for (int i = 0; i < n * n; i++) {
        if (t.pecas[i] == valorPeca) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        return false;
    }
    if (!saoVizinhos(n, idx, t.vazio)) {
        return false;
    }

    // Troca a peca com o vazio.
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
                cout << " ";
            } else {
                cout << v;
            }
            cout << "\t";
        }
        cout << "\n";
    }
}

int embaralhar(Tabuleiro &t, int quantidade) {
    int ultimaMovida = VAZIO;          // nenhuma peca movida ainda
    int feitos = 0;

    while (feitos < quantidade || estaResolvido(t)) {
        int movimentos[4];
        int qtd = movimentosValidos(t, movimentos);

        // Tira das opcoes a ultima peca movida: mover ela de novo desfaria o passo.
        int opcoes[4];
        int qtdOpcoes = 0;
        for (int i = 0; i < qtd; i++) {
            if (movimentos[i] != ultimaMovida) {
                opcoes[qtdOpcoes] = movimentos[i];
                qtdOpcoes++;
            }
        }

        // Sempre sobra opcao: o vazio tem no minimo 2 vizinhos e so tiramos 1.
        int escolhida = opcoes[rand() % qtdOpcoes];
        aplicarMovimento(t, escolhida);
        ultimaMovida = escolhida;
        feitos++;
    }
    return feitos;
}
