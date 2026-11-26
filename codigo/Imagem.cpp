#include "Imagem.h"
#include <fstream>
#include <cstdlib>
using namespace std;

// Limita o tamanho para nao estourar a memoria (4000 x 4000 ja usa ~190 MB).
const int DIMENSAO_MAXIMA = 4000;
const int MAX_VALOR_PPM = 65535;

void criarImagem(Imagem &img, int largura, int altura, int maxValor) {
    img.largura = largura;
    img.altura = altura;
    img.maxValor = maxValor;
    int total = largura * altura * 3;
    img.pixels = new int[total];
    for (int i = 0; i < total; i++) {
        img.pixels[i] = 0;
    }
}

void liberarImagem(Imagem &img) {
    delete[] img.pixels;
    img.pixels = NULL;
    img.largura = 0;
    img.altura = 0;
}

int pixelEm(const Imagem &img, int x, int y, int canal) {
    return img.pixels[(y * img.largura + x) * 3 + canal];
}

void definirPixel(Imagem &img, int x, int y, int r, int g, int b) {
    int i = (y * img.largura + x) * 3;
    img.pixels[i] = r;
    img.pixels[i + 1] = g;
    img.pixels[i + 2] = b;
}

// Le o proximo numero, pulando espacos e comentarios (do # ate o fim da linha).
// Editores como o GIMP colocam um comentario logo depois do "P3".
static bool lerNumero(ifstream &arq, int &valor) {
    arq >> ws;
    while (arq.peek() == '#') {
        arq.ignore(100000, '\n');
        arq >> ws;
    }
    arq >> valor;
    return !arq.fail();
}

bool lerPPM(const char *caminho, Imagem &img) {
    img.largura = 0;
    img.altura = 0;
    img.maxValor = 0;
    img.pixels = NULL;

    ifstream arq(caminho);
    if (!arq.is_open()) {
        return false;
    }

    // Cabecalho: P3, largura, altura e maior valor de cor.
    char p, tres;
    arq >> p >> tres;
    if (arq.fail() || p != 'P' || tres != '3') {
        return false;
    }
    int largura, altura, maxValor;
    if (!lerNumero(arq, largura) || !lerNumero(arq, altura) || !lerNumero(arq, maxValor)) {
        return false;
    }
    if (largura < 1 || altura < 1 || largura > DIMENSAO_MAXIMA || altura > DIMENSAO_MAXIMA) {
        return false;
    }
    if (maxValor < 1 || maxValor > MAX_VALOR_PPM) {
        return false;
    }

    criarImagem(img, largura, altura, maxValor);
    int total = largura * altura * 3;
    for (int i = 0; i < total; i++) {
        int valor;
        if (!lerNumero(arq, valor) || valor < 0 || valor > maxValor) {
            liberarImagem(img);    // arquivo cortado ou valor invalido
            return false;
        }
        img.pixels[i] = valor;
    }
    return true;
}

bool salvarPPM(const char *caminho, const Imagem &img) {
    ofstream arq(caminho);
    if (!arq.is_open()) {
        return false;
    }
    arq << "P3\n" << img.largura << " " << img.altura << "\n" << img.maxValor << "\n";
    for (int i = 0; i < img.largura * img.altura; i++) {
        arq << img.pixels[i * 3] << " " << img.pixels[i * 3 + 1] << " "
            << img.pixels[i * 3 + 2] << "\n";     // um pixel por linha
    }
    return !arq.fail();
}

// Copia um retangulo de 'origem' (a partir de xo, yo) para 'destino' (a partir de xd, yd).
static void copiarRetangulo(const Imagem &origem, int xo, int yo,
                            Imagem &destino, int xd, int yd, int largura, int altura) {
    for (int y = 0; y < altura; y++) {
        for (int x = 0; x < largura; x++) {
            definirPixel(destino, xd + x, yd + y,
                         pixelEm(origem, xo + x, yo + y, 0),
                         pixelEm(origem, xo + x, yo + y, 1),
                         pixelEm(origem, xo + x, yo + y, 2));
        }
    }
}

Imagem *recortarPecas(const Imagem &img, int n) {
    // Divisao inteira: a sobra fica de fora.
    int larguraPeca = img.largura / n;
    int alturaPeca = img.altura / n;

    Imagem *pecas = new Imagem[n * n];
    for (int linha = 0; linha < n; linha++) {
        for (int coluna = 0; coluna < n; coluna++) {
            Imagem &peca = pecas[linha * n + coluna];
            criarImagem(peca, larguraPeca, alturaPeca, img.maxValor);
            copiarRetangulo(img, coluna * larguraPeca, linha * alturaPeca,
                            peca, 0, 0, larguraPeca, alturaPeca);
        }
    }
    return pecas;
}

void liberarPecas(Imagem *pecas, int n) {
    for (int i = 0; i < n * n; i++) {
        liberarImagem(pecas[i]);
    }
    delete[] pecas;
}

void montarImagem(const Imagem pecas[], const Tabuleiro &t, Imagem &saida) {
    int n = t.n;
    int larguraPeca = pecas[0].largura;
    int alturaPeca = pecas[0].altura;
    criarImagem(saida, larguraPeca * n, alturaPeca * n, pecas[0].maxValor);   // comeca preta

    for (int casa = 0; casa < n * n; casa++) {
        int valor = t.pecas[casa];
        if (valor != VAZIO) {
            int xd = (casa % n) * larguraPeca;
            int yd = (casa / n) * alturaPeca;
            copiarRetangulo(pecas[valor - 1], 0, 0, saida, xd, yd, larguraPeca, alturaPeca);
        }
    }
}
