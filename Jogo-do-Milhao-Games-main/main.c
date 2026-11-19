#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include "raylib.h"
#include "arquivo.h"
#include "cadastro.h"
#include "jogo.h"
#include "limpabuffer.h"

typedef enum{SAIR,JOGAR,CADASTRAR,LISTAR,PESQUISAR,ALTERAR,EXCLUIR}Opcoes;

int main(void){
    Pergunta *pergunta = NULL;
    int qtdetotal = 0;
    int opcao;

    srand(time(NULL));//gera uma seed aleatoria

    // Se os dados.bin não existirem - passa para a importação dos perguntas.csv
    //DETALHE: Se a pessoa for adicionar direto no .csv, vai dar ruim XDDDDDDDDDDD
    if (lerBinario(&pergunta, &qtdetotal) == 1) {

        // Se não existir BIN, importa do CSV
        if(lerCSV(&pergunta, &qtdetotal) == 0){
            printf("Perguntas importadas do CSV com sucesso!\n");

            escreveBinario(pergunta, qtdetotal);
        }else{
            pergunta = NULL;
            qtdetotal = 0;
        }
    }

    do{
        printf("\n>----SHOW DO MILHAO----<\n"
        "1 = Jogar\n"
        "2 = Cadastrar perguntas\n"
        "3 = Listar perguntas\n"
        "4 = Pesquisar perguntas\n"
        "5 = Alterar pergunta\n"
        "6 = Excluir pergunta\n"
        "0 = Sair\n"
        "Opcao: ");

        while (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida! Digite apenas um numero: ");
            limparBuffer(); // Joga a letra fora do buffer
        }

        limparBuffer();

        switch(opcao){
            case JOGAR:// 1
                jogar(pergunta, qtdetotal);
                break;

            case CADASTRAR:// 2
                inserir(&pergunta, &qtdetotal);
                break;

            case LISTAR:// 3
                listar(pergunta, qtdetotal);
                break;

            case PESQUISAR:// 4
                pesquisar(pergunta, qtdetotal);
                break;

            case ALTERAR:// 5
                alterar(pergunta, qtdetotal);
                break;

            case EXCLUIR:// 6
                excluir(&pergunta,&qtdetotal);
                break;

            case SAIR:// 0
                printf("Ate mais!\n");
                break;

            default:// != enum Opcoes
                printf("Opcao invalida.\n");
                break;
        }

    }while(opcao != 0);

    salvaCSV(pergunta, qtdetotal);

    if(escreveBinario(pergunta, qtdetotal) == 0){//Recria BIN
        printf("BIN atualizado com sucesso!\n");
    }else{
        printf("Erro ao atualizar BIN!\n");
    }

    free(pergunta);

    return 0;
}// main