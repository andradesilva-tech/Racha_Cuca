// testes_fila.cpp
// Testa a Fila SOZINHA (sem tabuleiro e sem solucionador).
#include "Fila.h"
#include <iostream>
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

void testeFilaNova() {
    cout << "== Fila recem-criada ==\n";
    Fila f;
    criarFila(f);
    verificar(filaVazia(f), "fila nova deve estar vazia");
    verificar(f.tamanho == 0, "fila nova deve ter tamanho 0");
    int v = 777;
    verificar(!desenfileirar(f, v), "desenfileirar de fila vazia deve falhar");
    verificar(v == 777, "valor nao muda quando desenfileirar falha");
    liberarFila(f);
}

void testeOrdemFIFO() {
    cout << "== Ordem FIFO (primeiro a entrar, primeiro a sair) ==\n";
    Fila f;
    criarFila(f);
    enfileirar(f, 1);
    enfileirar(f, 2);
    enfileirar(f, 3);
    verificar(!filaVazia(f), "fila com elementos nao esta vazia");
    verificar(f.tamanho == 3, "tamanho deve ser 3");

    int v;
    verificar(desenfileirar(f, v) && v == 1, "primeiro a sair deve ser o 1");
    verificar(desenfileirar(f, v) && v == 2, "segundo a sair deve ser o 2");
    verificar(desenfileirar(f, v) && v == 3, "terceiro a sair deve ser o 3");
    verificar(filaVazia(f), "depois de tirar tudo, fica vazia");
    verificar(f.tamanho == 0, "tamanho volta a 0");
    liberarFila(f);
}

void testeIntercalado() {
    cout << "== Enfileirar e desenfileirar intercalados ==\n";
    Fila f;
    criarFila(f);
    int v;
    enfileirar(f, 10);
    enfileirar(f, 20);
    verificar(desenfileirar(f, v) && v == 10, "sai o 10");
    enfileirar(f, 30);
    verificar(desenfileirar(f, v) && v == 20, "sai o 20");
    verificar(desenfileirar(f, v) && v == 30, "sai o 30");
    verificar(filaVazia(f), "fila vazia no fim");

    // Depois de esvaziar, a fila deve continuar funcionando normalmente.
    enfileirar(f, 40);
    verificar(f.tamanho == 1, "reusar fila vazia: tamanho 1");
    verificar(desenfileirar(f, v) && v == 40, "reusar fila vazia: sai o 40");
    liberarFila(f);
}

void testeMuitosElementos() {
    cout << "== Muitos elementos ==\n";
    Fila f;
    criarFila(f);
    const int QTD = 100000;
    for (int i = 0; i < QTD; i++) {
        enfileirar(f, i);
    }
    verificar(f.tamanho == QTD, "tamanho deve ser 100000");
    bool ordemCerta = true;
    int v;
    for (int i = 0; i < QTD; i++) {
        if (!desenfileirar(f, v) || v != i) {
            ordemCerta = false;
        }
    }
    verificar(ordemCerta, "100000 elementos saem na mesma ordem em que entraram");
    verificar(filaVazia(f), "fila vazia no fim");
    liberarFila(f);
}

void testeLiberar() {
    cout << "== Liberar fila com elementos ==\n";
    Fila f;
    criarFila(f);
    enfileirar(f, 5);
    enfileirar(f, 6);
    liberarFila(f);
    verificar(filaVazia(f), "liberar deixa a fila vazia");
    verificar(f.tamanho == 0, "liberar zera o tamanho");
}

int main() {
    cout << "Executando testes da Fila\n\n";
    testeFilaNova();
    testeOrdemFIFO();
    testeIntercalado();
    testeMuitosElementos();
    testeLiberar();

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
