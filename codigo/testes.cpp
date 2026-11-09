// testes.cpp
// Testes da Etapa 1: estado do tabuleiro e validade dos movimentos no centro,
// nas bordas e nos cantos. Nao usa biblioteca de testes: conta as verificacoes
// e as falhas e devolve codigo de saida diferente de zero se algo falhar
// (assim o 'ctest' do CMake tambem percebe o erro).
#include "Tabuleiro.h"
#include <iostream>
using namespace std;

// Contadores globais do resultado dos testes.
int totalVerificacoes = 0;
int totalFalhas = 0;

// Registra uma verificacao: se a condicao for falsa, conta como falha e avisa.
void verificar(bool condicao, const char *descricao) {
    totalVerificacoes++;
    if (!condicao) {
        totalFalhas++;
        cout << "  [FALHOU] " << descricao << "\n";
    }
}

// Diz se o valor x aparece no vetor v de tamanho qtd.
bool contem(int v[], int qtd, int x) {
    for (int i = 0; i < qtd; i++) {
        if (v[i] == x) {
            return true;
        }
    }
    return false;
}

// Diz se dois tabuleiros tem exatamente o mesmo estado.
bool estadosIguais(const Tabuleiro &a, const Tabuleiro &b) {
    if (a.n != b.n) {
        return false;
    }
    for (int i = 0; i < a.n * a.n; i++) {
        if (a.pecas[i] != b.pecas[i]) {
            return false;
        }
    }
    return true;
}

void testeEstadoResolvido() {
    cout << "== Estado resolvido ==\n";

    Tabuleiro t;
    criarTabuleiro(t, 3);                 // 1 2 3 / 4 5 6 / 7 8 _
    verificar(t.n == 3, "n deve ser 3");
    verificar(estaResolvido(t), "tabuleiro recem-criado deve estar resolvido");
    verificar(pecaEm(t, 0, 0) == 1, "canto superior esquerdo deve ser 1");
    verificar(pecaEm(t, 2, 1) == 8, "posicao (2,1) deve ser 8");
    verificar(pecaEm(t, 2, 2) == VAZIO, "canto inferior direito deve ser o vazio");
    verificar(t.vazio == 8, "indice do vazio deve ser 8");
    destruirTabuleiro(t);

    int embaralhado[9] = {1, 2, 3, 4, 5, 6, 7, 0, 8};
    Tabuleiro e;
    criarTabuleiroComEstado(e, 3, embaralhado);
    verificar(!estaResolvido(e), "estado embaralhado nao esta resolvido");
    destruirTabuleiro(e);

    Tabuleiro t2;
    criarTabuleiro(t2, 2);
    verificar(estaResolvido(t2), "tabuleiro 2x2 recem-criado resolvido");
    destruirTabuleiro(t2);

    Tabuleiro t4;
    criarTabuleiro(t4, 4);
    verificar(estaResolvido(t4), "tabuleiro 4x4 recem-criado resolvido");
    destruirTabuleiro(t4);
}

void testeMovimentoCentro() {
    cout << "== Movimentos no centro ==\n";
    int estado[9] = {1, 2, 3, 4, 0, 5, 6, 7, 8};   // vazio no centro (indice 4)
    Tabuleiro t;
    criarTabuleiroComEstado(t, 3, estado);
    verificar(t.vazio == 4, "vazio deve estar no centro");

    int mov[4];
    int qtd = movimentosValidos(t, mov);
    verificar(qtd == 4, "no centro deve haver 4 movimentos");
    verificar(contem(mov, qtd, 2), "peca de cima (2) deve poder mover");
    verificar(contem(mov, qtd, 7), "peca de baixo (7) deve poder mover");
    verificar(contem(mov, qtd, 4), "peca da esquerda (4) deve poder mover");
    verificar(contem(mov, qtd, 5), "peca da direita (5) deve poder mover");
    destruirTabuleiro(t);
}

void testeMovimentoBorda() {
    cout << "== Movimentos na borda ==\n";
    int estado[9] = {1, 0, 2, 3, 4, 5, 6, 7, 8};   // vazio na borda de cima (indice 1)
    Tabuleiro t;
    criarTabuleiroComEstado(t, 3, estado);
    verificar(t.vazio == 1, "vazio deve estar na borda superior");

    int mov[4];
    int qtd = movimentosValidos(t, mov);
    verificar(qtd == 3, "na borda deve haver 3 movimentos");
    verificar(contem(mov, qtd, 4), "peca de baixo (4) deve poder mover");
    verificar(contem(mov, qtd, 1), "peca da esquerda (1) deve poder mover");
    verificar(contem(mov, qtd, 2), "peca da direita (2) deve poder mover");
    destruirTabuleiro(t);
}

void testeMovimentoCanto() {
    cout << "== Movimentos no canto ==\n";
    int estado[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};   // vazio no canto sup. esq. (indice 0)
    Tabuleiro t;
    criarTabuleiroComEstado(t, 3, estado);
    verificar(t.vazio == 0, "vazio deve estar no canto");

    int mov[4];
    int qtd = movimentosValidos(t, mov);
    verificar(qtd == 2, "no canto deve haver 2 movimentos");
    verificar(contem(mov, qtd, 3), "peca de baixo (3) deve poder mover");
    verificar(contem(mov, qtd, 1), "peca da direita (1) deve poder mover");
    destruirTabuleiro(t);

    // O canto inferior direito do estado resolvido tambem tem 2 movimentos.
    Tabuleiro r;
    criarTabuleiro(r, 3);
    int mov2[4];
    int qtd2 = movimentosValidos(r, mov2);
    verificar(qtd2 == 2, "canto inferior direito tem 2 movimentos");
    verificar(contem(mov2, qtd2, 6), "peca de cima (6) deve poder mover");
    verificar(contem(mov2, qtd2, 8), "peca da esquerda (8) deve poder mover");
    destruirTabuleiro(r);
}

void testeAplicarMovimento() {
    cout << "== Aplicacao de movimentos ==\n";
    int estado[9] = {1, 2, 3, 4, 0, 5, 6, 7, 8};   // vazio no centro
    Tabuleiro t;
    criarTabuleiroComEstado(t, 3, estado);

    verificar(aplicarMovimento(t, 7), "mover a peca 7 (vizinha) deve funcionar");
    verificar(t.vazio == 7, "o vazio deve ter descido para o indice 7");
    verificar(pecaEm(t, 1, 1) == 7, "a peca 7 deve estar no centro agora");
    verificar(pecaEm(t, 2, 1) == VAZIO, "o lugar antigo da 7 deve ficar vazio");

    // Guarda o estado atual para conferir que um movimento invalido nao muda nada.
    Tabuleiro copia;
    criarTabuleiroComEstado(copia, 3, t.pecas);
    verificar(!aplicarMovimento(t, 2), "peca 2 nao e vizinha do vazio: deve falhar");
    verificar(estadosIguais(t, copia), "o estado nao pode mudar apos movimento invalido");
    verificar(!aplicarMovimento(t, 99), "peca inexistente (99) deve falhar");
    verificar(!aplicarMovimento(t, VAZIO), "nao se move o vazio: deve falhar");
    verificar(estadosIguais(t, copia), "o estado continua intacto");
    destruirTabuleiro(copia);
    destruirTabuleiro(t);

    // Um movimento seguido do seu inverso volta ao estado original.
    Tabuleiro u;
    criarTabuleiro(u, 3);
    Tabuleiro original;
    criarTabuleiroComEstado(original, 3, u.pecas);
    verificar(aplicarMovimento(u, 8), "mover a peca 8");
    verificar(!estaResolvido(u), "apos um movimento nao esta mais resolvido");
    verificar(aplicarMovimento(u, 8), "mover a peca 8 de volta");
    verificar(estadosIguais(u, original), "deve ter voltado ao estado original");
    verificar(estaResolvido(u), "e esta resolvido de novo");
    destruirTabuleiro(original);
    destruirTabuleiro(u);
}

int main() {
    cout << "Executando testes da Etapa 1 (Tabuleiro e movimentos)\n\n";

    testeEstadoResolvido();
    testeMovimentoCentro();
    testeMovimentoBorda();
    testeMovimentoCanto();
    testeAplicarMovimento();

    cout << "\n------------------------------------------\n";
    cout << "Verificacoes: " << totalVerificacoes
         << "  |  Falhas: " << totalFalhas << "\n";
    if (totalFalhas == 0) {
        cout << "RESULTADO: TODOS OS TESTES PASSARAM\n";
        return 0;
    }
    cout << "RESULTADO: HOUVE FALHAS\n";
    return 1;
}
