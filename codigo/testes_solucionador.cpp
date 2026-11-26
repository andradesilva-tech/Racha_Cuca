// Testes das estrategias de solucao: todo movimento tem que ser valido e o
// tabuleiro tem que terminar resolvido.
#include "Tabuleiro.h"
#include "Solucionador.h"
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

// Aplica a solucao numa copia, para nao alterar o tabuleiro original.
bool solucaoValida(const Tabuleiro &inicial, const Solucao &s) {
    Tabuleiro copia;
    criarTabuleiroComEstado(copia, inicial.n, inicial.pecas);
    bool ok = true;
    for (int i = 0; i < s.quantidade && ok; i++) {
        if (!aplicarMovimento(copia, s.movimentos[i])) {
            ok = false;
        }
    }
    if (ok && !estaResolvido(copia)) {
        ok = false;
    }
    destruirTabuleiro(copia);
    return ok;
}

void testeJaResolvido() {
    cout << "== Tabuleiro ja resolvido: 0 movimentos ==\n";
    Tabuleiro t;
    Solucao s;

    criarTabuleiro(t, 3);
    criarSolucao(s);
    verificar(resolverBFS(t, s) && s.quantidade == 0, "BFS em 3x3 resolvido: 0 movimentos");
    liberarSolucao(s);
    destruirTabuleiro(t);

    criarTabuleiro(t, 4);
    criarSolucao(s);
    verificar(resolverAEstrela(t, s) && s.quantidade == 0, "A* em 4x4 resolvido: 0 movimentos");
    liberarSolucao(s);
    destruirTabuleiro(t);

    criarTabuleiro(t, 5);
    criarSolucao(s);
    verificar(resolverCamadas(t, s) && s.quantidade == 0, "camadas em 5x5 resolvido: 0 movimentos");
    liberarSolucao(s);
    destruirTabuleiro(t);
}

void testeUmMovimento() {
    cout << "== A um movimento da solucao ==\n";
    Tabuleiro t;
    criarTabuleiro(t, 3);
    aplicarMovimento(t, 8);                    // tira o 8 do lugar
    Solucao s;
    criarSolucao(s);
    verificar(resolverBFS(t, s), "BFS acha a solucao");
    verificar(s.quantidade == 1, "a solucao tem exatamente 1 movimento");
    verificar(s.quantidade == 1 && s.movimentos[0] == 8, "o movimento e devolver o 8");
    liberarSolucao(s);
    destruirTabuleiro(t);
}

void testeBFS() {
    cout << "== BFS para N = 2 e 3 ==\n";
    for (int n = 2; n <= 3; n++) {
        for (int semente = 1; semente <= 10; semente++) {
            srand(semente);
            Tabuleiro t;
            criarTabuleiro(t, n);
            embaralhar(t, 40);
            Solucao s;
            criarSolucao(s);
            verificar(resolverBFS(t, s), "BFS encontra a solucao");
            verificar(solucaoValida(t, s), "BFS: movimentos validos e tabuleiro resolvido");
            liberarSolucao(s);
            destruirTabuleiro(t);
        }
    }
}

void testeBFSeAEstrelaIguais() {
    cout << "== BFS e A* acham o MESMO numero minimo de movimentos (3x3) ==\n";
    // As duas estrategias sao otimas, entao o tamanho tem que bater.
    for (int semente = 1; semente <= 10; semente++) {
        srand(100 + semente);
        Tabuleiro t;
        criarTabuleiro(t, 3);
        embaralhar(t, 60);
        Solucao sBFS, sA;
        criarSolucao(sBFS);
        criarSolucao(sA);
        resolverBFS(t, sBFS);
        resolverAEstrela(t, sA);
        verificar(sBFS.quantidade == sA.quantidade, "BFS e A* com o mesmo tamanho");
        verificar(solucaoValida(t, sA), "A* em 3x3: solucao valida");
        liberarSolucao(sBFS);
        liberarSolucao(sA);
        destruirTabuleiro(t);
    }
}

void testeAEstrela() {
    cout << "== A* para N = 4 ==\n";
    for (int semente = 1; semente <= 5; semente++) {
        srand(200 + semente);
        Tabuleiro t;
        criarTabuleiro(t, 4);
        int feitos = embaralhar(t, 40);
        Solucao s;
        criarSolucao(s);
        verificar(resolverAEstrela(t, s), "A* encontra a solucao");
        verificar(solucaoValida(t, s), "A*: movimentos validos e tabuleiro resolvido");
        // O proprio embaralhamento (ao contrario) ja e uma solucao; a do A*,
        // por ser a menor, nunca pode ser mais longa que ele.
        verificar(s.quantidade <= feitos, "A* nao usa mais movimentos que o embaralhamento");
        liberarSolucao(s);
        destruirTabuleiro(t);
    }
}

void testeCamadas() {
    cout << "== Camadas para N = 5 e 6 (e tambem 3 e 4, para conferir) ==\n";
    for (int n = 3; n <= 6; n++) {
        for (int semente = 1; semente <= 8; semente++) {
            srand(300 + 10 * n + semente);
            Tabuleiro t;
            criarTabuleiro(t, n);
            embaralhar(t, 500);
            Solucao s;
            criarSolucao(s);
            verificar(resolverCamadas(t, s), "camadas encontra a solucao");
            verificar(solucaoValida(t, s), "camadas: movimentos validos e tabuleiro resolvido");
            liberarSolucao(s);
            destruirTabuleiro(t);
        }
    }
}

void testeResolverEscolhe() {
    cout << "== resolver() escolhe a estrategia certa para cada N ==\n";
    // Estrategia esperada para N = 2, 3, 4, 5 e 6 (posicao 0 = N 2).
    int esperada[5] = {ESTRATEGIA_BFS, ESTRATEGIA_BFS, ESTRATEGIA_AESTRELA,
                       ESTRATEGIA_CAMADAS, ESTRATEGIA_CAMADAS};
    for (int n = 2; n <= 6; n++) {
        srand(400 + n);
        Tabuleiro t;
        criarTabuleiro(t, n);
        embaralhar(t, 40);
        Solucao s;
        criarSolucao(s);
        int estrategia = resolver(t, s);
        verificar(estrategia == esperada[n - 2], "resolver usa a estrategia certa para o N");
        verificar(solucaoValida(t, s), "resolver: solucao valida");
        liberarSolucao(s);
        destruirTabuleiro(t);
    }
}

void testePlanoB() {
    cout << "== 4x4 muito embaralhado: se o A* desistir, as camadas resolvem ==\n";
    // Com 100 movimentos aleatorios, alguns embaralhamentos sao dificeis
    // demais para o A* dentro do limite de estados. O resolver nao pode
    // ficar sem resposta: tem que cair para a estrategia por camadas.
    srand(1);
    Tabuleiro t;
    criarTabuleiro(t, 4);
    embaralhar(t, 100);

    Solucao soA;
    criarSolucao(soA);
    bool aEstrelaConseguiu = resolverAEstrela(t, soA);
    liberarSolucao(soA);

    Solucao s;
    criarSolucao(s);
    int estrategia = resolver(t, s);
    if (aEstrelaConseguiu) {
        verificar(estrategia == ESTRATEGIA_AESTRELA, "se o A* consegue, o resolver usa o A*");
    } else {
        verificar(estrategia == ESTRATEGIA_CAMADAS, "se o A* desiste, o resolver usa as camadas");
    }
    verificar(solucaoValida(t, s), "a solucao do plano B e valida");
    liberarSolucao(s);
    destruirTabuleiro(t);
}

void testeLimites() {
    cout << "== BFS e A* recusam N maior que 4 ==\n";
    Tabuleiro t;
    criarTabuleiro(t, 5);
    Solucao s;
    criarSolucao(s);
    verificar(!resolverBFS(t, s), "BFS recusa 5x5");
    verificar(!resolverAEstrela(t, s), "A* recusa 5x5");
    liberarSolucao(s);
    destruirTabuleiro(t);
}

int main() {
    cout << "Executando testes do Solucionador\n\n";
    testeJaResolvido();
    testeUmMovimento();
    testeBFS();
    testeBFSeAEstrelaIguais();
    testeAEstrela();
    testeCamadas();
    testeResolverEscolhe();
    testePlanoB();
    testeLimites();

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
