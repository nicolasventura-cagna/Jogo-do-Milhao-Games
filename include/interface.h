#ifndef INTERFACE_H
#define INTERFACE_H

#include "cadastro.h"

typedef struct{
    int posx;
    int posy;
    char *caracteres;
}Texto;

typedef struct{
    Rectangle retangulo;
    Texto palavra;
    Color cor;
}Botao;

extern int telaAtual;

extern Vector2 mouse;

void Menu(void);

void OpcJogarModo(void);

/*
void JogoPerg();

void JogoMinigame();

void JogoAjuda();

void JogarPergunta();
*/

void Minigames(void);

void WinLose(void);

void OpcPerguntas(void);

void OpcRanking(void);

void PerguntaListar(Pergunta *pergunta, int qtde);

int DesenhaBotao(Botao botao, Vector2 mouse);

Botao CriarBotao(int x, int y, int largura, int altura, char *texto);

void ExibePergunta(Pergunta pergunta, int numero, int x, int y);

#endif