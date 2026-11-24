#include <stdio.h>
#include <stdlib.h>
#include "jogo.h"
#include "minigames.h"
#include "interface.h"
#include "cadastro.h"

static EstadoJogo estado = {0};

/**
 * @brief Usuário clicou em Jogar, agora escolhe o modo
 * telaAtual = 1
 */
void OpcJogarModo(Partida *partida){    
    int posx = (COMPTELA / 2) - (LARGRET / 2);
    int posy = (ALTTELA / 2) - ALTRET;

    Botao back = CriarBotao(POSBACK, POSBACK, MEDBACK, MEDBACK, "<");

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
        ReiniciarJogatina();
        iniciarPartida(partida, 0);
        telaAtual = 2;
    }

    //MÉDIO
    if(DesenhaBotao(medio, mouse)){
        ReiniciarJogatina();
        iniciarPartida(partida, 1);
        telaAtual = 2;
    }

    //DIFÍCIL
    if(DesenhaBotao(dificil, mouse)){
        ReiniciarJogatina();
        iniciarPartida(partida, 2);
        telaAtual = 2;
    }

    EndDrawing();
}

/**
 * @brief Tela do jogo todo, basicamente, mostra as perguntas
 * 
 * @param perguntas 
 * @param qtde 
 * @param modo 
 */
void JogoPerg(Pergunta *perguntas, int qtde, Partida *partida){
    int medeTexto;
    int posicaoxPerg;
    int posx = (COMPTELA / 2) - LARGRET - ESPACAMENTO/2;
    int posy = (ALTTELA / 2) - ALTRET;

    int nivel;
    int idx; //indice pergunta

    Botao back = CriarBotao(POSBACK, POSBACK, MEDBACK, MEDBACK, "<");

    Botao continuar = CriarBotao((COMPTELA - LARGRET) / 2,
        posy + 2 * ESPACAMENTO,
        LARGRET, ALTRET, "Continuar"
    );

    BeginDrawing();

    ClearBackground(CORFUNDO);

    if(estado.fimDeJogo != 2){//Se usuário n desistiu, continua

        if(partida->rodada > TOTAL_RODADAS){
            estado.fimDeJogo = 0; // Vitória
            telaAtual = 9;//tela winlose
            EndDrawing();
            return;
        }

        //USUÁRIO CLICA EM BACK
        if(DesenhaBotao(back, mouse)){
            estado.saida = 1;
        }

        if(estado.saida == 1){
            Botao sim = CriarBotao(50, 100, 50, 50, "SIM");
            Botao nao = CriarBotao(100 + ESPACAMENTO/2, 100, 50, 50, "NÃO");

            DrawText("Realmente deseja parar?", 50, 75, 24, CORTEXTO);

            if(DesenhaBotao(sim, mouse)){
                estado.fimDeJogo = 2;//Desistiu
                //partida->cash
                telaAtual = 0;
            }
            if(DesenhaBotao(nao, mouse)){
                estado.saida = 0;
            }
        }

        //COMEÇO
        if(estado.iniciado == 0){
            estado.iniciado = 1;
            estado.idxPergunta = -1;
        }

        if(estado.idxPergunta == -1){

            nivel = (partida->rodada - 1) / 3 + 1;

            estado.idxPergunta = sortearPergunta(
                partida, perguntas, qtde, nivel);
        }

        idx = estado.idxPergunta;
        if(idx == -1){
            DrawText("Sem perguntas disponíveis!",//diferenciar?
                    50, 50, ALTTEXTO, CORTEXTO);

            EndDrawing();
            return;
        }

        if(temPerguntasSuficientes(perguntas, qtde) == 0){
            DrawText("Teste", 0, 0, 32, BLUE);
        }

        posicaoxPerg = ((COMPTELA/2) - (MeasureText(perguntas[idx].enunciado, ALTTEXTO)/2));

        ExibePergunta(perguntas[idx], idx + 1, posicaoxPerg,//EXIBE COM NUMERO DA PERGUNTA EM LISTAR..... reutilizei a função
                50, 900, 1);

        //Alternativas com botão padrão, não aumenta se mt grande
        Botao A = CriarBotao(posx, posy, LARGRET, ALTRET,
                    TextFormat("A) %s", perguntas[idx].alternativa[0]));

        Botao B = CriarBotao(posx + ESPACAMENTO/2 + LARGRET, posy, LARGRET, ALTRET,
                    TextFormat("B) %s", perguntas[idx].alternativa[1]));

        Botao C = CriarBotao(posx, posy + ESPACAMENTO, LARGRET, ALTRET,
                    TextFormat("C) %s", perguntas[idx].alternativa[2]));

        Botao D = CriarBotao(posx + ESPACAMENTO/2 + LARGRET, posy + ESPACAMENTO, LARGRET, ALTRET,
                    TextFormat("D) %s", perguntas[idx].alternativa[3]));

        if(estado.respostaProcessada == 0){//Evitar loop desnecessário, se ainda n foi processada, recebe
            if(DesenhaBotao(A, mouse)){
                estado.respostaFeita = 'A';
                estado.respondeu = 1;
            }

            if(DesenhaBotao(B, mouse)){
                estado.respostaFeita = 'B';
                estado.respondeu = 1;
            }

            if(DesenhaBotao(C, mouse)){
                estado.respostaFeita = 'C';
                estado.respondeu = 1;
            }

            if(DesenhaBotao(D, mouse)){
                estado.respostaFeita = 'D';
                estado.respondeu = 1;
            }
        }

        if(estado.respondeu == 1 && estado.respostaProcessada == 0){

            estado.acertou = conferirResposta(
                partida, perguntas[idx], estado.respostaFeita);

            estado.respostaProcessada = 1;

            if(estado.acertou){
                estado.premioPendente = aplicarKillstreak(partida);//fzr
            }else{
                estado.premioPendente = -1;
            }
        }

        if(estado.respostaProcessada == 1){
            if(estado.acertou){
                DrawText("ACERTOU!", 50, 450, 32, GREEN);
            }else{
                DrawText("ERROU!", 50, 450, 32, RED);

                DrawText(TextFormat("Resposta correta: %c",
                    perguntas[idx].correta),
                    50, 490, 24, BLACK);
            }
        }

        if(estado.respostaProcessada == 1){
            if(DesenhaBotao(continuar, mouse)){
                if(partida->vidas <= 0){
                    //Manter info pra mandar pra tela winlose
                    estado.fimDeJogo = 1;// 1 perde

                    estado.idxPergunta = -1;
                    estado.respondeu = 0;
                    estado.respostaProcessada = 0;
                    estado.respostaFeita = '\0';
                    telaAtual = 9;
                }else{
                    partida->rodada++;

                    estado.idxPergunta = -1;
                    estado.respondeu = 0;
                    estado.respostaProcessada = 0;
                    estado.respostaFeita = '\0';
                }
            }
        }
    }

    EndDrawing();
}

/**
 * @brief Tela vitória/derrota
 * 
 */
void WinLose(Partida partida){

    BeginDrawing();
    
    ClearBackground(CORFUNDO);

    if(estado.fimDeJogo == 0){
        DrawText("Vitoria---------", (COMPTELA - MeasureText("Vitória---------", 42)), 50, 42, CORTEXTO);
        DrawText(TextFormat("Dinheiro: %d", partida.cash), 50, 50 + ESPACAMENTO, 42, CORTEXTO);
        //...
    }else{
        //....
    }

    EndDrawing();
}

/**
 * @brief Reinicia jogatina
 * 
 */
void ReiniciarJogatina(void){
    estado = (EstadoJogo){0};

    estado.idxPergunta = -1;
    estado.premioPendente = -1;
}