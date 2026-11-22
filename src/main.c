#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "raylib.h"
#include "arquivo.h"
#include "cadastro.h"
#include "jogo.h"
#include "interface.h"
#include "limpabuffer.h"

#define LARGURA 1000
#define ALTURA 600
#define NOME "Jogo do Milhão"

typedef enum{SAIR,JOGAR,CADASTRAR,LISTAR,PESQUISAR,ALTERAR,EXCLUIR}Opcoes;

int main(void){
    Pergunta *pergunta = NULL;
    int qtdetotal = 0;
    
    //Raylib:
    

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

    InitWindow(LARGURA, ALTURA, NOME);
    SetTargetFPS(60);

    while (!WindowShouldClose() && telaAtual != -1)    // Detecta o botão de fechar a janela ou a tecla ESC
    {
        mouse = GetMousePosition();

        switch(telaAtual){
            case 0:
                Menu();
                break;

            case 1:
                OpcJogarModo();
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
    
    CloseWindow();

    salvaCSV(pergunta, qtdetotal);

    if(escreveBinario(pergunta, qtdetotal) == 0){//Recria BIN
        printf("BIN atualizado com sucesso!\n");
    }else{
        printf("Erro ao atualizar BIN!\n");
    }

    free(pergunta);

    return 0;
}