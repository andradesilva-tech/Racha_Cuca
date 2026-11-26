// Estado do quebra-cabeca e movimentos. Lida so com numeros, nao com imagem.
#ifndef TABULEIRO_H
#define TABULEIRO_H

const int VAZIO = 0;

struct Tabuleiro {
    int n;       // o tabuleiro e n x n
    int *pecas;  // n*n valores, linha por linha
    int vazio;   // indice do espaco vazio em 'pecas'
};

// Estado resolvido: 1, 2, ..., n*n-1 e o vazio no canto inferior direito.
// Aloca memoria: chame destruirTabuleiro ao terminar.
void criarTabuleiro(Tabuleiro &t, int n);

// Monta o tabuleiro com os valores dados (usado nos testes). Tambem aloca.
void criarTabuleiroComEstado(Tabuleiro &t, int n, const int valores[]);

void destruirTabuleiro(Tabuleiro &t);
int pecaEm(const Tabuleiro &t, int linha, int coluna);

// Coloca em 'movimentos' (tamanho 4) as pecas que podem ir para o vazio,
// na ordem cima, baixo, esquerda, direita. Devolve quantas sao (2, 3 ou 4).
int movimentosValidos(const Tabuleiro &t, int movimentos[]);

// Desliza a peca para o vazio. Devolve false (sem mudar nada) se ela nao
// existir ou nao estiver ao lado do vazio.
bool aplicarMovimento(Tabuleiro &t, int valorPeca);

bool estaResolvido(const Tabuleiro &t);
void imprimirTabuleiro(const Tabuleiro &t);

// Faz 'quantidade' movimentos aleatorios validos, sem desfazer o anterior.
// Como so usa movimentos validos, sempre tem solucao. Se cair no estado
// resolvido, continua ate sair dele. Devolve quantos movimentos fez.
// Chame srand uma vez antes (no main).
int embaralhar(Tabuleiro &t, int quantidade);

#endif
