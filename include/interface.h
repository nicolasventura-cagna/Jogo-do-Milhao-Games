#ifndef INTERFACE_H
#define INTERFACE_H

#include "raylib.h"
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

typedef struct{
    Rectangle retangulo;
    char texto[201];
    int tamanho;
    int max;   
    int ativa; 
}CaixaTexto;

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

void IniciarCaixa(CaixaTexto *caixa, int x, int y, int largura, int altura, int max);

void DesenhaCaixa(CaixaTexto *caixa, Vector2 mouse);

void IniciarTelaInserir(void);

void PerguntaExcluir(Pergunta **pergunta, int *qtde);

void OpcInserir(Pergunta **pergunta, int *qtde);

void PerguntaPesquisar(Pergunta *pergunta, int qtde);

void PerguntaAlterar(Pergunta *pergunta, int qtde);

#endif