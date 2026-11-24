#ifndef INTERFACE_H
#define INTERFACE_H

#include "raylib.h"
#include "cadastro.h"
#include "jogo.h"

#define TAMPERG 22    // fonte menor 
#define ESPLINHA 28   // distância entre linhas
#define ALTTELA GetScreenHeight()
#define COMPTELA GetScreenWidth()

#define LARGRET 300 //Largura Retângulo
#define ALTRET 50 //Altura Retângulo
#define ESPACAMENTO 75

#define POSBACK 25
#define MEDBACK 50 //Medida back, tirei a separação pq n acha q vamos mudar...
//Qualquer coisa pra mudar: cria dnv e Control + Shift + L nas palavras q quer mudar, muda todas :)

//Mudar cores, p ficar + agradável
#define CORFUNDO GRAY //Mudar pra imagem dps
#define CORTEXTO BLACK
#define CORCMOUSE DARKBLUE //Cor botão com mouse
#define CORSMOUSE BLUE     //Cor botão sem mouse

#define ALTTEXTO 32 //Altura do texto (tamanho da fonte, a default)

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

typedef struct {
    int iniciado;
    int idxPergunta;
    int respostaFeita;
    int respondeu;
    int respostaProcessada;//evitar bug, e chamar a func infinitamente
    int acertou;
    int saida;//Back
    int fimDeJogo;// 0 - ganha , 1 - perde, 2 - desiste
    int premioPendente;
    char mensagem[100];
}EstadoJogo;

extern int telaAtual;

extern Vector2 mouse;

extern float scroll;

//Colocado aqui pra diferenciar interface.c, q continha tudo, agora é dividida.
extern CaixaTexto caixaEnun;
extern CaixaTexto caixaAlt[4];

extern char corretaSel;
extern int difSel;
extern char mensagem[60];

//INTERFACE.C -- GERAL

void Menu(void);

void OpcPerguntas(void);

void OpcRanking(void);

Botao CriarBotao(int x, int y, int largura, int altura, char *texto);

void DesenhaCaixa(CaixaTexto *caixa, Vector2 mouse);

int DesenhaBotao(Botao botao, Vector2 mouse);

void IniciarCaixa(CaixaTexto *caixa, int x, int y, int largura, int altura, int max);

//INTERFACE.C

//MODPERGUNTAS.C -- MODIFICACAO PERGUNTAS

void IniciarTelaInserir(void);

void PerguntaExcluir(Pergunta **pergunta, int *qtde);

void OpcInserir(Pergunta **pergunta, int *qtde);

void PerguntaListar(Pergunta *pergunta, int qtde);

void PerguntaPesquisar(Pergunta *pergunta, int qtde);

void PerguntaAlterar(Pergunta *pergunta, int qtde);

int QuebraTexto(char *texto, int x, int y, int largura, int desenhar);

int ExibePergunta(Pergunta pergunta, int numero, int x, int y, int largura, int desenhar);

//MODPERGUNTAS.C

//JOGATINA.C -- TELAS DE JOGO

void JogoPerg(Pergunta *perguntas, int qtde, Partida *partida);

void OpcJogarModo(Partida *partida);

void WinLose(Partida partida);

void ReiniciarJogatina(void);

//JOGATINA.C

//A FAZER:

/*
void JogoMinigame();

void JogoAjuda();//diferenciar
*/

//A FAZER

#endif