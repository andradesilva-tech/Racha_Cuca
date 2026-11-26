// Testes da Pilha, isolada do resto do projeto.
#include "Pilha.h"
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

void testePilhaNova() {
    cout << "== Pilha recem-criada ==\n";
    Pilha p;
    criarPilha(p);
    verificar(pilhaVazia(p), "pilha nova deve estar vazia");
    verificar(p.tamanho == 0, "pilha nova deve ter tamanho 0");
    int v = 777;
    verificar(!desempilhar(p, v), "desempilhar de pilha vazia deve falhar");
    verificar(v == 777, "valor nao muda quando desempilhar falha");
    liberarPilha(p);
}

void testeOrdemLIFO() {
    cout << "== Ordem LIFO (ultimo a entrar, primeiro a sair) ==\n";
    Pilha p;
    criarPilha(p);
    empilhar(p, 1);
    empilhar(p, 2);
    empilhar(p, 3);
    verificar(!pilhaVazia(p), "pilha com elementos nao esta vazia");
    verificar(p.tamanho == 3, "tamanho deve ser 3");

    int v;
    verificar(desempilhar(p, v) && v == 3, "primeiro a sair deve ser o 3");
    verificar(desempilhar(p, v) && v == 2, "segundo a sair deve ser o 2");
    verificar(desempilhar(p, v) && v == 1, "terceiro a sair deve ser o 1");
    verificar(pilhaVazia(p), "depois de tirar tudo, fica vazia");
    verificar(p.tamanho == 0, "tamanho volta a 0");
    liberarPilha(p);
}

void testeInverterSequencia() {
    cout << "== Inverter uma sequencia (como na montagem da solucao) ==\n";
    // Mesmo uso do montarCaminho: entra do fim para o comeco, sai na ordem certa.
    int descobertos[5] = {50, 40, 30, 20, 10};
    int esperados[5]   = {10, 20, 30, 40, 50};
    Pilha p;
    criarPilha(p);
    for (int i = 0; i < 5; i++) {
        empilhar(p, descobertos[i]);
    }
    bool ordemCerta = true;
    int v;
    for (int i = 0; i < 5; i++) {
        if (!desempilhar(p, v) || v != esperados[i]) {
            ordemCerta = false;
        }
    }
    verificar(ordemCerta, "a pilha inverte a sequencia");
    liberarPilha(p);
}

void testeMuitosElementos() {
    cout << "== Muitos elementos ==\n";
    Pilha p;
    criarPilha(p);
    const int QTD = 100000;
    for (int i = 0; i < QTD; i++) {
        empilhar(p, i);
    }
    verificar(p.tamanho == QTD, "tamanho deve ser 100000");
    bool ordemCerta = true;
    int v;
    for (int i = QTD - 1; i >= 0; i--) {
        if (!desempilhar(p, v) || v != i) {
            ordemCerta = false;
        }
    }
    verificar(ordemCerta, "100000 elementos saem na ordem inversa");
    verificar(pilhaVazia(p), "pilha vazia no fim");
    liberarPilha(p);
}

void testeLiberar() {
    cout << "== Liberar pilha com elementos ==\n";
    Pilha p;
    criarPilha(p);
    empilhar(p, 5);
    empilhar(p, 6);
    liberarPilha(p);
    verificar(pilhaVazia(p), "liberar deixa a pilha vazia");
    verificar(p.tamanho == 0, "liberar zera o tamanho");
}

int main() {
    cout << "Executando testes da Pilha\n\n";
    testePilhaNova();
    testeOrdemLIFO();
    testeInverterSequencia();
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
