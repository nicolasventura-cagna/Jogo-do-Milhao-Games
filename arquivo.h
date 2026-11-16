
#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "pergunta.h"

int lerBinario(Pergunta **pergunta, int *qtde);

int escreveBinario(Pergunta *pergunta, int qtde);

int lerCSV(Pergunta **pergunta, int *qtde);

int exportaCSV(Pergunta *pergunta, int qtde);

#endif