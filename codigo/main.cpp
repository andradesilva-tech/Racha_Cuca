// main.cpp
// Jogo no terminal: le o tamanho N, valida, embaralha o tabuleiro com
// movimentos aleatorios validos e mostra a solucao passo a passo.
// (A parte da imagem entra numa proxima etapa.)
#include "Tabuleiro.h"
#include "Solucionador.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

const int N_MINIMO = 2;
const int N_MAXIMO = 6;

// Converte o texto digitado num numero inteiro, caractere por caractere.
// Aceita um sinal de menos no comeco. Devolve false se tiver qualquer coisa
// que nao seja digito (ex.: "abc", "3.5", "4x") ou se for grande demais.
bool textoParaInteiro(const string &texto, int &numero) {
    int inicio = 0;
    bool negativo = false;
    if (texto.size() > 0 && texto[0] == '-') {
        negativo = true;
        inicio = 1;
    }
    int qtdDigitos = (int)texto.size() - inicio;
    if (qtdDigitos < 1 || qtdDigitos > 9) {
        return false;            // vazio, so "-", ou numero grande demais
    }
    numero = 0;
    for (int i = inicio; i < (int)texto.size(); i++) {
        char c = texto[i];
        if (c < '0' || c > '9') {
            return false;
        }
        numero = numero * 10 + (c - '0');
    }
    if (negativo) {
        numero = -numero;
    }
    return true;
}

// Le N do teclado ate receber um valor valido.
// Devolve -1 se a entrada acabar antes disso.
int lerN() {
    string entrada;
    while (true) {
        cout << "Digite o tamanho N do tabuleiro (de " << N_MINIMO
             << " a " << N_MAXIMO << "): ";
        if (!(cin >> entrada)) {
            return -1;           // acabou a entrada (ex.: Ctrl+Z)
        }
        int n;
        if (!textoParaInteiro(entrada, n)) {
            cout << "Entrada invalida: \"" << entrada
                 << "\" nao e um numero inteiro. Tente de novo.\n";
        } else if (n < N_MINIMO || n > N_MAXIMO) {
            cout << "Valor nao aceito: N = " << n << ". O programa aceita N de "
                 << N_MINIMO << " a " << N_MAXIMO << ". Tente de novo.\n";
        } else {
            return n;
        }
    }
}

// Quantos movimentos aleatorios usar no embaralhamento de cada tamanho.
int movimentosDeEmbaralhamento(int n) {
    if (n == 2) {
        return 15;
    }
    if (n == 3) {
        return 40;
    }
    if (n == 4) {
        return 40;
    }
    return 200;                  // N = 5 e 6
}

int main() {
    srand(time(0));              // sorteio diferente a cada execucao

    int n = lerN();
    if (n == -1) {
        cout << "\nNenhum valor valido foi informado. Encerrando.\n";
        return 1;
    }

    Tabuleiro t;
    criarTabuleiro(t, n);
    embaralhar(t, movimentosDeEmbaralhamento(n));

    cout << "\nTabuleiro embaralhado (" << n << "x" << n << "):\n";
    imprimirTabuleiro(t);

    cout << "\nResolvendo...\n";
    Solucao s;
    criarSolucao(s);
    int estrategia = resolver(t, s);
    if (estrategia == ESTRATEGIA_NENHUMA) {
        cout << "Nao foi possivel encontrar a solucao.\n";
        liberarSolucao(s);
        destruirTabuleiro(t);
        return 1;
    }
    if (n == 4 && estrategia == ESTRATEGIA_CAMADAS) {
        cout << "(O A* passou do limite de estados neste embaralhamento;"
             << " foi usada a estrategia por camadas.)\n";
    }
    cout << "Solucao encontrada com " << s.quantidade << " movimentos, usando "
         << nomeEstrategia(estrategia) << ".\n";

    // Mostra cada passo: qual peca mexer e como fica o tabuleiro.
    for (int i = 0; i < s.quantidade; i++) {
        aplicarMovimento(t, s.movimentos[i]);
        cout << "\nPasso " << (i + 1) << " de " << s.quantidade
             << ": mover a peca " << s.movimentos[i] << "\n";
        imprimirTabuleiro(t);
    }

    if (estaResolvido(t)) {
        cout << "\nTabuleiro resolvido!\n";
    }

    liberarSolucao(s);
    destruirTabuleiro(t);
    return 0;
}
