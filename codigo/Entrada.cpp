#include "Entrada.h"

bool textoParaInteiro(const char *texto, int &numero) {
    int inicio = 0;
    bool negativo = false;
    if (texto[0] == '-') {
        negativo = true;
        inicio = 1;
    }
    int tamanho = 0;
    while (texto[tamanho] != '\0') {
        tamanho++;
    }
    int qtdDigitos = tamanho - inicio;
    if (qtdDigitos < 1 || qtdDigitos > 9) {
        return false;
    }
    numero = 0;
    for (int i = inicio; i < tamanho; i++) {
        char c = texto[i];
        if (c < '0' || c > '9') {
            return false;
        }
        numero = numero * 10 + (c - '0');
    }
    if (negativo) {
        numero = -numero;
    }
    return true;
}

bool nAceito(int n) {
    return n >= N_MINIMO && n <= N_MAXIMO;
}
