#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "interface.h"
#include "cadastro.h"
#include "arquivo.h"
#include "minigames.h"

#define ALTTELA GetScreenHeight()
#define COMPTELA GetScreenWidth()

#define LARGRET 300 //Largura Retângulo
#define ALTRET 50 //Altura Retângulo
#define ESPACAMENTO 75

#define POSBACK 25
#define LARGBACK 50
#define ALTBACK 50

//Mudar cores, p ficar + agradável
#define CORFUNDO GRAY //Mudar pra imagem dps
#define CORTEXTO BLACK
#define CORCMOUSE DARKBLUE //Cor botão com mouse
#define CORSMOUSE BLUE     //Cor botão sem mouse

#define ALTTEXTO 32 //Altura do texto (tamanho da fonte, a default)

#define PORPAG 3

/*
-1 - SAIR
0  - Tela Inicial
1  - Jogar -> Escolher Modo
2  - Jogar -> Perguntas
3  - Jogar -> Minigames
4  - Jogar -> Ajuda
...
10 - Perguntas -> Opções
11 - Perguntas -> Opções -> Inserir
12 - Perguntas -> Opções -> Listar

 FAZER EM ORDEM DO MENU
 Ex.: Primeira opção (Jogar) -> Modo -> Pergunta -> Ajuda/Minigame -> WinLose
 e só dps vai pra Perguntas -> .....

 Pularei números para não gerar confusão, por exemplo: Jogar 1-9, Perguntas 10-19... -- Talvez mude o range dps
*/
int telaAtual = 0;//Muriel não gosta de variável global, mas é pra n dar erro

Vector2 mouse; //Coloquei aqui pq utilizei em quase tudo, Muriel n deve reclamar, deve?
//Qlqr coisa mudem ai

int pagina = 0;//Dá problema no LOOP se dentro da função

//Dps da escolha de modo, tem q separar todas as coisas? pq em cada modo a vida é dif, fora dps de ganhar vida com bonus...



//Enrico, para de colocar "// for (ou seja la oq)". Grata

/**
 * @brief Tela de Menu
 * 
 */
void Menu(){
    int posx = (COMPTELA / 2) - (LARGRET / 2);
    int posy = (ALTTELA / 2) - ALTRET;

    Botao jogar = CriarBotao(posx, posy, LARGRET, ALTRET, "Jogar");

    Botao perguntas = CriarBotao(posx, posy + ESPACAMENTO, LARGRET, ALTRET, "Perguntas");

    Botao ranking = CriarBotao(posx, posy + 2 * ESPACAMENTO, LARGRET, ALTRET, "Ranking");

    Botao sair = CriarBotao(posx, posy + 3 * ESPACAMENTO, LARGRET, ALTRET, "Sair");

    BeginDrawing();

    ClearBackground(CORFUNDO);
    
    //JOGAR
    if(DesenhaBotao(jogar, mouse)){
        telaAtual = 1;
    }


    //PERGUNTAS
    if(DesenhaBotao(perguntas, mouse)){
        telaAtual = 10;
    }

    //RANKING
    if(DesenhaBotao(ranking, mouse)){
        telaAtual = 20;
    }

    //SAIR
    if(DesenhaBotao(sair, mouse)){
        telaAtual = -1;
    }

    EndDrawing();
}

/**
 * @brief Usuário clicou em Jogar, agora escolhe o modo
 * telaAtual = 1
 */
void OpcJogarModo(){    
    int posx = (COMPTELA / 2) - (LARGRET / 2);
    int posy = (ALTTELA / 2) - ALTRET;

    Botao back = CriarBotao(POSBACK, POSBACK, LARGBACK, ALTBACK, "<");

    Botao facil = CriarBotao(posx, posy, LARGRET, ALTRET, "Fácil");

    Botao medio = CriarBotao(posx, posy + (ESPACAMENTO), LARGRET, ALTRET, "Médio");

    Botao dificil = CriarBotao(posx, posy + (2 * ESPACAMENTO), LARGRET, ALTRET, "Difícil");

    BeginDrawing();

    ClearBackground(CORFUNDO);

    //Desenhando Retângulos

    //BACK
    if(DesenhaBotao(back, mouse)){
        telaAtual = 0;
    }

    //FÁCIL
    if(DesenhaBotao(facil, mouse)){
        //.............
    }

    //MÉDIO
    if(DesenhaBotao(medio, mouse)){
        //.............
    }

    //DIFÍCIL
    if(DesenhaBotao(dificil, mouse)){
        //.............
    }

    EndDrawing();
}

//void JogarPergunta();

void Minigames();

void WinLose();

/**
 * @brief Usuário Seleciona, no menu, Perguntas
 * telaAtual = 10 (NUMERO TEMPORARIO)
 */
void OpcPerguntas(){
    int posx = (COMPTELA / 2) - (LARGRET / 2);
    int posy = (ALTTELA / 2) - ALTRET; //Deixar menor pq tem mais opções
    
    Botao back = CriarBotao(POSBACK, POSBACK, LARGBACK, ALTBACK, "<");

    Botao inserir = CriarBotao(posx, posy, LARGRET, ALTRET, "Inserir"); 

    Botao listar = CriarBotao(posx, posy + (ESPACAMENTO), LARGRET, ALTRET, "Listar"); 

    Botao pesquisar = CriarBotao(posx, posy + (2 * ESPACAMENTO), LARGRET, ALTRET, "Pesquisar"); 

    Botao alterar = CriarBotao(posx, posy + (3 * ESPACAMENTO), LARGRET, ALTRET, "Alterar"); 

    Botao excluir = CriarBotao(posx, posy + (4 * ESPACAMENTO), LARGRET, ALTRET, "Excluir"); 

    BeginDrawing();

    ClearBackground(CORFUNDO);

    //Desenhando Retângulos

    //BACK
    if(DesenhaBotao(back, mouse)){
        telaAtual = 0;
    }

    //INSERIR
    if(DesenhaBotao(inserir, mouse)){

    }

    //LISTAR
    if(DesenhaBotao(listar, mouse)){
        telaAtual = 12;
    }

    //PESQUISAR --- dá pra colocar na mesma tela será?
    if(DesenhaBotao(pesquisar, mouse)){

    }

    //ALTERAR
    if(DesenhaBotao(alterar, mouse)){

    }

    //EXCLUIR
    if(DesenhaBotao(excluir, mouse)){

    }

    EndDrawing();
}

/**
 * @brief Usuário clicou em Ranking, no menu
 * 
 */
void OpcRanking(){
    Botao back = CriarBotao(POSBACK, POSBACK, LARGBACK, ALTBACK, "<");

    BeginDrawing();

    ClearBackground(CORFUNDO);

    //BACK
    if(DesenhaBotao(back, mouse)){
        telaAtual = 0;
    }

    DrawText("Ranking", 100, 100, 50, RED);

    EndDrawing();
}

//Agora vem o rombo
/**
 * @brief Lista as perguntas cadastradas
 * telaAtual = 12
 * @param pergunta 
 * @param qtde 
 */
void PerguntaListar(Pergunta *pergunta, int qtde){
    //Diminui tamanho da fonte
    int iniciopag = pagina * PORPAG;
    int fimpag = iniciopag + PORPAG;
    int posy = 70;
    /*Ex das variaveis iniciopag e fimpag:
        Pagina = 0, iniciopag = 0, fimpag = 3 ---> Vai mostrar pergunta de n 0, 1 e 2
    */

    if(fimpag > qtde){
        fimpag = qtde;
    }

    Botao back = CriarBotao(POSBACK, POSBACK, LARGBACK, ALTBACK, "<");

    BeginDrawing();

    ClearBackground(CORFUNDO);

    if(DesenhaBotao(back, mouse)){
        telaAtual = 10;//Volta nas opções de perguntas
    }

    DrawText("Lista de Perguntas", 300, 30, 30, CORTEXTO);

    if(qtde == 0){
        DrawText("Nenhuma pergunta cadastrada", 300, posy, 30, CORTEXTO);
    }

    //Fui testando valores pra ver qual ficava melhor
    //Falta mostra a terceira pergunta
    //Não coloquei com rolagem pq tinha q usar um bocado de func dif, fiquei com preguiça
    for(int i = iniciopag; i<fimpag; i++){

        DrawText(TextFormat("Pergunta %d", i+1), 30, posy, 24, CORTEXTO);
        
        DrawText(pergunta[i].enunciado, 30, posy + 50, 24, CORTEXTO);

        for(int alternativas = 0; alternativas < 4; alternativas++){
            DrawText(TextFormat("%c) %s", 'A' + alternativas, pergunta[i].alternativa[alternativas]),
                    50, posy + 100 + (ESPACAMENTO * alternativas/2), 24, CORTEXTO);
        }

        DrawText(TextFormat("Correta: %c | Dificuldade: %d", pergunta[i].correta, pergunta[i].dificuldade),
                300, posy + (3 * ESPACAMENTO), 24, CORTEXTO);

        posy = posy + 50 + (ESPACAMENTO * PORPAG);
    }

    //Troca Página
    if(IsKeyPressed(KEY_RIGHT) && (pagina + 1) * PORPAG < qtde){
        pagina++;
    }
    if(IsKeyPressed(KEY_LEFT) && pagina > 0){
        pagina--;
    }

    EndDrawing();
}

/**
 * @brief Desenha Botão
 * Facilitar e deixar clean o código
 * @param botao 
 * @param mouse 
 * @return int 
 */
int DesenhaBotao(Botao botao, Vector2 mouse){
    //Como tudo ta dentro de um loop, verifica constantemente
    botao.cor = CheckCollisionPointRec(mouse, botao.retangulo) ? CORCMOUSE : CORSMOUSE;//Operador Terciário (? acho q é esse o nome), if de 2 opções

    DrawRectangleRec(botao.retangulo, botao.cor);

    DrawText(botao.palavra.caracteres,
            botao.palavra.posx,
            botao.palavra.posy,
            ALTTEXTO, CORTEXTO);

    if(CheckCollisionPointRec(mouse, botao.retangulo)){
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            return 1;//if dá true e muda tela
        }
    }

    return 0;
}

/**
 * @brief Cria Botão
 * Usada para facilitar e não ficar tão grande 
 * @param x
 * @param y 
 * @param texto 
 * @return Botao 
 */
Botao CriarBotao(int x, int y, int largura, int altura, char *texto){
    Botao botao;

    botao.retangulo = (Rectangle){
        x, y, largura, altura
    };

    botao.palavra.caracteres = texto;

    botao.palavra.posx = x + (largura - MeasureText(texto, ALTTEXTO))/2;

    botao.palavra.posy = y + (altura - ALTTEXTO)/2;

    botao.cor = CORSMOUSE;

    return botao;
}