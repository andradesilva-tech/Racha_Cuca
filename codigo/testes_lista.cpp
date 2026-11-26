// Testes da Lista ordenada, isolada do resto do projeto.
#include "ListaOrdenada.h"
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

void testeListaNova() {
    cout << "== Lista recem-criada ==\n";
    ListaOrdenada l;
    criarLista(l);
    verificar(listaVazia(l), "lista nova deve estar vazia");
    verificar(l.tamanho == 0, "lista nova deve ter tamanho 0");
    int p = 7, v = 7;
    verificar(!removerPrimeiro(l, p, v), "remover de lista vazia deve falhar");
    liberarLista(l);
}

void testeOrdenacao() {
    cout << "== Sai em ordem crescente de prioridade ==\n";
    ListaOrdenada l;
    criarLista(l);
    inserirOrdenado(l, 5, 50);
    inserirOrdenado(l, 1, 10);
    inserirOrdenado(l, 3, 30);
    inserirOrdenado(l, 4, 40);
    inserirOrdenado(l, 2, 20);
    verificar(l.tamanho == 5, "tamanho deve ser 5");

    int p, v;
    verificar(removerPrimeiro(l, p, v) && p == 1 && v == 10, "sai prioridade 1 (valor 10)");
    verificar(removerPrimeiro(l, p, v) && p == 2 && v == 20, "sai prioridade 2 (valor 20)");
    verificar(removerPrimeiro(l, p, v) && p == 3 && v == 30, "sai prioridade 3 (valor 30)");
    verificar(removerPrimeiro(l, p, v) && p == 4 && v == 40, "sai prioridade 4 (valor 40)");
    verificar(removerPrimeiro(l, p, v) && p == 5 && v == 50, "sai prioridade 5 (valor 50)");
    verificar(listaVazia(l), "lista vazia no fim");
    liberarLista(l);
}

// Percorre pelo 'prox' e pelo 'ant': os dois sentidos tem que contar
// 'tamanho' elementos, com 'ant' e 'fim' certos e prioridades em ordem.
bool ligacoesConsistentes(const ListaOrdenada &l) {
    int contaFrente = 0;
    NoLista *atual = l.inicio;
    NoLista *anterior = NULL;
    while (atual != NULL) {
        if (atual->ant != anterior) {
            return false;
        }
        if (anterior != NULL && anterior->prioridade > atual->prioridade) {
            return false;
        }
        contaFrente++;
        anterior = atual;
        atual = atual->prox;
    }
    if (anterior != l.fim) {
        return false;
    }
    int contaTras = 0;
    atual = l.fim;
    while (atual != NULL) {
        contaTras++;
        atual = atual->ant;
    }
    return contaFrente == l.tamanho && contaTras == l.tamanho;
}

void testeEmpates() {
    cout << "== Empates ==\n";
    int p, v;

    // Empate com o primeiro: o novo entra na frente.
    ListaOrdenada a;
    criarLista(a);
    inserirOrdenado(a, 2, 1);
    inserirOrdenado(a, 2, 2);
    verificar(removerPrimeiro(a, p, v) && v == 2, "empate com o primeiro: o mais novo sai antes");
    verificar(removerPrimeiro(a, p, v) && v == 1, "... e depois o mais antigo");
    liberarLista(a);

    // Empate no meio ou no fim: o novo entra depois dos iguais.
    ListaOrdenada b;
    criarLista(b);
    inserirOrdenado(b, 1, 10);
    inserirOrdenado(b, 5, 51);
    inserirOrdenado(b, 9, 90);
    inserirOrdenado(b, 5, 52);      // empata no meio
    inserirOrdenado(b, 9, 91);      // empata no fim
    verificar(ligacoesConsistentes(b), "ligacoes certas depois dos empates");
    int esperados[5] = {10, 51, 52, 90, 91};
    bool ordemCerta = true;
    for (int i = 0; i < 5; i++) {
        if (!removerPrimeiro(b, p, v) || v != esperados[i]) {
            ordemCerta = false;
        }
    }
    verificar(ordemCerta, "empate no meio/fim: o mais antigo sai antes");
    liberarLista(b);
}

void testeAleatorio() {
    cout << "== Muitas insercoes e remocoes sorteadas ==\n";
    srand(2026);
    ListaOrdenada l;
    criarLista(l);

    // Prioridades so de 0 a 49, para forcar muitos empates.
    for (int i = 0; i < 3000; i++) {
        inserirOrdenado(l, rand() % 50, i);
    }
    verificar(l.tamanho == 3000, "fase 1: 3000 elementos");
    verificar(ligacoesConsistentes(l), "fase 1: ligacoes e ordem corretas");

    bool emOrdem = true;
    int p, v, ultima = -1;
    for (int i = 0; i < 1000; i++) {
        if (!removerPrimeiro(l, p, v) || p < ultima) {
            emOrdem = false;
        }
        ultima = p;
    }
    verificar(emOrdem, "fase 2: 1000 removidos em ordem");
    verificar(ligacoesConsistentes(l), "fase 2: ligacoes certas apos remover");

    for (int i = 0; i < 3000; i++) {
        inserirOrdenado(l, rand() % 50, i);
    }
    verificar(l.tamanho == 5000, "fase 3: 5000 elementos");
    verificar(ligacoesConsistentes(l), "fase 3: ligacoes e ordem corretas");
    emOrdem = true;
    ultima = -1;
    int removidos = 0;
    while (removerPrimeiro(l, p, v)) {
        if (p < ultima) {
            emOrdem = false;
        }
        ultima = p;
        removidos++;
    }
    verificar(emOrdem && removidos == 5000, "fase 3: todos saem em ordem, nenhum perdido");
    verificar(l.inicio == NULL && l.fim == NULL, "lista vazia: inicio e fim nulos");

    inserirOrdenado(l, 3, 33);
    verificar(l.inicio == l.fim && l.tamanho == 1, "reusar lista vazia: 1 elemento");
    liberarLista(l);
}

void testeInsercaoNoMeio() {
    cout << "== Inserir no comeco, no meio e no fim ==\n";
    ListaOrdenada l;
    criarLista(l);
    inserirOrdenado(l, 10, 100);
    inserirOrdenado(l, 30, 300);
    inserirOrdenado(l, 20, 200);   // meio
    inserirOrdenado(l, 5, 50);     // comeco
    inserirOrdenado(l, 40, 400);   // fim

    int esperados[5] = {50, 100, 200, 300, 400};
    bool ordemCerta = true;
    int p, v;
    for (int i = 0; i < 5; i++) {
        if (!removerPrimeiro(l, p, v) || v != esperados[i]) {
            ordemCerta = false;
        }
    }
    verificar(ordemCerta, "comeco, meio e fim ficam na ordem certa");
    liberarLista(l);
}

void testeMuitosElementos() {
    cout << "== Muitos elementos em ordem decrescente ==\n";
    ListaOrdenada l;
    criarLista(l);
    const int QTD = 2000;
    for (int i = QTD; i >= 1; i--) {
        inserirOrdenado(l, i, i);
    }
    verificar(l.tamanho == QTD, "tamanho deve ser 2000");
    bool ordemCerta = true;
    int p, v;
    for (int i = 1; i <= QTD; i++) {
        if (!removerPrimeiro(l, p, v) || p != i) {
            ordemCerta = false;
        }
    }
    verificar(ordemCerta, "inseridos de tras para frente, saem em ordem crescente");
    liberarLista(l);
}

void testeLiberar() {
    cout << "== Liberar lista com elementos ==\n";
    ListaOrdenada l;
    criarLista(l);
    inserirOrdenado(l, 1, 1);
    inserirOrdenado(l, 2, 2);
    liberarLista(l);
    verificar(listaVazia(l), "liberar deixa a lista vazia");
    verificar(l.tamanho == 0, "liberar zera o tamanho");
}

int main() {
    cout << "Executando testes da Lista ordenada\n\n";
    testeListaNova();
    testeOrdenacao();
    testeEmpates();
    testeInsercaoNoMeio();
    testeMuitosElementos();
    testeAleatorio();
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
