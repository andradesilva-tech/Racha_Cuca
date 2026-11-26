// Le N e a imagem, embaralha, mostra a solucao passo a passo e salva as
// imagens dos estados inicial e final.
#include "Tabuleiro.h"
#include "Solucionador.h"
#include "Imagem.h"
#include "Entrada.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

const char *ARQUIVO_INICIAL = "estado_inicial.ppm";
const char *ARQUIVO_FINAL = "estado_final.ppm";

// Pergunta N ate receber um valor valido. Devolve -1 se a entrada acabar.
int lerN() {
    string entrada;
    while (true) {
        cout << "Digite o tamanho N do tabuleiro (de " << N_MINIMO
             << " a " << N_MAXIMO << "): ";
        if (!(cin >> entrada)) {
            return -1;
        }
        int n;
        if (!textoParaInteiro(entrada.c_str(), n)) {
            cout << "Entrada invalida: \"" << entrada
                 << "\" nao e um numero inteiro. Tente de novo.\n";
        } else if (!nAceito(n)) {
            cout << "Valor nao aceito: N = " << n << ". O programa aceita N de "
                 << N_MINIMO << " a " << N_MAXIMO << ". Tente de novo.\n";
        } else {
            return n;
        }
    }
}

// Pergunta o caminho da imagem ate conseguir ler uma que caiba N x N pecas.
// Devolve false se a entrada acabar.
bool lerImagem(int n, Imagem &img) {
    string caminho;
    while (true) {
        cout << "Digite o caminho da imagem PPM (formato P3): ";
        if (!getline(cin, caminho)) {
            return false;
        }
        // O "Copiar como caminho" do Windows coloca aspas em volta.
        if (caminho.size() >= 2 && caminho[0] == '"' && caminho[caminho.size() - 1] == '"') {
            caminho = caminho.substr(1, caminho.size() - 2);
        }
        if (caminho.size() == 0) {
            continue;
        }
        if (!lerPPM(caminho.c_str(), img)) {
            cout << "Nao foi possivel ler \"" << caminho << "\". Confira se o arquivo"
                 << " existe e esta no formato PPM texto (P3). Tente de novo.\n";
        } else if (img.largura < n || img.altura < n) {
            cout << "A imagem tem " << img.largura << "x" << img.altura
                 << " pixels: pequena demais para " << n << "x" << n
                 << " pecas. Tente outra.\n";
            liberarImagem(img);
        } else {
            int sobraLargura = img.largura % n;
            int sobraAltura = img.altura % n;
            cout << "Imagem de " << img.largura << "x" << img.altura << " pixels; cada peca tera "
                 << img.largura / n << "x" << img.altura / n << ".\n";
            if (sobraLargura > 0 || sobraAltura > 0) {
                cout << "Aviso: as dimensoes nao sao multiplas de " << n << ". Foram descartados "
                     << sobraLargura << " pixel(s) a direita e " << sobraAltura
                     << " embaixo.\n";
            }
            return true;
        }
    }
}

// Monta a imagem do tabuleiro e grava no arquivo, avisando o resultado.
void salvarEstado(const Imagem pecas[], const Tabuleiro &t, const char *arquivo, const char *nome) {
    Imagem img;
    montarImagem(pecas, t, img);
    if (salvarPPM(arquivo, img)) {
        cout << "Imagem do estado " << nome << " salva em " << arquivo << "\n";
    } else {
        cout << "Nao foi possivel salvar " << arquivo << "\n";
    }
    liberarImagem(img);
}

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
    return 200;
}

int main() {
    srand(time(0));

    int n = lerN();
    if (n == -1) {
        cout << "\nNenhum valor valido foi informado. Encerrando.\n";
        return 1;
    }
    cin.ignore(10000, '\n');     // descarta o resto da linha do N antes do getline

    Imagem original;
    if (!lerImagem(n, original)) {
        cout << "\nNenhuma imagem valida foi informada. Encerrando.\n";
        return 1;
    }
    Imagem *pecas = recortarPecas(original, n);
    liberarImagem(original);

    Tabuleiro t;
    criarTabuleiro(t, n);
    embaralhar(t, movimentosDeEmbaralhamento(n));

    cout << "\nTabuleiro embaralhado (" << n << "x" << n << "):\n";
    imprimirTabuleiro(t);
    salvarEstado(pecas, t, ARQUIVO_INICIAL, "inicial");

    cout << "\nResolvendo...\n";
    Solucao s;
    criarSolucao(s);
    int estrategia = resolver(t, s);
    if (estrategia == ESTRATEGIA_NENHUMA) {
        cout << "Nao foi possivel encontrar a solucao.\n";
        liberarSolucao(s);
        destruirTabuleiro(t);
        liberarPecas(pecas, n);
        return 1;
    }
    if (n == 4 && estrategia == ESTRATEGIA_CAMADAS) {
        cout << "(O A* passou do limite de estados neste embaralhamento;"
             << " foi usada a estrategia por camadas.)\n";
    }
    cout << "Solucao encontrada com " << s.quantidade << " movimentos, usando "
         << nomeEstrategia(estrategia) << ".\n";

    for (int i = 0; i < s.quantidade; i++) {
        aplicarMovimento(t, s.movimentos[i]);
        cout << "\nPasso " << (i + 1) << " de " << s.quantidade
             << ": mover a peca " << s.movimentos[i] << "\n";
        imprimirTabuleiro(t);
    }

    if (estaResolvido(t)) {
        cout << "\nTabuleiro resolvido!\n";
    }
    salvarEstado(pecas, t, ARQUIVO_FINAL, "final");

    liberarSolucao(s);
    destruirTabuleiro(t);
    liberarPecas(pecas, n);
    return 0;
}
