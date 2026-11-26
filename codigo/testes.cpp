// Testes do Tabuleiro: movimentos no centro, bordas e cantos, e embaralhar.
// Sai com codigo 1 se algo falhar, para o ctest perceber.
#include "Tabuleiro.h"
#include <iostream>
#include <cstdlib>
using namespace std;

int totalVerificacoes = 0;
int totalFalhas = 0;

void verificar(bool condicao, const char *descricao) {
    totalVerificacoes++;
    if (!condicao) {
        totalFalhas++;
        cout << "  [FALHOU] " << descricao << "\n";
    }
}

bool contem(int v[], int qtd, int x) {
    for (int i = 0; i < qtd; i++) {
        if (v[i] == x) {
            return true;
        }
    }
    return false;
}

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

    // Movimento invalido nao pode mudar nada.
    Tabuleiro copia;
    criarTabuleiroComEstado(copia, 3, t.pecas);
    verificar(!aplicarMovimento(t, 2), "peca 2 nao e vizinha do vazio: deve falhar");
    verificar(estadosIguais(t, copia), "o estado nao pode mudar apos movimento invalido");
    verificar(!aplicarMovimento(t, 99), "peca inexistente (99) deve falhar");
    verificar(!aplicarMovimento(t, VAZIO), "nao se move o vazio: deve falhar");
    verificar(estadosIguais(t, copia), "o estado continua intacto");
    destruirTabuleiro(copia);
    destruirTabuleiro(t);

    // Mover a mesma peca duas vezes volta ao estado original.
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

// Cada valor de 0 a n*n-1 aparece uma unica vez e 'vazio' aponta para o 0.
bool configuracaoValida(const Tabuleiro &t) {
    int total = t.n * t.n;
    int *contagem = new int[total];
    for (int v = 0; v < total; v++) {
        contagem[v] = 0;
    }
    bool ok = true;
    for (int i = 0; i < total; i++) {
        int v = t.pecas[i];
        if (v < 0 || v >= total) {
            ok = false;
        } else {
            contagem[v]++;
        }
    }
    for (int v = 0; v < total; v++) {
        if (contagem[v] != 1) {
            ok = false;
        }
    }
    if (t.pecas[t.vazio] != VAZIO) {
        ok = false;
    }
    delete[] contagem;
    return ok;
}

void testeEmbaralhar() {
    cout << "== Embaralhar ==\n";
    for (int n = 2; n <= 6; n++) {
        Tabuleiro t;
        criarTabuleiro(t, n);
        int feitos = embaralhar(t, 50);
        verificar(feitos >= 50, "deve fazer pelo menos a quantidade pedida");
        verificar(!estaResolvido(t), "depois de embaralhar nao pode estar resolvido");
        verificar(configuracaoValida(t), "cada peca uma vez e um unico vazio");
        destruirTabuleiro(t);
    }

    // Mesmo pedindo 0 movimentos, um tabuleiro resolvido sai embaralhado.
    Tabuleiro r;
    criarTabuleiro(r, 3);
    int feitos = embaralhar(r, 0);
    verificar(feitos > 0, "pedindo 0 em tabuleiro resolvido, ainda mexe");
    verificar(!estaResolvido(r), "e termina fora do estado resolvido");
    destruirTabuleiro(r);

    // No 2x2 sobra so 1 opcao por vez: confere que nunca trava.
    for (int vez = 0; vez < 20; vez++) {
        Tabuleiro p;
        criarTabuleiro(p, 2);
        embaralhar(p, 12 + vez);
        verificar(!estaResolvido(p), "2x2 embaralhado nao fica resolvido");
        verificar(configuracaoValida(p), "2x2 continua com pecas validas");
        destruirTabuleiro(p);
    }
}

int main() {
    cout << "Executando testes do Tabuleiro (movimentos e embaralhar)\n\n";
    srand(12345);   // semente fixa: resultado igual em toda execucao

    testeEstadoResolvido();
    testeMovimentoCentro();
    testeMovimentoBorda();
    testeMovimentoCanto();
    testeAplicarMovimento();
    testeEmbaralhar();

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
