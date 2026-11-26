// Imagem PPM texto (P3): ler, salvar, recortar em N x N pecas e montar a
// imagem de um tabuleiro a partir das pecas.
#ifndef IMAGEM_H
#define IMAGEM_H

#include "Tabuleiro.h"

struct Imagem {
    int largura;
    int altura;
    int maxValor;   // maior valor de cor do arquivo (quase sempre 255)
    int *pixels;    // 3 valores (R, G, B) por pixel, linha por linha
};

// Aloca a imagem toda preta. Chame liberarImagem ao terminar.
void criarImagem(Imagem &img, int largura, int altura, int maxValor);
void liberarImagem(Imagem &img);

// canal: 0 = vermelho, 1 = verde, 2 = azul.
int pixelEm(const Imagem &img, int x, int y, int canal);
void definirPixel(Imagem &img, int x, int y, int r, int g, int b);

// Devolve false se o arquivo nao abrir ou nao for um P3 valido;
// nesse caso 'img' fica vazia (pixels = NULL) e nao precisa ser liberada.
bool lerPPM(const char *caminho, Imagem &img);
bool salvarPPM(const char *caminho, const Imagem &img);

// Divide em n*n pecas de (largura/n) x (altura/n). Se nao dividir certo, a
// sobra da direita e de baixo e descartada. Exige largura >= n e altura >= n.
// A peca i e a da linha i/n e coluna i%n; a ultima e a que sera removida.
// Devolve um vetor alocado: chame liberarPecas ao terminar.
Imagem *recortarPecas(const Imagem &img, int n);
void liberarPecas(Imagem *pecas, int n);

// Cria em 'saida' a imagem do tabuleiro: cada casa recebe a peca do seu
// valor (pecas[valor-1]) e o vazio fica preto. Chame liberarImagem depois.
void montarImagem(const Imagem pecas[], const Tabuleiro &t, Imagem &saida);

#endif
