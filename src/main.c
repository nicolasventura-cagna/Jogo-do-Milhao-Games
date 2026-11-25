#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "raylib.h"
#include "arquivo.h"
#include "cadastro.h"
#include "jogo.h"
#include "interface.h"
#include "minigames.h"
#include "limpabuffer.h"

#define LARGURA 1280
#define ALTURA 720
#define NOME "Jogo do Milhão"

typedef enum{SAIR,JOGAR,CADASTRAR,LISTAR,PESQUISAR,ALTERAR,EXCLUIR}Opcoes;

int main(void){
    Partida partida = {0};
    Pergunta *pergunta = NULL;
    int qtdetotal = 0;

    srand(time(NULL));//gera uma seed aleatoria

    // Se os dados.bin não existirem - passa para a importação dos perguntas.csv
    //DETALHE: Se a pessoa for adicionar direto no .csv, vai dar ruim XDDDDDDDDDDD
    if(lerBinario(&pergunta, &qtdetotal) == 1){

        // Se não existir BIN, importa do CSV
        if(lerCSV(&pergunta, &qtdetotal) == 0){
            printf("Perguntas importadas do CSV com sucesso!\n");

            escreveBinario(pergunta, qtdetotal);
        }else{
            pergunta = NULL;
            qtdetotal = 0;
        }
    }

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);//necessario pro botao Maximizar
    InitWindow(LARGURA, ALTURA, NOME);
    InitAudioDevice();      
    CarregarRecursos();     
    SetTargetFPS(60);

    while (!WindowShouldClose() && telaAtual != -1)    // Detecta o botão de fechar a janela ou a tecla ESC
    {
        mouse = GetMousePosition();

        /* Pra alguém mais paciente que eu (G):
            - Imagem de fundo
            - Sons/Músicas
        */
        AtualizarMusica();

        switch(telaAtual){
            case 0:
                Menu();
                break;

            case 1:
                OpcJogarModo(pergunta, qtdetotal, &partida);
                break;

            case 2:
                JogoPerg(pergunta, qtdetotal, &partida);
                break;

            case 3:
                Minigame();
                break;


            case 9:
                WinLose(partida);
                break;

            case 10:
                OpcPerguntas();
                break;

            case 11:
                OpcInserir(&pergunta, &qtdetotal);
                break;

            case 12:
                PerguntaListar(pergunta, qtdetotal);
                break;
                
            case 13:
                PerguntaPesquisar(pergunta, qtdetotal);
                break;

            case 14:
                PerguntaAlterar(pergunta, qtdetotal);
                break;
            case 15:
                PerguntaExcluir(&pergunta, &qtdetotal);
                break;

            case 20:
                OpcRanking();
                break;
        }

    }

    LiberarRecursos();
    CloseAudioDevice();
    CloseWindow();

    salvaCSV(pergunta, qtdetotal);

    if(escreveBinario(pergunta, qtdetotal) == 0){//Recria BIN
        printf("BIN atualizado com sucesso!\n");
    }else{
        printf("Erro ao atualizar BIN!\n");
    }

    liberarPartida(&partida);//limpa as perguntas usadas, se fechou no meio da partida

    free(pergunta);

    return 0;
}