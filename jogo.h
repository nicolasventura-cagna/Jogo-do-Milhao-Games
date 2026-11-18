#ifndef JOGO_H
#define JOGO_H

#include "pergunta.h"
// definido em "jogo.h"
typedef struct {
    int modo;
    int vidas;
    float multiplicador;
    float bonus;
    int cash;
    int acertosSeg;
    int rodada;
    int *usadas;
    int qtdUsadas;
    int altRemovida[4];
} Partida;

void iniciarPartida(Partida *p, int modo);

int sortearPergunta(Partida *p, Pergunta *perguntas, int qtde, int nivel);

int conferirResposta(Partida *p, Pergunta pergunta, char resposta);

int removerAlternativaErrada(Partida *p, Pergunta pergunta);

int aplicarKillstreak(Partida *p);

void liberarPartida(Partida *p);

void exibirPerguntaJogo(Pergunta pergunta, Partida *p);

int temPerguntasSuficientes(Pergunta *perguntas, int qtde);

int jogar(Pergunta *perguntas, int qtde);

#endif

