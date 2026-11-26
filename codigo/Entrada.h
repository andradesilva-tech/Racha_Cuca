// Validacao do que o usuario digita (separada do main para poder ser testada).
#ifndef ENTRADA_H
#define ENTRADA_H

const int N_MINIMO = 2;
const int N_MAXIMO = 6;

// Converte texto em inteiro. Devolve false se tiver algo que nao seja digito
// (ex.: "abc", "3.5", "4x", "") ou se tiver mais de 9 digitos.
bool textoParaInteiro(const char *texto, int &numero);

// true se o programa aceita esse tamanho de tabuleiro.
bool nAceito(int n);

#endif
