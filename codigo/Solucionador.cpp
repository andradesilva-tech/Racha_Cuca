// Solucionador.cpp
#include "Solucionador.h"
#include "Fila.h"
#include "Pilha.h"
#include "TabelaHash.h"
#include "ListaOrdenada.h"
#include <cstdlib>

// Tamanhos das tabelas hash (numeros primos espalham melhor as chaves).
const int TAMANHO_HASH_BFS = 200003;      // N = 3 tem no maximo 181.440 estados
const int TAMANHO_HASH_AESTRELA = 1000003;
const int TAMANHO_HASH_CAMADAS = 100003;

// Trava de seguranca do A*: se passar desse numero de estados, desiste
// (protege a memoria em embaralhamentos muito dificeis).
const int LIMITE_NOS_AESTRELA = 2000000;

// =========================================================================
// Solucao (vetor dinamico de movimentos)
// =========================================================================

void criarSolucao(Solucao &s) {
    s.movimentos = NULL;
    s.quantidade = 0;
    s.capacidade = 0;
}

void adicionarMovimento(Solucao &s, int movimento) {
    if (s.quantidade == s.capacidade) {
        // Vetor cheio: aloca um com o DOBRO do espaco e copia tudo para ele.
        int novaCapacidade;
        if (s.capacidade == 0) {
            novaCapacidade = 16;
        } else {
            novaCapacidade = s.capacidade * 2;
        }
        int *novo = new int[novaCapacidade];
        for (int i = 0; i < s.quantidade; i++) {
            novo[i] = s.movimentos[i];
        }
        delete[] s.movimentos;
        s.movimentos = novo;
        s.capacidade = novaCapacidade;
    }
    s.movimentos[s.quantidade] = movimento;
    s.quantidade++;
}

void liberarSolucao(Solucao &s) {
    delete[] s.movimentos;
    criarSolucao(s);
}

// =========================================================================
// Nos da busca
// =========================================================================
// Cada estado descoberto durante a busca vira um "No". Todos ficam guardados
// num vetor dinamico, e as estruturas (Fila, Lista, Hash) guardam so a
// POSICAO (indice) do no nesse vetor.

struct No {
    unsigned long long codigo;  // o estado codificado num numero
    int g;                      // quantos movimentos desde o inicio
    int pai;                    // indice do no anterior (-1 = estado inicial)
    int movimento;              // movimento feito para sair do pai e chegar aqui
    bool fechado;               // (A*) true quando o no ja foi expandido
};

struct VetorNos {
    No *nos;
    int quantidade;
    int capacidade;
};

static void criarVetorNos(VetorNos &v) {
    v.nos = NULL;
    v.quantidade = 0;
    v.capacidade = 0;
}

// Acrescenta um no e devolve a posicao (indice) dele no vetor.
static int adicionarNo(VetorNos &v, unsigned long long codigo, int g, int pai, int movimento) {
    if (v.quantidade == v.capacidade) {
        int novaCapacidade;
        if (v.capacidade == 0) {
            novaCapacidade = 1024;
        } else {
            novaCapacidade = v.capacidade * 2;
        }
        No *novo = new No[novaCapacidade];
        for (int i = 0; i < v.quantidade; i++) {
            novo[i] = v.nos[i];
        }
        delete[] v.nos;
        v.nos = novo;
        v.capacidade = novaCapacidade;
    }
    int indice = v.quantidade;
    v.nos[indice].codigo = codigo;
    v.nos[indice].g = g;
    v.nos[indice].pai = pai;
    v.nos[indice].movimento = movimento;
    v.nos[indice].fechado = false;
    v.quantidade++;
    return indice;
}

static void liberarVetorNos(VetorNos &v) {
    delete[] v.nos;
    criarVetorNos(v);
}

// Monta a sequencia de movimentos do inicio ate o no 'indice' usando a PILHA.
// Subindo pelos pais (do fim para o comeco), cada movimento e empilhado.
// Depois, desempilhando, eles saem na ordem certa (do comeco para o fim).
static void montarCaminho(const VetorNos &v, int indice, Solucao &s) {
    Pilha pilha;
    criarPilha(pilha);
    int atual = indice;
    while (v.nos[atual].pai != -1) {
        empilhar(pilha, v.nos[atual].movimento);
        atual = v.nos[atual].pai;
    }
    int movimento;
    while (desempilhar(pilha, movimento)) {
        adicionarMovimento(s, movimento);
    }
    liberarPilha(pilha);
}

// =========================================================================
// Codificacao do tabuleiro inteiro num unico numero (usada por BFS e A*)
// =========================================================================
// Ideia: ler as pecas em ordem como "digitos" de um numero na base n*n.
// Ex.: N = 2 (base 4), pecas 1 2 3 0 -> 1*64 + 2*16 + 3*4 + 0 = 108.
// Para N = 4 (16 casas, base 16) o maior codigo ainda cabe num
// unsigned long long; para N = 5 e 6 nao cabe, por isso BFS e A* so aceitam N <= 4.

static unsigned long long codificar(const Tabuleiro &t) {
    int total = t.n * t.n;
    unsigned long long codigo = 0;
    for (int i = 0; i < total; i++) {
        codigo = codigo * total + t.pecas[i];
    }
    return codigo;
}

// Faz o caminho inverso: escreve no tabuleiro o estado guardado no codigo.
static void decodificar(Tabuleiro &t, unsigned long long codigo) {
    int total = t.n * t.n;
    for (int i = total - 1; i >= 0; i--) {   // o ultimo "digito" e o da ultima casa
        t.pecas[i] = (int)(codigo % total);
        codigo = codigo / total;
        if (t.pecas[i] == VAZIO) {
            t.vazio = i;
        }
    }
}

// =========================================================================
// BFS - busca em largura (N = 2 e 3)
// =========================================================================
// Visita os estados "por camadas de distancia": primeiro todos a 1 movimento,
// depois todos a 2, e assim por diante. Por isso, o primeiro estado resolvido
// encontrado e o de MENOR numero de movimentos.
//   Fila       -> guarda os estados na ordem em que devem ser visitados
//   Tabela Hash -> lembra os estados ja vistos (para nao repetir)
//   Pilha      -> monta a resposta no final (montarCaminho)

bool resolverBFS(const Tabuleiro &inicial, Solucao &s) {
    if (inicial.n > 4) {
        return false;                        // o estado nao cabe no codigo
    }
    Tabuleiro t;                             // copia de trabalho
    criarTabuleiroComEstado(t, inicial.n, inicial.pecas);

    VetorNos v;
    criarVetorNos(v);
    TabelaHash vistos;
    criarTabelaHash(vistos, TAMANHO_HASH_BFS);
    Fila fila;
    criarFila(fila);

    int raiz = adicionarNo(v, codificar(t), 0, -1, 0);
    inserirHash(vistos, v.nos[raiz].codigo, raiz);
    enfileirar(fila, raiz);

    bool achou = false;
    int atual;
    while (!achou && desenfileirar(fila, atual)) {
        decodificar(t, v.nos[atual].codigo);
        if (estaResolvido(t)) {
            montarCaminho(v, atual, s);
            achou = true;
        } else {
            int movimentos[4];
            int qtd = movimentosValidos(t, movimentos);
            for (int k = 0; k < qtd; k++) {
                aplicarMovimento(t, movimentos[k]);          // vai para o vizinho
                unsigned long long codigo = codificar(t);
                int jaVisto;
                if (!buscarHash(vistos, codigo, jaVisto)) {
                    int novo = adicionarNo(v, codigo, v.nos[atual].g + 1, atual, movimentos[k]);
                    inserirHash(vistos, codigo, novo);
                    enfileirar(fila, novo);
                }
                aplicarMovimento(t, movimentos[k]);          // desfaz (volta)
            }
        }
    }

    liberarFila(fila);
    liberarTabelaHash(vistos);
    liberarVetorNos(v);
    destruirTabuleiro(t);
    return achou;
}

// =========================================================================
// A* (N = 4)
// =========================================================================
// Para cada estado calculamos:
//   g = movimentos feitos ate aqui
//   h = estimativa do que falta: soma das distancias de Manhattan
//       (quantas casas cada peca esta longe do seu lugar, em linha + coluna)
//   f = g + h
// A Lista ordenada mantem os estados a explorar em ordem de f; o A* sempre
// continua pelo primeiro (o mais promissor). Como h nunca exagera o que falta,
// a solucao encontrada tem o menor numero de movimentos.

static int distanciaManhattan(const Tabuleiro &t) {
    int n = t.n;
    int soma = 0;
    for (int i = 0; i < n * n; i++) {
        int valor = t.pecas[i];
        if (valor != VAZIO) {
            int destino = valor - 1;                 // a peca 'valor' mora no indice valor-1
            int difLinha = i / n - destino / n;
            if (difLinha < 0) {
                difLinha = -difLinha;
            }
            int difColuna = i % n - destino % n;
            if (difColuna < 0) {
                difColuna = -difColuna;
            }
            soma = soma + difLinha + difColuna;
        }
    }
    return soma;
}

// Prioridade na lista: f = g + h. Em empate com o primeiro da lista, a Lista
// ordenada coloca o estado MAIS NOVO na frente, o que faz o A* "aprofundar"
// no caminho mais promissor antes de abrir outros com o mesmo f.
static int prioridadeAEstrela(int g, int h) {
    return g + h;
}

bool resolverAEstrela(const Tabuleiro &inicial, Solucao &s) {
    if (inicial.n > 4) {
        return false;
    }
    Tabuleiro t;
    criarTabuleiroComEstado(t, inicial.n, inicial.pecas);

    VetorNos v;
    criarVetorNos(v);
    TabelaHash vistos;
    criarTabelaHash(vistos, TAMANHO_HASH_AESTRELA);
    ListaOrdenada abertos;                   // estados descobertos e ainda nao expandidos
    criarLista(abertos);

    int raiz = adicionarNo(v, codificar(t), 0, -1, 0);
    inserirHash(vistos, v.nos[raiz].codigo, raiz);
    inserirOrdenado(abertos, prioridadeAEstrela(0, distanciaManhattan(t)), raiz);

    bool achou = false;
    int prioridade, atual;
    while (!achou && v.quantidade < LIMITE_NOS_AESTRELA
           && removerPrimeiro(abertos, prioridade, atual)) {
        // O mesmo no pode estar na lista mais de uma vez (quando achamos um
        // caminho mais curto ate ele). Se ja foi expandido, ignora a copia velha.
        if (v.nos[atual].fechado) {
            continue;
        }
        v.nos[atual].fechado = true;

        decodificar(t, v.nos[atual].codigo);
        if (estaResolvido(t)) {
            montarCaminho(v, atual, s);
            achou = true;
        } else {
            int gNovo = v.nos[atual].g + 1;
            int movimentos[4];
            int qtd = movimentosValidos(t, movimentos);
            for (int k = 0; k < qtd; k++) {
                aplicarMovimento(t, movimentos[k]);
                unsigned long long codigo = codificar(t);
                int indice;
                if (buscarHash(vistos, codigo, indice)) {
                    // Ja conhecido: so atualiza se achamos um caminho MAIS CURTO
                    // e ele ainda nao foi expandido.
                    if (!v.nos[indice].fechado && gNovo < v.nos[indice].g) {
                        v.nos[indice].g = gNovo;
                        v.nos[indice].pai = atual;
                        v.nos[indice].movimento = movimentos[k];
                        inserirOrdenado(abertos,
                                        prioridadeAEstrela(gNovo, distanciaManhattan(t)), indice);
                    }
                } else {
                    indice = adicionarNo(v, codigo, gNovo, atual, movimentos[k]);
                    inserirHash(vistos, codigo, indice);
                    inserirOrdenado(abertos,
                                    prioridadeAEstrela(gNovo, distanciaManhattan(t)), indice);
                }
                aplicarMovimento(t, movimentos[k]);          // desfaz
            }
        }
    }

    liberarLista(abertos);
    liberarTabelaHash(vistos);
    liberarVetorNos(v);
    destruirTabuleiro(t);
    return achou;
}

// =========================================================================
// Estrategia por camadas (N = 5 e 6)
// =========================================================================
// Igual a uma pessoa resolvendo: arruma a LINHA DE CIMA, depois a COLUNA DA
// ESQUERDA, e as casas arrumadas ficam TRAVADAS (nao mexemos mais nelas).
// Sobra um tabuleiro menor no canto de baixo/direita, e repetimos ate sobrar
// um quadrado 2x2, que e resolvido por ultimo.
//
// Para encaixar cada peca usamos uma BFS "reduzida": o estado guarda so a
// posicao do vazio e das pecas que estamos encaixando agora (1, 2 ou 3).
// As outras pecas nao importam. Isso deixa a busca pequena e rapida.
//
// As duas ultimas pecas de cada linha (e de cada coluna) sao encaixadas
// JUNTAS: tentar colocar uma de cada vez costuma tirar a outra do lugar.

// Indice onde a peca de valor 'valor' esta agora.
static int posicaoDaPeca(const Tabuleiro &t, int valor) {
    for (int i = 0; i < t.n * t.n; i++) {
        if (t.pecas[i] == valor) {
            return i;
        }
    }
    return -1;
}

// Codifica (vazio, posicoes das pecas) num numero, como "digitos" na base total.
static unsigned long long codificarReduzido(int vazio, const int pos[], int qtd, int total) {
    unsigned long long codigo = vazio;
    for (int i = 0; i < qtd; i++) {
        codigo = codigo * total + pos[i];
    }
    return codigo;
}

static void decodificarReduzido(unsigned long long codigo, int &vazio, int pos[], int qtd, int total) {
    for (int i = qtd - 1; i >= 0; i--) {
        pos[i] = (int)(codigo % total);
        codigo = codigo / total;
    }
    vazio = (int)codigo;
}

// Encaixa as pecas de 'pecas' (1 a 3 valores) nos seus lugares finais, sem
// passar o vazio pelas casas travadas. Aplica os movimentos no tabuleiro 't'
// e acrescenta cada um em 's'. Devolve false se nao for possivel.
static bool encaixarPecas(Tabuleiro &t, const bool travada[], const int pecas[], int qtdPecas, Solucao &s) {
    int n = t.n;
    int total = n * n;

    int posInicial[3];
    for (int i = 0; i < qtdPecas; i++) {
        posInicial[i] = posicaoDaPeca(t, pecas[i]);
    }

    VetorNos v;
    criarVetorNos(v);
    TabelaHash vistos;
    criarTabelaHash(vistos, TAMANHO_HASH_CAMADAS);
    Fila fila;
    criarFila(fila);

    // Aqui o "movimento" guardado em cada no e a CASA para onde o vazio foi.
    int raiz = adicionarNo(v, codificarReduzido(t.vazio, posInicial, qtdPecas, total), 0, -1, 0);
    inserirHash(vistos, v.nos[raiz].codigo, raiz);
    enfileirar(fila, raiz);

    int final = -1;
    int atual;
    while (final == -1 && desenfileirar(fila, atual)) {
        int vazio;
        int pos[3];
        decodificarReduzido(v.nos[atual].codigo, vazio, pos, qtdPecas, total);

        bool pronto = true;
        for (int i = 0; i < qtdPecas; i++) {
            if (pos[i] != pecas[i] - 1) {
                pronto = false;
            }
        }
        if (pronto) {
            final = atual;
        } else {
            // Casas vizinhas do vazio (cima, baixo, esquerda, direita).
            int linha = vazio / n;
            int coluna = vazio % n;
            int vizinhos[4];
            int qtdVizinhos = 0;
            if (linha > 0) {
                vizinhos[qtdVizinhos] = vazio - n;
                qtdVizinhos++;
            }
            if (linha < n - 1) {
                vizinhos[qtdVizinhos] = vazio + n;
                qtdVizinhos++;
            }
            if (coluna > 0) {
                vizinhos[qtdVizinhos] = vazio - 1;
                qtdVizinhos++;
            }
            if (coluna < n - 1) {
                vizinhos[qtdVizinhos] = vazio + 1;
                qtdVizinhos++;
            }

            for (int k = 0; k < qtdVizinhos; k++) {
                int casa = vizinhos[k];
                if (!travada[casa]) {
                    // O vazio vai para 'casa'. Se uma das nossas pecas estava
                    // la, ela vai para onde o vazio estava.
                    int novaPos[3];
                    for (int i = 0; i < qtdPecas; i++) {
                        if (pos[i] == casa) {
                            novaPos[i] = vazio;
                        } else {
                            novaPos[i] = pos[i];
                        }
                    }
                    unsigned long long codigo = codificarReduzido(casa, novaPos, qtdPecas, total);
                    int jaVisto;
                    if (!buscarHash(vistos, codigo, jaVisto)) {
                        int novo = adicionarNo(v, codigo, v.nos[atual].g + 1, atual, casa);
                        inserirHash(vistos, codigo, novo);
                        enfileirar(fila, novo);
                    }
                }
            }
        }
    }

    bool ok = false;
    if (final != -1) {
        // Sequencia de casas por onde o vazio passa (montada com a Pilha).
        Solucao casas;
        criarSolucao(casas);
        montarCaminho(v, final, casas);
        // Traduz cada casa para o VALOR da peca que esta nela e aplica o movimento.
        for (int i = 0; i < casas.quantidade; i++) {
            int valor = t.pecas[casas.movimentos[i]];
            aplicarMovimento(t, valor);
            adicionarMovimento(s, valor);
        }
        liberarSolucao(casas);
        ok = true;
    }

    liberarFila(fila);
    liberarTabelaHash(vistos);
    liberarVetorNos(v);
    return ok;
}

bool resolverCamadas(const Tabuleiro &inicial, Solucao &s) {
    int n = inicial.n;
    Tabuleiro t;
    criarTabuleiroComEstado(t, n, inicial.pecas);

    bool *travada = new bool[n * n];
    for (int i = 0; i < n * n; i++) {
        travada[i] = false;
    }

    bool ok = true;
    int k = 0;   // a parte que falta resolver comeca na linha k e na coluna k
    while (ok && n - k > 2) {
        // 1) Linha de cima da parte que falta: casas (k, k) ate (k, n-1).
        for (int c = k; c <= n - 3 && ok; c++) {
            int peca[1];
            peca[0] = k * n + c + 1;               // peca que mora na casa (k, c)
            ok = encaixarPecas(t, travada, peca, 1, s);
            travada[k * n + c] = true;
        }
        if (ok) {                                   // as duas ultimas da linha, juntas
            int par[2];
            par[0] = k * n + (n - 2) + 1;
            par[1] = k * n + (n - 1) + 1;
            ok = encaixarPecas(t, travada, par, 2, s);
            travada[k * n + (n - 2)] = true;
            travada[k * n + (n - 1)] = true;
        }

        // 2) Coluna da esquerda da parte que falta: casas (k+1, k) ate (n-1, k).
        for (int l = k + 1; l <= n - 3 && ok; l++) {
            int peca[1];
            peca[0] = l * n + k + 1;
            ok = encaixarPecas(t, travada, peca, 1, s);
            travada[l * n + k] = true;
        }
        if (ok) {                                   // as duas ultimas da coluna, juntas
            int par[2];
            par[0] = (n - 2) * n + k + 1;
            par[1] = (n - 1) * n + k + 1;
            ok = encaixarPecas(t, travada, par, 2, s);
            travada[(n - 2) * n + k] = true;
            travada[(n - 1) * n + k] = true;
        }
        k++;
    }

    // 3) Sobrou o quadrado 2x2 do canto: encaixa as 3 pecas finais juntas
    //    (o vazio termina sozinho no canto inferior direito).
    if (ok) {
        int ultimas[3];
        ultimas[0] = (n - 2) * n + (n - 2) + 1;
        ultimas[1] = (n - 2) * n + (n - 1) + 1;
        ultimas[2] = (n - 1) * n + (n - 2) + 1;
        ok = encaixarPecas(t, travada, ultimas, 3, s);
    }

    if (ok && !estaResolvido(t)) {
        ok = false;
    }

    delete[] travada;
    destruirTabuleiro(t);
    return ok;
}

// =========================================================================
// Escolha da estrategia
// =========================================================================

int resolver(const Tabuleiro &t, Solucao &s) {
    if (t.n <= 3) {
        if (resolverBFS(t, s)) {
            return ESTRATEGIA_BFS;
        }
        return ESTRATEGIA_NENHUMA;
    }
    if (t.n == 4) {
        if (resolverAEstrela(t, s)) {
            return ESTRATEGIA_AESTRELA;
        }
        // O A* desistiu (limite de estados): descarta o que tiver ficado
        // em 's' e tenta pela estrategia por camadas.
        liberarSolucao(s);
        criarSolucao(s);
    }
    if (resolverCamadas(t, s)) {
        return ESTRATEGIA_CAMADAS;
    }
    return ESTRATEGIA_NENHUMA;
}

const char *nomeEstrategia(int estrategia) {
    if (estrategia == ESTRATEGIA_BFS) {
        return "busca em largura (BFS)";
    }
    if (estrategia == ESTRATEGIA_AESTRELA) {
        return "A* com distancia de Manhattan";
    }
    if (estrategia == ESTRATEGIA_CAMADAS) {
        return "estrategia por camadas";
    }
    return "nenhuma";
}
