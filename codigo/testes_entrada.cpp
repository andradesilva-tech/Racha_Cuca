// Testes da validacao de entrada: textos que nao sao numero e N nao aceito.
#include "Entrada.h"
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

void testeNumerosValidos() {
    cout << "== Textos que sao numeros inteiros ==\n";
    int n;
    verificar(textoParaInteiro("3", n) && n == 3, "\"3\" vira 3");
    verificar(textoParaInteiro("0", n) && n == 0, "\"0\" vira 0");
    verificar(textoParaInteiro("-2", n) && n == -2, "\"-2\" vira -2");
    verificar(textoParaInteiro("007", n) && n == 7, "\"007\" vira 7");
    verificar(textoParaInteiro("123456789", n) && n == 123456789, "9 digitos e o limite");
}

void testeTextosInvalidos() {
    cout << "== Textos que nao sao numeros inteiros ==\n";
    int n = 99;
    verificar(!textoParaInteiro("abc", n), "\"abc\" e recusado");
    verificar(!textoParaInteiro("3.5", n), "\"3.5\" e recusado");
    verificar(!textoParaInteiro("3,5", n), "\"3,5\" e recusado");
    verificar(!textoParaInteiro("4x", n), "\"4x\" e recusado");
    verificar(!textoParaInteiro("x4", n), "\"x4\" e recusado");
    verificar(!textoParaInteiro("", n), "texto vazio e recusado");
    verificar(!textoParaInteiro("-", n), "so o sinal e recusado");
    verificar(!textoParaInteiro("+3", n), "\"+3\" e recusado");
    verificar(!textoParaInteiro("1234567890", n), "10 digitos e recusado (estouraria o int)");
}

void testeNAceito() {
    cout << "== Valores de N aceitos pelo programa ==\n";
    for (int n = 2; n <= 6; n++) {
        verificar(nAceito(n), "N de 2 a 6 e aceito");
    }
    verificar(!nAceito(1), "N = 1 nao e aceito");
    verificar(!nAceito(0), "N = 0 nao e aceito");
    verificar(!nAceito(-3), "N negativo nao e aceito");
    verificar(!nAceito(7), "N = 7 nao e aceito");
    verificar(!nAceito(100), "N = 100 nao e aceito");
}

int main() {
    cout << "Executando testes da Entrada\n\n";
    testeNumerosValidos();
    testeTextosInvalidos();
    testeNAceito();

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
