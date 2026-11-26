// Teste do fluxo completo, como no main: recorta a imagem, embaralha, confere a
// imagem inicial, resolve, aplica os passos e confere a imagem reconstruida.
// A imagem tem 125x97 pixels, que nao e multiplo de nenhum N de 2 a 6.
#include "Imagem.h"
#include "Tabuleiro.h"
#include "Solucionador.h"
#include <iostream>
#include <cstdlib>
using namespace std;

const int SEMENTES_POR_N = 5;

int totalVerificacoes = 0;
int totalFalhas = 0;

void verificar(bool condicao, const char *descricao) {
    totalVerificacoes++;
    if (!condicao) {
        totalFalhas++;
        cout << "  [FALHOU] " << descricao << "\n";
    }
}

// Cor unica por pixel (x, y, x+y): cada pedaco da imagem e diferente dos outros.
void preencherPadrao(Imagem &img) {
    for (int y = 0; y < img.altura; y++) {
        for (int x = 0; x < img.largura; x++) {
            definirPixel(img, x, y, x, y, (x + y) % 256);
        }
    }
}

bool mesmoPixel(const Imagem &a, int xa, int ya, const Imagem &b, int xb, int yb) {
    for (int c = 0; c < 3; c++) {
        if (pixelEm(a, xa, ya, c) != pixelEm(b, xb, yb, c)) {
            return false;
        }
    }
    return true;
}

// true se o bloco da casa (x0, y0) na imagem montada e igual a 'peca'.
bool blocoIgual(const Imagem &montada, int x0, int y0, const Imagem &peca) {
    for (int y = 0; y < peca.altura; y++) {
        for (int x = 0; x < peca.largura; x++) {
            if (!mesmoPixel(montada, x0 + x, y0 + y, peca, x, y)) {
                return false;
            }
        }
    }
    return true;
}

bool blocoPreto(const Imagem &montada, int x0, int y0, int largura, int altura) {
    for (int y = 0; y < altura; y++) {
        for (int x = 0; x < largura; x++) {
            for (int c = 0; c < 3; c++) {
                if (pixelEm(montada, x0 + x, y0 + y, c) != 0) {
                    return false;
                }
            }
        }
    }
    return true;
}

// Olha so para a imagem e descobre qual peca esta na casa.
// Devolve o indice da peca, -1 se o bloco for preto ou -2 se nao for nenhuma.
int pecaNaCasa(const Imagem &montada, const Imagem pecas[], int n, int casa) {
    int lp = pecas[0].largura;
    int ap = pecas[0].altura;
    int x0 = (casa % n) * lp;
    int y0 = (casa / n) * ap;
    if (blocoPreto(montada, x0, y0, lp, ap)) {
        return -1;
    }
    for (int i = 0; i < n * n; i++) {
        if (blocoIgual(montada, x0, y0, pecas[i])) {
            return i;
        }
    }
    return -2;
}

int movimentosDeEmbaralhamento(int n) {
    if (n == 2) {
        return 15;
    }
    if (n <= 4) {
        return 40;
    }
    return 200;
}

// Confere a imagem inicial: toda peca (menos a retirada) aparece uma unica
// vez, na casa que o tabuleiro diz, e ha um unico bloco preto.
void conferirImagemInicial(const Imagem &inicial, const Imagem pecas[], const Tabuleiro &t) {
    int n = t.n;
    int *usos = new int[n * n];
    for (int i = 0; i < n * n; i++) {
        usos[i] = 0;
    }
    int pretos = 0;
    bool reconhecidas = true;
    bool batemComTabuleiro = true;
    for (int casa = 0; casa < n * n; casa++) {
        int k = pecaNaCasa(inicial, pecas, n, casa);
        if (k == -1) {
            pretos++;
        } else if (k == -2) {
            reconhecidas = false;
        } else {
            usos[k]++;
        }
        if (k != t.pecas[casa] - 1) {      // vazio: valor 0, entao espera -1
            batemComTabuleiro = false;
        }
    }
    bool cadaUmaVez = true;
    for (int i = 0; i < n * n - 1; i++) {
        if (usos[i] != 1) {
            cadaUmaVez = false;
        }
    }
    verificar(reconhecidas, "todo bloco da imagem inicial e uma peca ou o vazio");
    verificar(cadaUmaVez, "cada peca (menos a retirada) aparece exatamente uma vez");
    verificar(usos[n * n - 1] == 0, "a peca retirada nao aparece");
    verificar(pretos == 1, "ha um unico espaco vazio (bloco preto)");
    verificar(batemComTabuleiro, "a imagem mostra as pecas nas casas do tabuleiro");
    delete[] usos;
}

// Confere a imagem final: igual a original (sem a sobra), com o canto preto.
void conferirImagemFinal(const Imagem &final, const Imagem &original, int n) {
    int lp = original.largura / n;
    int ap = original.altura / n;
    verificar(final.largura == lp * n && final.altura == ap * n,
              "imagem final tem o tamanho sem a sobra");
    bool igual = true;
    for (int y = 0; y < final.altura; y++) {
        for (int x = 0; x < final.largura; x++) {
            bool noCanto = (x >= lp * (n - 1) && y >= ap * (n - 1));
            if (noCanto) {
                for (int c = 0; c < 3; c++) {
                    if (pixelEm(final, x, y, c) != 0) {
                        igual = false;
                    }
                }
            } else if (!mesmoPixel(final, x, y, original, x, y)) {
                igual = false;
            }
        }
    }
    verificar(igual, "imagem reconstruida igual a original, com o canto preto");
}

void testarN(const Imagem &original, int n) {
    cout << "== N = " << n << " (" << SEMENTES_POR_N << " embaralhamentos) ==\n";
    Imagem *pecas = recortarPecas(original, n);

    for (int semente = 1; semente <= SEMENTES_POR_N; semente++) {
        srand(semente);
        Tabuleiro t;
        criarTabuleiro(t, n);
        embaralhar(t, movimentosDeEmbaralhamento(n));
        verificar(!estaResolvido(t), "tabuleiro embaralhado nao comeca resolvido");

        Imagem inicial;
        montarImagem(pecas, t, inicial);
        conferirImagemInicial(inicial, pecas, t);
        liberarImagem(inicial);

        Solucao s;
        criarSolucao(s);
        int estrategia = resolver(t, s);
        verificar(estrategia != ESTRATEGIA_NENHUMA, "encontrou a solucao");

        // Aplica os passos exatamente como o main exibe.
        bool todosValidos = true;
        for (int i = 0; i < s.quantidade; i++) {
            if (!aplicarMovimento(t, s.movimentos[i])) {
                todosValidos = false;
            }
        }
        verificar(todosValidos, "todos os movimentos exibidos sao validos");
        verificar(estaResolvido(t), "o tabuleiro termina resolvido");

        Imagem final;
        montarImagem(pecas, t, final);
        conferirImagemFinal(final, original, n);
        liberarImagem(final);

        liberarSolucao(s);
        destruirTabuleiro(t);
    }
    liberarPecas(pecas, n);
}

int main() {
    cout << "Executando teste do fluxo completo (imagem 125x97)\n\n";
    Imagem original;
    criarImagem(original, 125, 97, 255);
    preencherPadrao(original);

    for (int n = 2; n <= 6; n++) {
        testarN(original, n);
    }
    liberarImagem(original);

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
