#ifndef PERGUNTA_H
#define PERGUNTA_H
// definido em "pergunta.h"
typedef struct{
    char enunciado[201];
    char alternativa[4][101];
    char correta;
    int dificuldade;
} Pergunta;

#endif