#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
#include "raylib.h"
#include "interface.h"
#include "cadastro.h"
#include "arquivo.h"
#include "minigames.h"

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

CaixaTexto caixaEnun;  //muriel não gosta de variavel global? ... Não recomenda o uso
CaixaTexto caixaAlt[4];
char corretaSel = 0;
int difSel = 0;
char mensagem[60] = "";

//Fundo e música
static Texture2D fundo;//arte completa (só no menu)
static Texture2D fundoJogo;//versão desfocada e escura (todas as outras telas)
static Music musica;
static int mudo = 0;//1 = música mutada

//Enrico, para de colocar "// for (ou seja la oq)". Grata
//bom saber que não sou o unico que não gosta.

/**
 * @brief Carrega a imagem de fundo e a música. Chamar depois de InitWindow e InitAudioDevice
 * Se algum arquivo não existir, o jogo roda normal, só sem ele
 */
void CarregarRecursos(){
    fundo = LoadTexture("assets/fundo.png");
    fundoJogo = LoadTexture("assets/fundo_jogo.png");

    musica = LoadMusicStream("assets/musica.ogg");

    if(IsMusicValid(musica)){
        musica.looping = true;
        SetMusicVolume(musica, 0.5f);
        PlayMusicStream(musica);
    }
}

/**
 * @brief Alimenta a música. Tem que ser chamada todo frame, senão ela engasga e para
 * 
 */
void AtualizarMusica(){
    if(IsMusicValid(musica)){
        UpdateMusicStream(musica);
    }
}

/**
 * @brief Botão de mudo, canto superior direito (espelho do back)
 * Chamado dentro do DesenhaFundo, então aparece em todas as telas
 */
static void BotaoMudo(){
    Rectangle ret = {COMPTELA - POSBACK - MEDBACK, POSBACK, MEDBACK, MEDBACK};
    int emcima = CheckCollisionPointRec(mouse, ret);
    int x = (int)ret.x;
    int y = (int)ret.y;

    if(emcima && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        mudo = !mudo;
        SetMasterVolume(mudo ? 0.0f : 1.0f);
    }

    DrawRectangleRounded(ret, 0.3f, 8, emcima ? CORCMOUSE : CORSMOUSE);

    //alto-falante
    DrawRectangle(x + 10, y + 19, 8, 12, WHITE);
    DrawTriangle((Vector2){x + 18, y + 19}, (Vector2){x + 18, y + 31}, (Vector2){x + 28, y + 38}, WHITE);
    DrawTriangle((Vector2){x + 18, y + 19}, (Vector2){x + 28, y + 38}, (Vector2){x + 28, y + 12}, WHITE);

    if(mudo){
        //X vermelho = mutado
        DrawLineEx((Vector2){x + 32, y + 18}, (Vector2){x + 44, y + 32}, 3, RED);
        DrawLineEx((Vector2){x + 44, y + 18}, (Vector2){x + 32, y + 32}, 3, RED);
    }else{
        //ondinhas = tocando
        DrawRing((Vector2){x + 28, y + 25}, 9, 11, -45, 45, 16, WHITE);
        DrawRing((Vector2){x + 28, y + 25}, 15, 17, -45, 45, 16, WHITE);
    }
}

/**
 * @brief Desenha o fundo (no lugar do ClearBackground) e o botão de mudo
 * Menu = arte completa, resto das telas = versão desfocada
 * Chamar logo depois do BeginDrawing em toda tela
 */
void DesenhaFundo(){
    Texture2D atual = fundo;//menu (telaAtual 0) usa a arte completa

    if(telaAtual != 0 && IsTextureValid(fundoJogo)){
        atual = fundoJogo;//qualquer outra tela usa a versão desfocada
    }

    if(IsTextureValid(atual)){
        DrawTexturePro(atual,
            (Rectangle){0, 0, atual.width, atual.height},
            (Rectangle){0, 0, COMPTELA, ALTTELA},
            (Vector2){0, 0}, 0.0f, WHITE);
    }else{
        ClearBackground(CORFUNDO);//sem imagem, usa a cor de antes
    }

    BotaoMudo();
}

/**
 * @brief Libera imagem e música. Chamar antes de CloseAudioDevice e CloseWindow
 * 
 */
void LiberarRecursos(){
    if(IsTextureValid(fundo)){
        UnloadTexture(fundo);
    }

    if(IsTextureValid(fundoJogo)){
        UnloadTexture(fundoJogo);
    }

    if(IsMusicValid(musica)){
        UnloadMusicStream(musica);
    }
}

/**
 * @brief Tela de Menu
 * 
 */
void Menu(){
    int posx = (COMPTELA / 2) - (LARGRET / 2);
    int posy = (ALTTELA / 2) - ALTRET + 65;//+65 pra ficar abaixo do logo da arte de fundo

    Botao jogar = CriarBotao(posx, posy, LARGRET, ALTRET, "Jogar");

    Botao perguntas = CriarBotao(posx, posy + ESPACAMENTO, LARGRET, ALTRET, "Perguntas");

    Botao ranking = CriarBotao(posx, posy + 2 * ESPACAMENTO, LARGRET, ALTRET, "Ranking");

    Botao sair = CriarBotao(posx, posy + 3 * ESPACAMENTO, LARGRET, ALTRET, "Sair");

    //Maximizar: canto superior direito, ao lado do botao de mudo
    Botao maximiza = CriarBotao(COMPTELA - POSBACK - MEDBACK - 15 - 190, POSBACK, 190, MEDBACK,
                                IsWindowMaximized() ? "Restaurar" : "Maximizar");

    BeginDrawing();

    DesenhaFundo();

    //MAXIMIZAR / RESTAURAR (sem tela cheia)
    if(DesenhaBotao(maximiza, mouse)){
        if(IsWindowMaximized()){
            RestoreWindow();
        }else{
            MaximizeWindow();
        }
    }

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
 * @brief Usuário Seleciona, no menu, Perguntas
 * telaAtual = 10 (NUMERO TEMPORARIO)
 */
void OpcPerguntas(){
    int posx = (COMPTELA / 2) - (LARGRET / 2);
    int posy = (ALTTELA / 2) - ALTRET; //Deixar menor pq tem mais opções
    
    Botao back = CriarBotao(POSBACK, POSBACK, MEDBACK, MEDBACK, "<");

    Botao inserir = CriarBotao(posx, posy, LARGRET, ALTRET, "Inserir"); 

    Botao listar = CriarBotao(posx, posy + (ESPACAMENTO), LARGRET, ALTRET, "Listar"); 

    Botao pesquisar = CriarBotao(posx, posy + (2 * ESPACAMENTO), LARGRET, ALTRET, "Pesquisar"); 

    Botao alterar = CriarBotao(posx, posy + (3 * ESPACAMENTO), LARGRET, ALTRET, "Alterar"); 

    Botao excluir = CriarBotao(posx, posy + (4 * ESPACAMENTO), LARGRET, ALTRET, "Excluir"); 

    BeginDrawing();

    DesenhaFundo();

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
    Botao back = CriarBotao(POSBACK, POSBACK, MEDBACK, MEDBACK, "<");

    BeginDrawing();

    DesenhaFundo();

    //BACK
    if(DesenhaBotao(back, mouse)){
        telaAtual = 0;
    }

    DrawText("Ranking", 100, 100, 50, RED);

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

    DrawRectangleRounded(botao.retangulo, 0.3f, 8, botao.cor);

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
Botao CriarBotao(int x, int y, int largura, int altura, const char *texto){
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

/**
 * @brief Desenha Caixa
 * 
 * @param caixa 
 * @param mouse 
 */
void DesenhaCaixa(CaixaTexto *caixa, Vector2 mouse){
    int tecla;
    int inicio = 0;

    static float tempoBackspace = 0;
    static float intervaloBackspace = 0.15f;

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

        if(caixa->ativa){
            if(IsKeyPressed(KEY_BACKSPACE)){
                //Apaga imediatamente ao pressionar
                if(caixa->tamanho > 0){
                    caixa->tamanho--;
                    caixa->texto[caixa->tamanho] = '\0';
                }

                tempoBackspace = 0;
            }else if(IsKeyDown(KEY_BACKSPACE)){
                //Continua apagando enquanto segura
                tempoBackspace += GetFrameTime();

                if(tempoBackspace >= intervaloBackspace){
                    if(caixa->tamanho > 0){
                        caixa->tamanho--;
                        caixa->texto[caixa->tamanho] = '\0';
                    }

                    tempoBackspace = 0;
                }
            }else{
                tempoBackspace = 0;
            }
        }
    }

    DrawRectangleRec(caixa->retangulo, caixa->ativa ? LIGHTGRAY : WHITE);
    DrawRectangleLinesEx(caixa->retangulo, 2, caixa->ativa ? DARKBLUE : BLACK);

    //texto maior que a caixa, mostra so o final
    while(MeasureText(caixa->texto + inicio, 20) > caixa->retangulo.width - 15){
        inicio++;
    }

    DrawText(caixa->texto + inicio, caixa->retangulo.x + 5, 
    caixa->retangulo.y + (caixa->retangulo.height - 20)/2, 20, BLACK);
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
    if(desenhar && telaAtual != 2){//Mudei ak
        DrawText(TextFormat("Correta: %c | Dificuldade: %d", pergunta.correta, pergunta.dificuldade),
            x + 10, posy, TAMPERG, CORTEXTO);
    }
    posy += ESPLINHA;

    return posy - y;
}