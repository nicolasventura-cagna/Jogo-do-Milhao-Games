#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jogo.h"
#include "minigames.h"
#include "interface.h"
#include "cadastro.h"

#define LARGBLOCO 1000 //largura do bloco central do jogo

//Estado do menu de ajuda (os minigames ficam em minigame, em minigames.c)
typedef struct{
    int aberta;      //painel de ajuda aberto
    int usouUniv;    //1 = ja usou universitarios nessa partida
    int usouRemover; //1 = ja usou remover pergunta nessa partida
    char msg[100];   //aviso da ultima ajuda
}EstadoAjuda;

static EstadoJogo estado = {0};
static EstadoAjuda ajuda = {0};

int jaForam(Partida *p, int indice);//está em jogo.c

//Devolve o nivel que nao tem perguntas suficientes (0 = tudo certo)
static int NivelFaltando(Pergunta *perguntas, int qtde){
    int cont;

    for(int nivel = 1; nivel <= 5; nivel++){
        cont = 0;

        for(int i = 0; i < qtde; i++){
            if(perguntas[i].dificuldade == nivel){
                cont++;
            }
        }

        if(cont < PERGUNTAS_POR_NIVEL){
            return nivel;
        }
    }

    return 0;
}

//Começa a partida se tiver perguntas suficientes, senão escreve o aviso em msg
static void ComecarJogo(Pergunta *perguntas, int qtde, Partida *partida, int modo, char *msg){
    int faltando = NivelFaltando(perguntas, qtde);

    if(faltando != 0){
        sprintf(msg, "Faltam perguntas do nivel %d (minimo %d)", faltando, PERGUNTAS_POR_NIVEL);
        return;
    }

    msg[0] = '\0';
    ReiniciarJogatina();//zera estado, ajudas e minigames da partida anterior
    liberarPartida(partida);//limpa a partida anterior (free(NULL) é seguro)
    iniciarPartida(partida, modo);
    telaAtual = 2;
}

//Botão do painel de ajuda: se já foi usado fica cinza e não clica
static int BotaoAjuda(Botao botao, int usada){
    if(!usada){
        return DesenhaBotao(botao, mouse);
    }

    DrawRectangleRounded(botao.retangulo, 0.3f, 8, DARKGRAY);
    DrawText(botao.palavra.caracteres, botao.palavra.posx, botao.palavra.posy, ALTTEXTO, LIGHTGRAY);

    return 0;
}

//Dá pra trocar a pergunta? Precisa sobrar uma pra troca + as das rodadas que faltam no nivel
static int PodeRemoverPergunta(Partida *partida, Pergunta *perguntas, int qtde, int nivel){
    int livres = 0;
    int rodadasFaltando = 2 - ((partida->rodada - 1) % 3);//rodadas do nivel depois dessa

    for(int i = 0; i < qtde; i++){
        if(perguntas[i].dificuldade == nivel && jaForam(partida, i) == 0){
            livres++;
        }
    }

    return livres >= 1 + rodadasFaltando;
}

//3 universitários dão palpite, quanto mais dificil o nivel mais eles erram
static void ConsultarUniversitarios(Partida *partida, Pergunta pergunta, int nivel){
    char op[3];
    int chance = 90 - nivel * 10;//nivel 1 = 80%, nivel 5 = 40%
    int cand[4];
    int total;

    for(int k = 0; k < 3; k++){
        if(rand() % 100 < chance){
            op[k] = pergunta.correta;
        }else{
            total = 0;
            for(int i = 0; i < 4; i++){
                if('A' + i != pergunta.correta && partida->altRemovida[i] == 0){
                    cand[total] = i;
                    total++;
                }
            }
            op[k] = (total > 0) ? 'A' + cand[rand() % total] : pergunta.correta;
        }
    }

    sprintf(ajuda.msg, "Universitarios: %c, %c e %c", op[0], op[1], op[2]);
}

/**
 * @brief Usuário clicou em Jogar, agora escolhe o modo
 * telaAtual = 1
 */
void OpcJogarModo(Pergunta *perguntas, int qtde, Partida *partida){
    static char msg[80] = "";//precisa sobreviver entre os frames

    int posx = (COMPTELA / 2) - (LARGRET / 2);
    int posy = (ALTTELA / 2) - ALTRET;

    Botao back = CriarBotao(POSBACK, POSBACK, MEDBACK, MEDBACK, "<");

    Botao facil = CriarBotao(posx, posy, LARGRET, ALTRET, "Fácil");

    Botao medio = CriarBotao(posx, posy + (ESPACAMENTO), LARGRET, ALTRET, "Médio");

    Botao dificil = CriarBotao(posx, posy + (2 * ESPACAMENTO), LARGRET, ALTRET, "Difícil");

    BeginDrawing();

    DesenhaFundo();

    DrawText("ESCOLHA O MODO", (COMPTELA - MeasureText("ESCOLHA O MODO", 50))/2, posy - 110, 50, CORTEXTO);

    //BACK
    if(DesenhaBotao(back, mouse)){
        msg[0] = '\0';
        telaAtual = 0;
    }

    //FÁCIL
    if(DesenhaBotao(facil, mouse)){
        ComecarJogo(perguntas, qtde, partida, 0, msg);
    }

    //MÉDIO
    if(DesenhaBotao(medio, mouse)){
        ComecarJogo(perguntas, qtde, partida, 1, msg);
    }

    //DIFÍCIL
    if(DesenhaBotao(dificil, mouse)){
        ComecarJogo(perguntas, qtde, partida, 2, msg);
    }

    DrawText(msg, (COMPTELA - MeasureText(msg, 24))/2, posy + (3 * ESPACAMENTO), 24, RED);

    EndDrawing();
}

/**
 * @brief Tela do jogo todo, basicamente, mostra as perguntas
 * telaAtual = 2
 */
void JogoPerg(Pergunta *perguntas, int qtde, Partida *partida){
    int mx = (COMPTELA - LARGBLOCO) / 2;//margem esquerda do bloco central
    int posy = (ALTTELA / 2) - 100;     //onde começam as alternativas
    int largAlt = (LARGBLOCO - 20) / 2; //largura de cada alternativa (2 colunas)
    int yres = posy + 245;              //onde aparece o resultado
    char status[120];

    int nivel;
    int idx; //indice pergunta

    Botao back = CriarBotao(POSBACK, POSBACK, MEDBACK, MEDBACK, "<");

    Botao continuar = CriarBotao(COMPTELA - mx - LARGRET, ALTTELA - 90, LARGRET, ALTRET, "Continuar");

    //Ajuda: embaixo, depois da zona do Parar (SIM/NAO ocupam de mx até mx + 220)
    Botao ajudaBtn = CriarBotao(mx + 250, ALTTELA - 90, 150, ALTRET, "Ajuda");

    BeginDrawing();

    DesenhaFundo();

    if(estado.fimDeJogo != 2){//Se usuário n desistiu, continua

        if(partida->rodada > TOTAL_RODADAS){
            estado.fimDeJogo = 0; // Vitória
            telaAtual = 9;//tela winlose
            EndDrawing();
            return;
        }

        //USUÁRIO CLICA EM BACK (fecha a ajuda pra nunca ficar uma em cima da outra)
        if(DesenhaBotao(back, mouse)){
            estado.saida = 1;
            ajuda.aberta = 0;
        }

        //USUÁRIO DECIDE SE QUER SAIR
        if(estado.saida == 1){
            Botao sim = CriarBotao(mx, ALTTELA - 90, 100, ALTRET, "SIM");
            Botao nao = CriarBotao(mx + 120, ALTTELA - 90, 100, ALTRET, "NAO");

            DrawText("Parar agora leva so metade do dinheiro. Deseja parar?", mx, ALTTELA - 130, 24, CORTEXTO);

            if(DesenhaBotao(sim, mouse)){
                estado.fimDeJogo = 2;//Desistiu
                partida->cash = partida->cash / 2;//desistir leva so metade
                telaAtual = 9;//vai pra tela de fim
            }
            if(DesenhaBotao(nao, mouse)){
                estado.saida = 0;
            }
        }

        //USUÁRIO CLICA EM AJUDA (só antes de responder) - abre/fecha o painel
        if(estado.respostaProcessada == 0 && DesenhaBotao(ajudaBtn, mouse)){
            ajuda.aberta = !ajuda.aberta;
            estado.saida = 0;
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

            //premio da killstreak anterior: tira uma alternativa
            //(tem que ser depois do sorteio, que zera as removidas)
            if(estado.idxPergunta != -1 && estado.premioPendente == 1){
                removerAlternativaErrada(partida, perguntas[estado.idxPergunta]);
            }
            estado.premioPendente = -1;
        }

        idx = estado.idxPergunta;
        if(idx == -1){
            estado.fimDeJogo = 3;//acabaram as perguntas do nivel
            telaAtual = 9;

            EndDrawing();
            return;
        }

        nivel = (partida->rodada - 1) / 3 + 1;

        //VOLTOU DE UM MINIGAME: aplica o resultado (só uma vez)
        if(minigame.pendente == 1){
            minigame.pendente = 0;

            if(minigame.vitoria == 1){
                removerAlternativaErrada(partida, perguntas[idx]);
                strcpy(ajuda.msg, "Ganhou! Uma alternativa foi eliminada.");
            }else{
                strcpy(ajuda.msg, "Perdeu no minigame. Sem ajuda.");
            }
        }

        //Barra de status, alinhada com o bloco
        DrawText(TextFormat("Rodada %d/%d | Nivel %d", partida->rodada, TOTAL_RODADAS, nivel), mx, 25, 24, CORTEXTO);

        sprintf(status, "Vidas: %d | R$ %d | Acertos seguidos: %d", partida->vidas, partida->cash, partida->acertosSeg);
        DrawText(status, mx + LARGBLOCO - MeasureText(status, 24), 25, 24, CORTEXTO);

        if(partida->bonus > 0){
            DrawText(TextFormat("+%.1fx nessa pergunta!", partida->bonus), mx, 55, 24, YELLOW);
        }

        //Enunciado
        QuebraTexto(perguntas[idx].enunciado, mx, 100, LARGBLOCO, 1);

        //Alternativas 2x2, com quebra de linha dentro da caixa
        for(int i = 0; i < 4; i++){
            Rectangle ret = {mx + (largAlt + 20) * (i % 2), posy + 120 * (i / 2), largAlt, 100};
            int removida = partida->altRemovida[i];
            int clicavel = !removida && estado.respostaProcessada == 0 && estado.saida == 0 && !ajuda.aberta;
            int emcima = clicavel && CheckCollisionPointRec(mouse, ret);
            Color cor = CORSMOUSE;
            char alt[150];

            if(removida){
                cor = DARKGRAY;
            }else if(emcima){
                cor = CORCMOUSE;
            }

            //depois de responder: certa em verde, errada escolhida em vermelho
            if(estado.respostaProcessada == 1){
                if('A' + i == perguntas[idx].correta){
                    cor = DARKGREEN;
                }else if('A' + i == estado.respostaFeita){
                    cor = MAROON;
                }
            }

            DrawRectangleRounded(ret, 0.15f, 8, cor);

            if(!removida){
                sprintf(alt, "%c) %s", 'A' + i, perguntas[idx].alternativa[i]);
                QuebraTexto(alt, ret.x + 15, ret.y + 10, ret.width - 30, 1);
            }

            if(emcima && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                estado.respostaFeita = 'A' + i;
                estado.respondeu = 1;
            }
        }

        if(estado.respondeu == 1 && estado.respostaProcessada == 0){

            estado.acertou = conferirResposta(
                partida, perguntas[idx], estado.respostaFeita);

            estado.respostaProcessada = 1;

            if(estado.acertou){
                estado.premioPendente = aplicarKillstreak(partida);
            }else{
                estado.premioPendente = -1;
            }
        }

        if(estado.respostaProcessada == 1){
            if(estado.acertou){
                DrawText("ACERTOU!", mx, yres, 32, GREEN);

                if(estado.premioPendente == 0){
                    DrawText("KILLSTREAK! Ganhou R$ 1000 de bonus!", mx, yres + 45, 24, CORTEXTO);
                }else if(estado.premioPendente == 1){
                    DrawText("KILLSTREAK! Uma alternativa sera eliminada na proxima pergunta.", mx, yres + 45, 24, CORTEXTO);
                }else if(estado.premioPendente == 2){
                    DrawText("KILLSTREAK! A proxima pergunta vale +0.5x de dinheiro.", mx, yres + 45, 24, CORTEXTO);
                }else if(estado.premioPendente == 3){
                    DrawText("KILLSTREAK! Ganhou uma vida extra!", mx, yres + 45, 24, CORTEXTO);
                }
            }else{
                DrawText("ERROU!", mx, yres, 32, RED);

                DrawText(TextFormat("Resposta correta: %c",
                    perguntas[idx].correta),
                    mx, yres + 45, 24, CORTEXTO);
            }
        }

        //Aviso da última ajuda, ao lado do botão
        if(ajuda.msg[0] != '\0' && estado.saida == 0){
            DrawText(ajuda.msg, mx + 250, ALTTELA - 125, 24, CORTEXTO);
        }

        //PAINEL DE AJUDA (desenhado por último pra ficar por cima das alternativas)
        if(ajuda.aberta == 1 && estado.respostaProcessada == 0){
            int px = (COMPTELA - 380) / 2;
            int py = 130;

            Rectangle painel = {px, py, 380, 400};

            Botao jok = CriarBotao(px + 20, py + 65, 340, ALTRET, "Jokempo");
            Botao parimp = CriarBotao(px + 20, py + 130, 340, ALTRET, "Par ou Ímpar");
            Botao univ = CriarBotao(px + 20, py + 195, 340, ALTRET, "Universitários");
            Botao remover = CriarBotao(px + 20, py + 260, 340, ALTRET, "Remover pergunta");
            Botao fechar = CriarBotao(px + 20, py + 325, 340, ALTRET, "Fechar");

            DrawRectangleRounded(painel, 0.05f, 8, Fade(BLACK, 0.8f));
            DrawText("AJUDAS", px + (380 - MeasureText("AJUDAS", ALTTEXTO))/2, py + 15, ALTTEXTO, CORTEXTO);

            //JOKEMPO
            if(BotaoAjuda(jok, minigame.jokempo)){
                IniciarMinigame(2);
                ajuda.aberta = 0;
                telaAtual = 3;
            }

            //PAR OU ÍMPAR
            if(BotaoAjuda(parimp, minigame.parimpar)){
                IniciarMinigame(1);
                ajuda.aberta = 0;
                telaAtual = 3;
            }

            //UNIVERSITÁRIOS
            if(BotaoAjuda(univ, ajuda.usouUniv)){
                ConsultarUniversitarios(partida, perguntas[idx], nivel);
                ajuda.usouUniv = 1;
                ajuda.aberta = 0;
            }

            //REMOVER PERGUNTA (sorteia outra do mesmo nivel, a rodada não muda)
            if(BotaoAjuda(remover, ajuda.usouRemover)){
                if(PodeRemoverPergunta(partida, perguntas, qtde, nivel)){
                    ajuda.usouRemover = 1;
                    estado.idxPergunta = -1;
                    strcpy(ajuda.msg, "Pergunta trocada!");
                }else{
                    strcpy(ajuda.msg, "Sem perguntas sobrando nesse nivel.");
                }
                ajuda.aberta = 0;
            }

            //FECHAR
            if(DesenhaBotao(fechar, mouse)){
                ajuda.aberta = 0;
            }
        }

        if(estado.respostaProcessada == 1){
            if(DesenhaBotao(continuar, mouse)){
                ajuda.msg[0] = '\0';

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
    char *titulo = "FIM DE JOGO";
    char *texto;
    char valor[40];
    int meio = ALTTELA / 2;

    Botao menu = CriarBotao((COMPTELA - LARGRET) / 2, ALTTELA - 120, LARGRET, ALTRET, "Menu");

    BeginDrawing();
    
    DesenhaFundo();

    if(estado.fimDeJogo == 0){
        titulo = "VITORIA!";
        texto = "Voce terminou todas as rodadas com:";
    }else if(estado.fimDeJogo == 1){
        texto = "Acabaram as vidas. Voce terminou com:";
    }else if(estado.fimDeJogo == 2){
        texto = "Voce parou e levou metade do dinheiro:";
    }else{
        texto = "Acabaram as perguntas do nivel. Voce terminou com:";
    }

    sprintf(valor, "R$ %d", partida.cash);

    DrawText(titulo, (COMPTELA - MeasureText(titulo, 60))/2, meio - 180, 60, CORTEXTO);
    DrawText(texto, (COMPTELA - MeasureText(texto, ALTTEXTO))/2, meio - 60, ALTTEXTO, CORTEXTO);
    DrawText(valor, (COMPTELA - MeasureText(valor, 70))/2, meio, 70, GREEN);

    //Aqui depois entra o ranking (pedir o nome e salvar partida.cash)

    if(DesenhaBotao(menu, mouse)){
        telaAtual = 0;
    }

    EndDrawing();
}

/**
 * @brief Reinicia jogatina (chamado a cada partida nova)
 * 
 */
void ReiniciarJogatina(void){
    estado = (EstadoJogo){0};
    ajuda = (EstadoAjuda){0};
    ReiniciarMinigame();

    estado.idxPergunta = -1;
    estado.premioPendente = -1;
}