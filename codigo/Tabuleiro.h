// Tabuleiro.h
// Representacao do quebra-cabeca deslizante (Racha Cuca).
// Estilo procedural: um 'struct' guarda os dados e funcoes soltas operam sobre
// ele. O tabuleiro nao conhece nada sobre imagem: lida so com pecas numericas.
#ifndef TABULEIRO_H
#define TABULEIRO_H

// Valor que representa o espaco vazio dentro do tabuleiro.
const int VAZIO = 0;

struct Tabuleiro {
    int n;       // dimensao: o tabuleiro e n x n
    int *pecas;  // vetor alocado dinamicamente, tamanho n*n, em ordem de leitura
    int vazio;   // indice do espaco vazio dentro do vetor 'pecas'
};

// Cria o tabuleiro n x n no estado RESOLVIDO: 1, 2, ..., n*n-1 em ordem de
// leitura e o espaco vazio no canto inferior direito.
// ATENCAO: aloca memoria; chame destruirTabuleiro ao terminar.
void criarTabuleiro(Tabuleiro &t, int n);

// Cria o tabuleiro n x n a partir de um vetor de valores (ordem de leitura).
// Util para montar cenarios de teste. Tambem aloca memoria.
void criarTabuleiroComEstado(Tabuleiro &t, int n, const int valores[]);

// Libera a memoria do tabuleiro.
void destruirTabuleiro(Tabuleiro &t);

// Devolve o valor da peca na posicao (linha, coluna); VAZIO = espaco vazio.
int pecaEm(const Tabuleiro &t, int linha, int coluna);

// Preenche o vetor 'movimentos' (que precisa caber 4) com os valores das pecas
// que podem deslizar para o vazio neste momento e devolve a quantidade:
// 2 quando o vazio esta num canto, 3 numa borda e 4 no centro.
// A ordem e sempre: cima, baixo, esquerda, direita (em relacao ao vazio).
int movimentosValidos(const Tabuleiro &t, int movimentos[]);

// Move a peca de valor 'valorPeca' para o espaco vazio, se ela for vizinha do
// vazio. Devolve true se o movimento foi feito; false se for invalido (peca
// inexistente ou nao vizinha) -- nesse caso o tabuleiro nao muda.
bool aplicarMovimento(Tabuleiro &t, int valorPeca);

// Devolve true se o tabuleiro esta no estado resolvido.
bool estaResolvido(const Tabuleiro &t);

// Imprime o tabuleiro no terminal (o espaco vazio aparece em branco).
void imprimirTabuleiro(const Tabuleiro &t);

#endif
