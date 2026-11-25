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
#define CORTEXTO WHITE
#define CORCMOUSE (Color){200, 30, 120, 255} //Cor das alternativas com mouse (roxo/rosa estilo neon)
#define CORSMOUSE (Color){70, 20, 120, 255}   //Cor das alternativas sem mouse

//Cores neon dos botões
#define NEON_ROSA    (Color){255, 46, 151, 255}
#define NEON_CIANO   (Color){0, 229, 255, 255}
#define NEON_LARANJA (Color){255, 150, 30, 255}

#define ALTTEXTO 32 //Altura do texto (tamanho da fonte, a default)

typedef struct{
    int posx;
    int posy;
    const char *caracteres;
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
    int fimDeJogo;// 0 - ganha , 1 - perde, 2 - desiste, 3 - acabaram as perguntas do nivel
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

//Fundo, música e botão de mudo
void CarregarRecursos(void);

void AtualizarMusica(void);

void DesenhaFundo(void);

void LiberarRecursos(void);

//Personagens (universitários) e efeitos sonoros
#define QTD_PERSONAGENS 6

#define SOM_ACERTO 0
#define SOM_KILLSTREAK 1
#define SOM_VITORIA 2

const char *NomePersonagem(int indice);

void DesenhaPersonagem(int indice, Rectangle destino);

void TocarSom(int qual);

Botao CriarBotao(int x, int y, int largura, int altura, const char *texto);

//Fonte do jogo (assets/fonte.ttf). Se o arquivo não existir, usa a fonte padrão do raylib
void DesenhaTexto(const char *texto, int x, int y, int tam, Color cor);

int MedeTexto(const char *texto, int tam);

//Daqui pra baixo, todo DrawText/MeasureText do jogo passa a usar a fonte nova, sem mudar os outros arquivos
#define DrawText(texto, x, y, tam, cor) DesenhaTexto(texto, x, y, tam, cor)
#define MeasureText(texto, tam) MedeTexto(texto, tam)

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

void OpcJogarModo(Pergunta *perguntas, int qtde, Partida *partida);

void WinLose(Partida partida);

void ReiniciarJogatina(void);

#endif