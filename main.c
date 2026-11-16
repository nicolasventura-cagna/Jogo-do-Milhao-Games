#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include "raylib.h"
#include "arquivo.h"
#include "cadastro.h"
#include "jogo.h"

int main(){
    Pergunta *pergunta = NULL;
    int qtdetotal = 0;
    int opcao;

    srand(time(NULL));//gera uma seed aleatoria

    //se o dados.bin não existir, segue vazio
    if(lerBinario(&pergunta, &qtdetotal) == 1){
        pergunta = NULL;
        qtdetotal = 0;
    }

    do{
        printf("\n>----SHOW DO MILHAO----<\n");
        printf("1 = Jogar\n");
        printf("2 = Cadastrar perguntas\n");
        printf("3 = Listar perguntas\n");
        printf("4 = Pesquisar perguntas\n");
        printf("5 = Alterar pergunta\n");
        printf("0 = Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch(opcao){
            case 1:
                jogar(pergunta, qtdetotal);
                break;

            case 2:
                inserir(&pergunta, &qtdetotal);
                break;

            case 3:
                listar(pergunta, qtdetotal);
                break;

            case 4:
                pesquisar(pergunta, qtdetotal);
                break;

            case 5:
                alterar(pergunta, qtdetotal);
                break;

            case 0:
                printf("Ate mais!\n");
                break;

            default:
                printf("Opcao invalida.\n");
                break;
        }
    }while(opcao != 0);

    free(pergunta);

    return 0;
}

//não precisou do include guard, talvez ela ia fazer de outra forma :p