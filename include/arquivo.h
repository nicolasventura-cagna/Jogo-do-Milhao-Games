#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "pergunta.h"

#define RANKING_MAX 10 //quantos jogadores o ranking guarda

typedef struct{
    char nome[21];
    int cash;
    int modo; //0 = Facil, 1 = Medio, 2 = Dificil
}Registro;

int lerBinario(Pergunta **pergunta, int *qtde);

int escreveBinario(Pergunta *pergunta, int qtde);

int lerCSV(Pergunta **pergunta, int *qtde);

int salvaCSV(Pergunta *pergunta, int qtde);

int carregarRanking(Registro *vet, int max);

int salvarRanking(Registro novo);

#endif