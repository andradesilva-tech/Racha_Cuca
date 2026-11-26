// Testes da Tabela Hash, isolada do resto do projeto.
#include "TabelaHash.h"
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

void testeTabelaNova() {
    cout << "== Tabela recem-criada ==\n";
    TabelaHash h;
    criarTabelaHash(h, 101);
    verificar(h.tamanho == 101, "tamanho deve ser 101");
    verificar(h.quantidade == 0, "tabela nova nao tem pares");
    int v = 777;
    verificar(!buscarHash(h, 42, v), "buscar numa tabela vazia nao acha nada");
    verificar(v == 777, "valor nao muda quando a busca falha");
    liberarTabelaHash(h);
}

void testeInserirEBuscar() {
    cout << "== Inserir e buscar ==\n";
    TabelaHash h;
    criarTabelaHash(h, 101);
    verificar(inserirHash(h, 5, 50), "inserir chave nova deve funcionar");
    verificar(inserirHash(h, 17, 170), "inserir outra chave nova");
    verificar(h.quantidade == 2, "quantidade deve ser 2");

    int v;
    verificar(buscarHash(h, 5, v) && v == 50, "chave 5 -> valor 50");
    verificar(buscarHash(h, 17, v) && v == 170, "chave 17 -> valor 170");
    verificar(!buscarHash(h, 6, v), "chave 6 nao foi inserida");
    liberarTabelaHash(h);
}

void testeChaveRepetida() {
    cout << "== Chave repetida ==\n";
    TabelaHash h;
    criarTabelaHash(h, 101);
    inserirHash(h, 9, 90);
    verificar(!inserirHash(h, 9, 999), "inserir chave repetida deve falhar");
    int v;
    verificar(buscarHash(h, 9, v) && v == 90, "o valor original continua la");
    verificar(h.quantidade == 1, "quantidade nao aumenta com repeticao");
    liberarTabelaHash(h);
}

void testeColisoes() {
    cout << "== Muitas colisoes (tabela pequena) ==\n";
    // Com so 7 posicoes e 100 chaves, varias chaves caem na mesma posicao.
    // O encadeamento tem que guardar todas sem perder nenhuma.
    TabelaHash h;
    criarTabelaHash(h, 7);
    for (int i = 0; i < 100; i++) {
        inserirHash(h, i, i * 10);
    }
    verificar(h.quantidade == 100, "todas as 100 chaves foram guardadas");

    bool todasAchadas = true;
    int v;
    for (int i = 0; i < 100; i++) {
        if (!buscarHash(h, i, v) || v != i * 10) {
            todasAchadas = false;
        }
    }
    verificar(todasAchadas, "todas as chaves sao encontradas com o valor certo");

    bool nenhumaExtra = true;
    for (int i = 100; i < 200; i++) {
        if (buscarHash(h, i, v)) {
            nenhumaExtra = false;
        }
    }
    verificar(nenhumaExtra, "chaves nunca inseridas nao sao encontradas");

    // 0, 7 e 14 caem todas na posicao 0 (resto da divisao por 7 e 0).
    verificar(funcaoHash(h, 0) == 0 && funcaoHash(h, 7) == 0 && funcaoHash(h, 14) == 0,
              "0, 7 e 14 colidem na posicao 0");
    liberarTabelaHash(h);
}

void testeChavesGrandes() {
    cout << "== Chaves muito grandes ==\n";
    // O estado de um tabuleiro 4x4 vira um numero enorme; a tabela precisa
    // funcionar com isso. O sufixo ULL indica um "unsigned long long".
    TabelaHash h;
    criarTabelaHash(h, 1009);
    unsigned long long grande1 = 18446744073709551615ULL;   // maior valor possivel
    unsigned long long grande2 = 1234567890123456789ULL;
    verificar(inserirHash(h, grande1, 1), "insere a maior chave possivel");
    verificar(inserirHash(h, grande2, 2), "insere outra chave grande");
    int v;
    verificar(buscarHash(h, grande1, v) && v == 1, "acha a maior chave");
    verificar(buscarHash(h, grande2, v) && v == 2, "acha a outra chave grande");
    verificar(!buscarHash(h, grande1 - 1, v), "uma chave vizinha nao e confundida");

    int pos = funcaoHash(h, grande1);
    verificar(pos >= 0 && pos < 1009, "posicao calculada fica dentro da tabela");
    liberarTabelaHash(h);
}

int main() {
    cout << "Executando testes da Tabela Hash\n\n";
    testeTabelaNova();
    testeInserirEBuscar();
    testeChaveRepetida();
    testeColisoes();
    testeChavesGrandes();

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
