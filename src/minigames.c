#include <stdio.h>
#include <stdlib.h>
#include "minigames.h"
#include "interface.h"

EstadoMinigame minigame = {0};

//Guarda o resultado e volta pra tela das perguntas (a jogatina aplica a ajuda)
static void Resultado(int vitoria){
    minigame.vitoria = vitoria;
    minigame.pendente = 1;
    telaAtual = 2;
}

/**
 * @brief 2 minigames, parimpar e jokempo
 * telaAtual = 3
 */
void Minigame(void){
    int posx = (COMPTELA / 2);
    int tecla;
    int res;
    const char *nomes[3] = {"Pedra", "Papel", "Tesoura"};

    Botao par = CriarBotao(posx - ESPACAMENTO - 65, 100, 125, 50, "PAR");

    Botao impar = CriarBotao(posx + ESPACAMENTO, 100, 125, 50, "ÍMPAR");

    Botao continuar = CriarBotao(posx - 100, ALTTELA - 100, 200, 50, "Continuar");

    Botao novamente = CriarBotao(posx - 125, ALTTELA - 100, 250, 50, "Jogar de novo");

    Botao jok[3];
    for(int i = 0; i < 3; i++){
        jok[i] = CriarBotao(posx - 260 + i * 180, 100, 160, 50, nomes[i]);
    }

    BeginDrawing();

    DesenhaFundo();

    //PAR OU ÍMPAR
    if(minigame.escolha == 1){
        if(minigame.parimp == '\0'){
            DrawText("Par ou Ímpar?", posx - (MeasureText("Par ou Ímpar", ALTTEXTO))/2, 50, ALTTEXTO, CORTEXTO);

            if(DesenhaBotao(par, mouse)){
                minigame.parimp = 'P';
                minigame.opcao = 1;
            }
            if(DesenhaBotao(impar, mouse)){
                minigame.parimp = 'I';
                minigame.opcao = 1;
            }
        }

        if(minigame.numusu == -1 && minigame.opcao == 1){
            DrawText("Escolha um número de zero (0) a nove (9):", 50, 150, ALTTEXTO, CORTEXTO);
            while((tecla = GetCharPressed()) > 0){
                if(tecla >= '0' && tecla <= '9'){
                    minigame.numusu = tecla - '0';
                    minigame.rangenumero = 1;
                }
            }
        }

        if(minigame.rangenumero == 1 && minigame.maquina == 0){
            minigame.nummaq = rand() % 10;//Número da máquina vai de 0 a 9

            minigame.soma = minigame.nummaq + minigame.numusu;

            minigame.maquina = 1;
        }

        if(minigame.rangenumero == 1 && minigame.maquina == 1){

            DrawText(TextFormat("Numero da maquina: %d | Seu numero: %d | Soma: %d | Sua escolha: %s",
                                minigame.nummaq, minigame.numusu, minigame.soma, minigame.parimp == 'P' ? "Par" : "Ímpar"),
                                50, 200, 28, CORTEXTO);

            if((minigame.parimp == 'P' && minigame.soma % 2 == 0) || (minigame.parimp == 'I' && minigame.soma % 2 != 0)){
                DrawText("VOCÊ GANHOU, conseguiu uma ajuda!", 50, 300, ALTTEXTO, CORTEXTO);
                if(DesenhaBotao(continuar, mouse)){
                    Resultado(1);
                }
            }else{
                DrawText("VOCÊ PERDEU, sem ajuda!", 50, 300, ALTTEXTO, CORTEXTO);
                if(DesenhaBotao(continuar, mouse)){
                    Resultado(0);
                }
            }
        }
    }

    //JOKEMPO
    if(minigame.escolha == 2){
        if(minigame.jokusu == -1){
            DrawText("Jokempo! Escolha:", posx - (MeasureText("Jokempo! Escolha:", ALTTEXTO))/2, 50, ALTTEXTO, CORTEXTO);

            for(int i = 0; i < 3; i++){
                if(DesenhaBotao(jok[i], mouse)){
                    minigame.jokusu = i;
                    minigame.jokmaq = rand() % 3;
                }
            }
        }else{
            //0 = empate, 1 = jogador ganha, 2 = máquina ganha
            res = (minigame.jokusu - minigame.jokmaq + 3) % 3;

            DrawText(TextFormat("Você: %s | Máquina: %s", nomes[minigame.jokusu], nomes[minigame.jokmaq]),
                                50, 200, 28, CORTEXTO);

            if(res == 0){
                DrawText("EMPATE! Jogue de novo.", 50, 300, ALTTEXTO, CORTEXTO);
                if(DesenhaBotao(novamente, mouse)){
                    minigame.jokusu = -1;
                }
            }else if(res == 1){
                DrawText("VOCÊ GANHOU, conseguiu uma ajuda!", 50, 300, ALTTEXTO, CORTEXTO);
                if(DesenhaBotao(continuar, mouse)){
                    Resultado(1);
                }
            }else{
                DrawText("VOCÊ PERDEU, sem ajuda!", 50, 300, ALTTEXTO, CORTEXTO);
                if(DesenhaBotao(continuar, mouse)){
                    Resultado(0);
                }
            }
        }
    }

    EndDrawing();
}

/**
 * @brief Prepara um minigame novo. Limpa os dados da jogada anterior,
 * mas mantém quais já foram usados
 * @param qual 1 = par/impar, 2 = jokempo
 */
void IniciarMinigame(int qual){
    int jaPI = minigame.parimpar;
    int jaJK = minigame.jokempo;

    minigame = (EstadoMinigame){0};

    minigame.parimpar = jaPI;
    minigame.jokempo = jaJK;

    minigame.escolha = qual;
    minigame.numusu = -1;
    minigame.jokusu = -1;
    minigame.parimp = '\0';

    //marca como usado já na entrada, não dá pra entrar 2x
    if(qual == 1){
        minigame.parimpar = 1;
    }else{
        minigame.jokempo = 1;
    }
}

void ReiniciarMinigame(void){
    minigame = (EstadoMinigame){0};

    minigame.numusu = -1;
    minigame.jokusu = -1;
    minigame.parimp = '\0';
}