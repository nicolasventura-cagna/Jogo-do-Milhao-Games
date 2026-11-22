#ifndef CADASTRO_H
#define CADASTRO_H

#include "pergunta.h"

int inserir(Pergunta **pergunta, int *qtde);

void listar(Pergunta *pergunta, int qtde);

int pesquisar(Pergunta *pergunta, int qtde);

int alterar(Pergunta *pergunta, int qtde);

int excluir(Pergunta **pergunta, int *qtde);

void exibirPergunta(Pergunta pergunta);

void exibeEnunciado(Pergunta pergunta);

int adicionarPergunta(Pergunta **pergunta, int *qtde, Pergunta nova);

int removerPergunta(Pergunta **pergunta, int *qtde, int indice);

int alterarPergunta(Pergunta *pergunta, int qtde, int indice, Pergunta nova);

#endif