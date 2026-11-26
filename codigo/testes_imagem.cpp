// Testes da Imagem: ler/salvar PPM, recortar (com e sem sobra) e montar.
// Cria arquivos temporarios na pasta onde roda e apaga no fim.
#include "Imagem.h"
#include "Tabuleiro.h"
#include <iostream>
#include <fstream>
#include <cstdio>
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

// Cada pixel recebe uma cor unica que depende da posicao: assim da para
// saber de onde veio qualquer pixel depois de recortar ou montar.
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

bool pixelPreto(const Imagem &img, int x, int y) {
    return pixelEm(img, x, y, 0) == 0 && pixelEm(img, x, y, 1) == 0 && pixelEm(img, x, y, 2) == 0;
}

void escreverArquivo(const char *caminho, const char *conteudo) {
    ofstream arq(caminho);
    arq << conteudo;
}

void testeCriar() {
    cout << "== Criar imagem ==\n";
    Imagem img;
    criarImagem(img, 4, 3, 255);
    verificar(img.largura == 4 && img.altura == 3, "dimensoes 4x3");
    bool tudoPreto = true;
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 4; x++) {
            if (!pixelPreto(img, x, y)) {
                tudoPreto = false;
            }
        }
    }
    verificar(tudoPreto, "imagem nova comeca preta");
    definirPixel(img, 3, 2, 10, 20, 30);
    verificar(pixelEm(img, 3, 2, 0) == 10 && pixelEm(img, 3, 2, 1) == 20
              && pixelEm(img, 3, 2, 2) == 30, "definir e ler um pixel");
    liberarImagem(img);
    verificar(img.pixels == NULL, "liberar zera o ponteiro");
}

void testeSalvarELer() {
    cout << "== Salvar e ler de volta ==\n";
    Imagem original;
    criarImagem(original, 7, 5, 255);
    preencherPadrao(original);
    verificar(salvarPPM("teste_tmp.ppm", original), "salvar deve funcionar");

    Imagem lida;
    verificar(lerPPM("teste_tmp.ppm", lida), "ler o arquivo salvo");
    verificar(lida.largura == 7 && lida.altura == 5 && lida.maxValor == 255,
              "cabecalho igual ao original");
    bool iguais = true;
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 7; x++) {
            if (!mesmoPixel(original, x, y, lida, x, y)) {
                iguais = false;
            }
        }
    }
    verificar(iguais, "todos os pixels iguais ao original");
    liberarImagem(lida);
    liberarImagem(original);
    remove("teste_tmp.ppm");
}

void testeComentarios() {
    cout << "== Cabecalho com comentarios ==\n";
    escreverArquivo("teste_tmp.ppm",
                    "P3\n# criado pelo GIMP\n2 1\n# outro comentario\n255\n"
                    "255 0 0   0 0 255\n");
    Imagem img;
    verificar(lerPPM("teste_tmp.ppm", img), "comentarios sao ignorados");
    verificar(img.largura == 2 && img.altura == 1, "dimensoes 2x1");
    verificar(pixelEm(img, 0, 0, 0) == 255 && pixelEm(img, 1, 0, 2) == 255,
              "pixels lidos certos (vermelho e azul)");
    liberarImagem(img);
    remove("teste_tmp.ppm");
}

void testeArquivosInvalidos() {
    cout << "== Arquivos invalidos ==\n";
    Imagem img;
    verificar(!lerPPM("nao_existe_123.ppm", img), "arquivo inexistente");
    verificar(img.pixels == NULL, "imagem fica vazia quando falha");

    escreverArquivo("teste_tmp.ppm", "P6\n2 1\n255\n0 0 0 0 0 0\n");
    verificar(!lerPPM("teste_tmp.ppm", img), "P6 (binario) nao e aceito");

    escreverArquivo("teste_tmp.ppm", "P3\n2 2\n255\n0 0 0 0 0 0\n");
    verificar(!lerPPM("teste_tmp.ppm", img), "arquivo cortado (faltam pixels)");
    verificar(img.pixels == NULL, "arquivo cortado nao deixa memoria alocada");

    escreverArquivo("teste_tmp.ppm", "P3\n1 1\n255\n0 300 0\n");
    verificar(!lerPPM("teste_tmp.ppm", img), "valor maior que o maxValor");

    escreverArquivo("teste_tmp.ppm", "P3\n0 5\n255\n");
    verificar(!lerPPM("teste_tmp.ppm", img), "largura zero");

    escreverArquivo("teste_tmp.ppm", "P3\n1 1\n255\n0 abc 0\n");
    verificar(!lerPPM("teste_tmp.ppm", img), "texto no lugar de numero");
    remove("teste_tmp.ppm");
}

void testeRecortarExato() {
    cout << "== Recortar sem sobra (6x6, N = 3) ==\n";
    Imagem img;
    criarImagem(img, 6, 6, 255);
    preencherPadrao(img);
    Imagem *pecas = recortarPecas(img, 3);
    verificar(pecas[0].largura == 2 && pecas[0].altura == 2, "cada peca 2x2");

    bool certo = true;
    for (int i = 0; i < 9; i++) {
        int x0 = (i % 3) * 2;
        int y0 = (i / 3) * 2;
        for (int y = 0; y < 2; y++) {
            for (int x = 0; x < 2; x++) {
                if (!mesmoPixel(pecas[i], x, y, img, x0 + x, y0 + y)) {
                    certo = false;
                }
            }
        }
    }
    verificar(certo, "cada peca e o pedaco certo da imagem");
    liberarPecas(pecas, 3);
    liberarImagem(img);
}

void testeRecortarComSobra() {
    cout << "== Recortar com sobra (7x5, N = 2) ==\n";
    Imagem img;
    criarImagem(img, 7, 5, 255);
    preencherPadrao(img);
    Imagem *pecas = recortarPecas(img, 2);
    verificar(pecas[0].largura == 3 && pecas[0].altura == 2, "cada peca 3x2 (7/2 = 3, 5/2 = 2)");
    // Peca 3 = linha 1, coluna 1: comeca no pixel (3, 2) da original.
    verificar(mesmoPixel(pecas[3], 0, 0, img, 3, 2), "peca 3 comeca em (3, 2)");
    verificar(mesmoPixel(pecas[3], 2, 1, img, 5, 3), "peca 3 termina em (5, 3)");
    liberarPecas(pecas, 2);

    // 100x100 com N = 3: pecas de 33x33, sobra de 1 pixel descartada.
    Imagem grande;
    criarImagem(grande, 100, 100, 255);
    preencherPadrao(grande);
    Imagem *p3 = recortarPecas(grande, 3);
    verificar(p3[8].largura == 33 && p3[8].altura == 33, "100x100 com N = 3: pecas 33x33");
    verificar(mesmoPixel(p3[8], 32, 32, grande, 98, 98), "ultima peca termina em (98, 98)");
    liberarPecas(p3, 3);
    liberarImagem(grande);
    liberarImagem(img);
}

void testeMontarResolvido() {
    cout << "== Montar tabuleiro resolvido ==\n";
    Imagem img;
    criarImagem(img, 7, 7, 255);              // 7 com N = 3: sobra 1
    preencherPadrao(img);
    Imagem *pecas = recortarPecas(img, 3);
    Tabuleiro t;
    criarTabuleiro(t, 3);
    Imagem saida;
    montarImagem(pecas, t, saida);

    verificar(saida.largura == 6 && saida.altura == 6, "saida 6x6 (sem a sobra)");
    bool certo = true;
    for (int y = 0; y < 6; y++) {
        for (int x = 0; x < 6; x++) {
            bool noVazio = (x >= 4 && y >= 4);
            if (noVazio && !pixelPreto(saida, x, y)) {
                certo = false;
            }
            if (!noVazio && !mesmoPixel(saida, x, y, img, x, y)) {
                certo = false;
            }
        }
    }
    verificar(certo, "igual a original, com o canto inferior direito preto");
    liberarImagem(saida);
    destruirTabuleiro(t);
    liberarPecas(pecas, 3);
    liberarImagem(img);
}

void testeMontarEmbaralhado() {
    cout << "== Montar tabuleiro embaralhado ==\n";
    Imagem img;
    criarImagem(img, 4, 4, 255);
    preencherPadrao(img);
    Imagem *pecas = recortarPecas(img, 2);    // pecas de 2x2
    int estado[4] = {3, 1, 0, 2};
    Tabuleiro t;
    criarTabuleiroComEstado(t, 2, estado);
    Imagem saida;
    montarImagem(pecas, t, saida);

    verificar(mesmoPixel(saida, 0, 0, pecas[2], 0, 0), "casa 0 mostra a peca 3");
    verificar(mesmoPixel(saida, 3, 1, pecas[0], 1, 1), "casa 1 mostra a peca 1");
    verificar(pixelPreto(saida, 0, 2) && pixelPreto(saida, 1, 3), "casa 2 (vazio) fica preta");
    verificar(mesmoPixel(saida, 2, 2, pecas[1], 0, 0), "casa 3 mostra a peca 2");
    liberarImagem(saida);
    destruirTabuleiro(t);
    liberarPecas(pecas, 2);
    liberarImagem(img);
}

int main() {
    cout << "Executando testes da Imagem\n\n";
    testeCriar();
    testeSalvarELer();
    testeComentarios();
    testeArquivosInvalidos();
    testeRecortarExato();
    testeRecortarComSobra();
    testeMontarResolvido();
    testeMontarEmbaralhado();

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
