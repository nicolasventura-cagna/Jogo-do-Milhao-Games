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
#define ALTBACK 50 //Outback XDDDDDDDDDDDDDDDDD

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

//int pagina = 0;//Dá problema no LOOP se dentro da função
float scroll = 0;//Rolagem da tela

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
        telaAtual = 11;
    }

    //LISTAR
    if(DesenhaBotao(listar, mouse)){
        telaAtual = 12;
    }

    //PESQUISAR --- dá pra colocar na mesma tela será?
    if(DesenhaBotao(pesquisar, mouse)){
        telaAtual = 13;
    }

    //ALTERAR
    if(DesenhaBotao(alterar, mouse)){
        telaAtual = 14;
    }

    //EXCLUIR
    if(DesenhaBotao(excluir, mouse)){
        telaAtual = 15;
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
    /* FALTA ARRUMAR:
        - Quebra de linha de enunciados/alternativas mt grandes
        - Talvez espaçamento
    */

    Rectangle areaPergunta ={50, 80, COMPTELA - 100, ALTTELA - 160};//Área das perguntas

    int espacamentoPerg = 20;
    int alturaPerg = 220;
    int alturatotal = qtde * (espacamentoPerg + alturaPerg); //Altura da lista
    float limitescroll = alturatotal - areaPergunta.height;

    if(limitescroll < 0){
        limitescroll = 0;
    }

    Botao back = CriarBotao(POSBACK,POSBACK, LARGBACK,ALTBACK, "<");

    BeginDrawing();

    ClearBackground(CORFUNDO);

    if(DesenhaBotao(back, mouse)){
        telaAtual = 10;//Retorna pra opções de perguntas
        scroll = 0;
    }

    DrawText("Lista de Perguntas", (COMPTELA - MeasureText("Lista de Perguntas", ALTTEXTO))/2, 25, ALTTEXTO, CORTEXTO);

    if(qtde == 0){
        DrawText("Nenhuma pergunta cadastrada", (COMPTELA - MeasureText("Nenhuma pergunta cadastrada", ALTTEXTO))/2, 50, ALTTEXTO, CORTEXTO);
    }

    if(CheckCollisionPointRec(mouse, areaPergunta)){
        scroll += GetMouseWheelMove() * 100;//"100" é meio q a velocidade q a lista sobe ou desce, bota um num grande p testar se quiser
    }

    if(scroll > 0){//impede a lista de descer além do começo
        scroll = 0;
    }

    if(scroll < -limitescroll){//impede a lista de subir além do final
        scroll = -limitescroll;
    }

    BeginScissorMode(areaPergunta.x, areaPergunta.y, areaPergunta.width, areaPergunta.height);

    for(int i=0; i<qtde; i++){
        int posy = areaPergunta.y + (alturaPerg + espacamentoPerg) * i + scroll;

        ExibePergunta(pergunta[i], i + 1, areaPergunta.x, posy);

    }

    EndScissorMode();

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

void ExibePergunta(Pergunta pergunta, int numero, int x, int y){

    // Título
    DrawText(TextFormat("Pergunta %d:", numero),
        x + 10, y + 20, ALTTEXTO, CORTEXTO);

    // Enunciado
    DrawText(pergunta.enunciado, x + 10, y + 50, ALTTEXTO, CORTEXTO);

    // Alternativas
    for(int i = 0; i < 4; i++){
        DrawText(TextFormat("%c) %s", 'A' + i, pergunta.alternativa[i]),
            x + 10, y + 90 + (30 * i), ALTTEXTO, CORTEXTO);
    }

    // Correta e dificuldade
    DrawText(TextFormat("Correta: %c | Dificuldade: %d",
        pergunta.correta, pergunta.dificuldade),
        x + 10, y + 210, ALTTEXTO, CORTEXTO);

}