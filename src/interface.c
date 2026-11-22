#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
#include "raylib.h"
#include "interface.h"
#include "cadastro.h"
#include "arquivo.h"
#include "minigames.h"


#define TAMPERG 22    // fonte menor 
#define ESPLINHA 28   // distância entre linhas
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

CaixaTexto caixaEnun;  //muriel não gosta de variavel global? 
CaixaTexto caixaAlt[4];
char corretaSel = 0;
int difSel = 0;
char mensagem[60] = "";

//Enrico, para de colocar "// for (ou seja la oq)". Grata
//bom saber que não sou o unico que não gosta.

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
        IniciarTelaInserir();
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
        - Talvez espaçamento
    */

    Rectangle areaPergunta ={50, 80, COMPTELA - 100, ALTTELA - 160};//Área das perguntas

    int espacamentoPerg = 20;
    int alturatotal = 0;//Altura da lista

    for(int i = 0; i < qtde; i++){//só mede, não desenha
        alturatotal += ExibePergunta(pergunta[i], i + 1, 0, 0, areaPergunta.width, 0) + espacamentoPerg;
    }

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

    int posy = areaPergunta.y + scroll;//começa no topo da área, já com a rolagem

    for(int i=0; i<qtde; i++){
        posy += ExibePergunta(pergunta[i], i + 1, areaPergunta.x, posy, areaPergunta.width, 1) + espacamentoPerg;
    }

    EndScissorMode();

    EndDrawing();
}

/**
 * @brief Tela de excluir pergunta
 * telaAtual = 15
 */
void PerguntaExcluir(Pergunta **pergunta, int *qtde){
    static int confirmar = -1;//Esperando o Sim/Não, o -1 = nenhuma

    Rectangle areaPergunta = {50, 80, COMPTELA - 100, ALTTELA - 160};

    int espacamentoPerg = 20;
    int alturatotal = 0;
    int posy;
    int altura;

    for(int i = 0; i < (*qtde); i++){//só mede
        alturatotal += ExibePergunta((*pergunta)[i], i + 1, 0, 0, areaPergunta.width, 0) + espacamentoPerg;
    }

    float limitescroll = alturatotal - areaPergunta.height;

    if(limitescroll < 0){
        limitescroll = 0;
    }

    Botao back = CriarBotao(POSBACK, POSBACK, LARGBACK, ALTBACK, "<");

    BeginDrawing();

    ClearBackground(CORFUNDO);

    if(DesenhaBotao(back, mouse)){
        telaAtual = 10;
        scroll = 0;
        confirmar = -1;
    }

    DrawText("Excluir Pergunta", (COMPTELA - MeasureText("Excluir Pergunta", ALTTEXTO))/2, 25, ALTTEXTO, CORTEXTO);

    if((*qtde) == 0){
        DrawText("Nenhuma pergunta cadastrada", (COMPTELA - MeasureText("Nenhuma pergunta cadastrada", ALTTEXTO))/2, 50, ALTTEXTO, CORTEXTO);
    }

    if(CheckCollisionPointRec(mouse, areaPergunta)){
        scroll += GetMouseWheelMove() * 100;
    }

    if(scroll > 0){
        scroll = 0;
    }

    if(scroll < -limitescroll){
        scroll = -limitescroll;
    }

    BeginScissorMode(areaPergunta.x, areaPergunta.y, areaPergunta.width, areaPergunta.height);

    posy = areaPergunta.y + scroll;

    for(int i = 0; i < (*qtde); i++){
        altura = ExibePergunta((*pergunta)[i], i + 1, areaPergunta.x, posy, areaPergunta.width, 1);

        //botão desenhado na mão
        Rectangle ret = {areaPergunta.x + areaPergunta.width - 90, posy, 80, 26};
        //so conta o clique se o mouse tá dentro da area
        int em_cima = CheckCollisionPointRec(mouse, ret) && CheckCollisionPointRec(mouse, areaPergunta);

        DrawRectangleRec(ret, em_cima ? MAROON : RED);
        DrawText("Excluir", ret.x + 8, ret.y + 3, 20, WHITE);

        if(em_cima && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            confirmar = i;
        }

        posy += altura + espacamentoPerg;
    }

    EndScissorMode();

    //Barra de confirmação
    if(confirmar != -1){
        Botao sim = CriarBotao(520, ALTTELA - 70, 120, ALTRET, "Sim");
        Botao nao = CriarBotao(660, ALTTELA - 70, 120, ALTRET, "Nao");

        DrawText(TextFormat("Excluir a pergunta %d?", confirmar + 1), 130, ALTTELA - 58, 24, CORTEXTO);

        if(DesenhaBotao(sim, mouse)){
            removerPergunta(pergunta, qtde, confirmar);
            confirmar = -1;
        }

        if(DesenhaBotao(nao, mouse)){
            confirmar = -1;
        }
    }

    EndDrawing();
}

/**
 * @brief Tela de pesquisar pergunta
 * telaAtual = 13
 */
void PerguntaPesquisar(Pergunta *pergunta, int qtde){
    static CaixaTexto caixa;
    static int dif = 0;      //0 = todas as dificuldades
    static int iniciada = 0;

    char *numeros[5] = {"1", "2", "3", "4", "5"};
    Rectangle areaPergunta = {50, 130, COMPTELA - 100, ALTTELA - 190};
    int espacamentoPerg = 20;
    int alturatotal = 0;
    int encontradas = 0;
    int posy;
    int altura;

    if(iniciada == 0){
        IniciarCaixa(&caixa, 100, 75, 500, 40, 100);
        dif = 0;
        iniciada = 1;
    }

    //só mede, não desenha
    for(int i = 0; i < qtde; i++){
        if(strstr(pergunta[i].enunciado, caixa.texto) != NULL && (dif == 0 || pergunta[i].dificuldade == dif)){
            alturatotal += ExibePergunta(pergunta[i], i + 1, 0, 0, areaPergunta.width, 0) + espacamentoPerg;
            encontradas++;
        }
    }

    float limitescroll = alturatotal - areaPergunta.height;

    if(limitescroll < 0){
        limitescroll = 0;
    }

    Botao back = CriarBotao(POSBACK, POSBACK, LARGBACK, ALTBACK, "<");

    Botao todas = CriarBotao(620, 75, 100, 40, "Todas");

    BeginDrawing();

    ClearBackground(CORFUNDO);

    if(DesenhaBotao(back, mouse)){
        telaAtual = 10;
        scroll = 0;
        iniciada = 0;
    }

    DrawText("Pesquisar Pergunta", (COMPTELA - MeasureText("Pesquisar Pergunta", ALTTEXTO))/2, 25, ALTTEXTO, CORTEXTO);

    DesenhaCaixa(&caixa, mouse);

    //filtro de dificuldade
    if(DesenhaBotao(todas, mouse)){
        dif = 0;
    }

    if(dif == 0){
        DrawRectangleLinesEx(todas.retangulo, 4, YELLOW);
    }

    for(int i = 0; i < 5; i++){
        Botao b = CriarBotao(730 + (45 * i), 75, 40, 40, numeros[i]);

        if(DesenhaBotao(b, mouse)){
            dif = i + 1;
        }

        if(dif == i + 1){
            DrawRectangleLinesEx(b.retangulo, 4, YELLOW);
        }
    }

    if(encontradas == 0){
        DrawText("Nenhuma pergunta encontrada", 50, ALTTELA - 50, 24, CORTEXTO);
    }else{
        DrawText(TextFormat("Encontradas: %d", encontradas), 50, ALTTELA - 50, 24, CORTEXTO);
    }

    if(CheckCollisionPointRec(mouse, areaPergunta)){
        scroll += GetMouseWheelMove() * 100;
    }

    if(scroll > 0){
        scroll = 0;
    }

    if(scroll < -limitescroll){
        scroll = -limitescroll;
    }

    BeginScissorMode(areaPergunta.x, areaPergunta.y, areaPergunta.width, areaPergunta.height);

    posy = areaPergunta.y + scroll;

    for(int i = 0; i < qtde; i++){
        if(strstr(pergunta[i].enunciado, caixa.texto) != NULL && (dif == 0 || pergunta[i].dificuldade == dif)){
            //mostra o numero original da pergunta, igual na Listar
            altura = ExibePergunta(pergunta[i], i + 1, areaPergunta.x, posy, areaPergunta.width, 1);
            posy += altura + espacamentoPerg;
        }
    }

    EndScissorMode();

    EndDrawing();
}

/**
 * @brief Tela de alterar pergunta
 * telaAtual = 14
 * Primeiro mostra a lista, ao clicar numa pergunta abre o formulario 
 */
void PerguntaAlterar(Pergunta *pergunta, int qtde){
    static int escolhida = -1;//-1 = ainda na lista
    static CaixaTexto caixaEnun;
    static CaixaTexto caixaAlt[4];
    static char corretaSel = 0;
    static int difSel = 0;
    static char mensagem[60] = "";
    static int iniciada = 0;//0 = precisa preencher o formulario

    Rectangle areaPergunta = {50, 80, COMPTELA - 100, ALTTELA - 160};
    char *letras[4] = {"A", "B", "C", "D"};
    char *numeros[5] = {"1", "2", "3", "4", "5"};
    int espacamentoPerg = 20;
    int alturatotal = 0;
    int vazio = 0;
    int posy;
    int altura;
    Pergunta nova;

    Botao back = CriarBotao(POSBACK, POSBACK, LARGBACK, ALTBACK, "<");

    //Lista
    if(escolhida == -1){
        for(int i = 0; i < qtde; i++){//só mede
            alturatotal += ExibePergunta(pergunta[i], i + 1, 0, 0, areaPergunta.width, 0) + espacamentoPerg;
        }

        float limitescroll = alturatotal - areaPergunta.height;

        if(limitescroll < 0){
            limitescroll = 0;
        }

        BeginDrawing();

        ClearBackground(CORFUNDO);

        if(DesenhaBotao(back, mouse)){
            telaAtual = 10;
            scroll = 0;
            mensagem[0] = '\0';
        }

        DrawText("Alterar Pergunta", (COMPTELA - MeasureText("Alterar Pergunta", ALTTEXTO))/2, 25, ALTTEXTO, CORTEXTO);

        if(qtde == 0){
            DrawText("Nenhuma pergunta cadastrada", (COMPTELA - MeasureText("Nenhuma pergunta cadastrada", ALTTEXTO))/2, 50, ALTTEXTO, CORTEXTO);
        }

        DrawText(mensagem, 50, ALTTELA - 50, 24, DARKGREEN);

        if(CheckCollisionPointRec(mouse, areaPergunta)){
            scroll += GetMouseWheelMove() * 100;
        }

        if(scroll > 0){
            scroll = 0;
        }

        if(scroll < -limitescroll){
            scroll = -limitescroll;
        }

        BeginScissorMode(areaPergunta.x, areaPergunta.y, areaPergunta.width, areaPergunta.height);

        posy = areaPergunta.y + scroll;

        for(int i = 0; i < qtde; i++){
            altura = ExibePergunta(pergunta[i], i + 1, areaPergunta.x, posy, areaPergunta.width, 1);

            Rectangle ret = {areaPergunta.x + areaPergunta.width - 90, posy, 80, 26};
            int em_cima = CheckCollisionPointRec(mouse, ret) && CheckCollisionPointRec(mouse, areaPergunta);

            DrawRectangleRec(ret, em_cima ? DARKGREEN : GREEN);
            DrawText("Alterar", ret.x + 8, ret.y + 3, 20, WHITE);

            if(em_cima && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                escolhida = i;
                iniciada = 0;//próximo frame preenche o formulario
                mensagem[0] = '\0';
            }

            posy += altura + espacamentoPerg;
        }

        EndScissorMode();

        EndDrawing();

        return;
    }

    //Formulario
    if(iniciada == 0){
        IniciarCaixa(&caixaEnun, 50, 90, 900, 40, 200);
        strcpy(caixaEnun.texto, pergunta[escolhida].enunciado);
        caixaEnun.tamanho = strlen(caixaEnun.texto);

        for(int i = 0; i < 4; i++){
            IniciarCaixa(&caixaAlt[i], 90, 150 + (50 * i), 860, 40, 100);
            strcpy(caixaAlt[i].texto, pergunta[escolhida].alternativa[i]);
            caixaAlt[i].tamanho = strlen(caixaAlt[i].texto);
        }

        corretaSel = pergunta[escolhida].correta;
        difSel = pergunta[escolhida].dificuldade;
        iniciada = 1;
    }

    Botao salvar = CriarBotao(50, 520, LARGRET, ALTRET, "Salvar");

    BeginDrawing();

    ClearBackground(CORFUNDO);

    if(DesenhaBotao(back, mouse)){
        escolhida = -1;//volta pra lista
        mensagem[0] = '\0';
    }

    DrawText(TextFormat("Alterar Pergunta %d", escolhida + 1), (COMPTELA - MeasureText("Alterar Pergunta 00", ALTTEXTO))/2, 25, ALTTEXTO, CORTEXTO);

    //Enunciado
    DrawText("Enunciado:", 50, 65, 20, CORTEXTO);
    DesenhaCaixa(&caixaEnun, mouse);

    //Alternativas
    for(int i = 0; i < 4; i++){
        DrawText(TextFormat("%c)", 'A' + i), 55, caixaAlt[i].retangulo.y + 10, 20, CORTEXTO);
        DesenhaCaixa(&caixaAlt[i], mouse);
    }

    //Correta
    DrawText("Alternativa correta:", 50, 345, 20, CORTEXTO);

    for(int i = 0; i < 4; i++){
        Botao b = CriarBotao(50 + (60 * i), 370, 50, 40, letras[i]);

        if(DesenhaBotao(b, mouse)){
            corretaSel = 'A' + i;
        }

        if(corretaSel == 'A' + i){
            DrawRectangleLinesEx(b.retangulo, 4, YELLOW);
        }
    }

    //Dificuldade
    DrawText("Dificuldade (1 = muito facil, 5 = muito dificil):", 50, 420, 20, CORTEXTO);

    for(int i = 0; i < 5; i++){
        Botao b = CriarBotao(50 + (60 * i), 445, 50, 40, numeros[i]);

        if(DesenhaBotao(b, mouse)){
            difSel = i + 1;
        }

        if(difSel == i + 1){
            DrawRectangleLinesEx(b.retangulo, 4, YELLOW);
        }
    }

    //Salvar
    if(DesenhaBotao(salvar, mouse)){
        for(int i = 0; i < 4; i++){
            if(caixaAlt[i].tamanho == 0){
                vazio = 1;
            }
        }

        if(caixaEnun.tamanho == 0 || vazio == 1){
            strcpy(mensagem, "Preencha tudo!");
        }else{
            strcpy(nova.enunciado, caixaEnun.texto);

            for(int i = 0; i < 4; i++){
                strcpy(nova.alternativa[i], caixaAlt[i].texto);
            }

            nova.correta = corretaSel;
            nova.dificuldade = difSel;

            if(alterarPergunta(pergunta, qtde, escolhida, nova) == 0){
                escolhida = -1;//volta pra lista
                strcpy(mensagem, "Pergunta alterada!");
            }else{
                strcpy(mensagem, "Erro ao salvar!");
            }
        }
    }

    DrawText(mensagem, 380, 535, 24, RED);

    EndDrawing();
}

/**
 * @brief Pra iniciar a tela inserir, chama uma vez quando vai entrar na tela.
 * TelaAtual=11
 */
void IniciarTelaInserir(){
    IniciarCaixa(&caixaEnun, 50, 90, 900, 40, 200);

    for(int i = 0; i < 4; i++){
        IniciarCaixa(&caixaAlt[i], 90, 150 + (50 * i), 860, 40, 100);
    }

    corretaSel = 0;
    difSel = 0;
    mensagem[0] = '\0';
}

/**
 * @brief Tela de inserir pergunta
 * 
 */
void OpcInserir(Pergunta **pergunta, int *qtde){
    char *letras[4] = {"A", "B", "C", "D"};
    char *numeros[5] = {"1", "2", "3", "4", "5"};
    int vazio = 0;
    Pergunta nova;

    Botao back = CriarBotao(POSBACK, POSBACK, LARGBACK, ALTBACK, "<");

    Botao salvar = CriarBotao(50, 520, LARGRET, ALTRET, "Salvar");

    BeginDrawing();

    ClearBackground(CORFUNDO);

    if(DesenhaBotao(back, mouse)){
        telaAtual = 10;
    }

    DrawText("Inserir Pergunta", (COMPTELA - MeasureText("Inserir Pergunta", ALTTEXTO))/2, 25, ALTTEXTO, CORTEXTO);

    //Enunciado
    DrawText("Enunciado:", 50, 65, 20, CORTEXTO);
    DesenhaCaixa(&caixaEnun, mouse);

    //Alternativas
    for(int i = 0; i < 4; i++){
        DrawText(TextFormat("%c)", 'A' + i), 55, caixaAlt[i].retangulo.y + 10, 20, CORTEXTO);
        DesenhaCaixa(&caixaAlt[i], mouse);
    }

    //Correta
    DrawText("Alternativa correta:", 50, 345, 20, CORTEXTO);

    for(int i = 0; i < 4; i++){
        Botao b = CriarBotao(50 + (60 * i), 370, 50, 40, letras[i]);

        if(DesenhaBotao(b, mouse)){
            corretaSel = 'A' + i;
        }

        if(corretaSel == 'A' + i){//contorno amarelo no escolhido
            DrawRectangleLinesEx(b.retangulo, 4, YELLOW);
        }
    }

    //Dificuldade
    DrawText("Dificuldade (1 = muito facil, 5 = muito dificil):", 50, 420, 20, CORTEXTO);

    for(int i = 0; i < 5; i++){
        Botao b = CriarBotao(50 + (60 * i), 445, 50, 40, numeros[i]);

        if(DesenhaBotao(b, mouse)){
            difSel = i + 1;
        }

        if(difSel == i + 1){
            DrawRectangleLinesEx(b.retangulo, 4, YELLOW);
        }
    }

    //Salvar
    if(DesenhaBotao(salvar, mouse)){
        for(int i = 0; i < 4; i++){
            if(caixaAlt[i].tamanho == 0){
                vazio = 1;
            }
        }

        if(caixaEnun.tamanho == 0 || vazio == 1 || corretaSel == 0 || difSel == 0){
            strcpy(mensagem, "Preencha tudo!");
        }else{
            strcpy(nova.enunciado, caixaEnun.texto);

            for(int i = 0; i < 4; i++){
                strcpy(nova.alternativa[i], caixaAlt[i].texto);
            }

            nova.correta = corretaSel;
            nova.dificuldade = difSel;

            if(adicionarPergunta(pergunta, qtde, nova) == 0){
                IniciarTelaInserir();//limpa a tela
                strcpy(mensagem, "Pergunta cadastrada!");
            }else{
                strcpy(mensagem, "Erro ao salvar!");
            }
        }
    }

    DrawText(mensagem, 380, 535, 24, RED);

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

/**
 * @brief Prepara a caixa de texto
 * 
 * @param caixa 
 * @param x 
 * @param y 
 * @param largura 
 * @param altura 
 * @param max 
 */
void IniciarCaixa(CaixaTexto *caixa, int x, int y, int largura, int altura, int max){
    caixa->retangulo = (Rectangle){x, y, largura, altura};
    caixa->texto[0] = '\0';
    caixa->tamanho = 0;
    caixa->max = max;
    caixa->ativa = 0;
}

void DesenhaCaixa(CaixaTexto *caixa, Vector2 mouse){
    int tecla;
    int inicio = 0;

    //clicar dentro ativa, clicar fora desativa
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        caixa->ativa = CheckCollisionPointRec(mouse, caixa->retangulo);
    }

    if(caixa->ativa){
        tecla = GetCharPressed();//Pega uma tecla por vez, 0 quando acabar

        while(tecla > 0){
            if(tecla >= 32 && tecla <= 125 && caixa->tamanho < caixa->max){
                caixa->texto[caixa->tamanho] = (char)tecla;
                caixa->tamanho++;
                caixa->texto[caixa->tamanho] = '\0';
            }
            tecla = GetCharPressed();
        }

        if(IsKeyPressed(KEY_BACKSPACE) && caixa->tamanho > 0){
            caixa->tamanho--;
            caixa->texto[caixa->tamanho] = '\0';
        }
    }

    DrawRectangleRec(caixa->retangulo, caixa->ativa ? LIGHTGRAY : WHITE);
    DrawRectangleLinesEx(caixa->retangulo, 2, caixa->ativa ? DARKBLUE : BLACK);

    //texto maior que a caixa, mostra so o final
    while(MeasureText(caixa->texto + inicio, 20) > caixa->retangulo.width - 15){
        inicio++;
    }

    DrawText(caixa->texto + inicio, caixa->retangulo.x + 5, 
    caixa->retangulo.y + (caixa->retangulo.height - 20)/2, 20, CORTEXTO);
}

/**
 * @brief Quebra o texto em linhas pra caber na largura
 * 
 * @return Quantas linhas o texto ocupou
 */
int QuebraTexto(char *texto, int x, int y, int largura, int desenhar){
    char copia[400];
    char linha[400] = "";
    char teste[400];
    char *palavra;
    int linhas = 0;

    strcpy(copia, texto);//strtok estraga a string, então usa uma strcpy

    palavra = strtok(copia, " ");

    while(palavra != NULL){
        //monta a linha com a palavra nova pra ver se cabe
        strcpy(teste, linha);
        if(linha[0] != '\0'){
            strcat(teste, " ");
        }
        strcat(teste, palavra);

        if(MeasureText(teste, TAMPERG) > largura && linha[0] != '\0'){
        //se não couber, desenha o que tinha e começa outra linha
            if(desenhar){
                DrawText(linha, x, y + linhas * ESPLINHA, TAMPERG, CORTEXTO);
            }
            linhas++;
            strcpy(linha, palavra);
        }else{
            strcpy(linha, teste);
        }

        palavra = strtok(NULL, " ");
    }

   //ultima linha
    if(linha[0] != '\0'){
        if(desenhar){
            DrawText(linha, x, y + linhas * ESPLINHA, TAMPERG, CORTEXTO);
        }
        linhas++;
    }

    return linhas;
}

int ExibePergunta(Pergunta pergunta, int numero, int x, int y, int largura, int desenhar){
    int posy = y;
    char alt[150];

    if(desenhar){
        DrawText(TextFormat("Pergunta %d:", numero), x + 10, posy, TAMPERG, CORTEXTO);
    }
    posy += ESPLINHA;

    //Enunciado
    posy += QuebraTexto(pergunta.enunciado, x + 10, posy, largura - 20, desenhar) * ESPLINHA;

    //Alternativas
    for(int i = 0; i < 4; i++){
        sprintf(alt, "%c) %s", 'A' + i, pergunta.alternativa[i]);
        posy += QuebraTexto(alt, x + 10, posy, largura - 20, desenhar) * ESPLINHA;
    }

    //Correta e dificuldade
    if(desenhar){
        DrawText(TextFormat("Correta: %c | Dificuldade: %d", pergunta.correta, pergunta.dificuldade),
            x + 10, posy, TAMPERG, CORTEXTO);
    }
    posy += ESPLINHA;

    return posy - y;
}