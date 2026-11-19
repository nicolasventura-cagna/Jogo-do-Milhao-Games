#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "minigames.h"
#include "jogo.h"


//NÃO FINALIZADO
//Interface Gráfica

int ParImpar(int *modo){
    //srand(time(NULL)); -- Já tem na main

    int c;
    int numusu, nummaq;
    int soma;
    int vezes = 1;
    int vitorias = 0, derrotas = 0; //pra modos com +1 vez
    char parimp;

    printf("Jogo do Par ou Impar!\n");
    if(modo == 1 || modo == 2){
        printf("Melhor de tres\n");
        vezes = 3;
    }

    for(int i=0; i<vezes && vitorias < 2 && derrotas < 2; i++){
        do{
            printf("Par ou impar? Digite 'P' ou 'I': ");
            scanf(" %c", &parimp);

            parimp = toupper(parimp);

        }while (parimp != 'P' && parimp != 'I');

        do {
            printf("Indique o valor que vai jogar, de zero a dez: ");

            if (scanf("%d", &numusu) != 1) {
                printf("Entrada invalida! Digite um numero.\n");

                limparBuffer();//Para loop qnd acha enter

                numusu = -1;//continua loop se scanf inválido
            }

        }while (numusu < 0 || numusu > 10);

        nummaq = rand() % 11;//Número da máquina vai de 0 a 10

        soma = nummaq + numusu;

        printf("Numero da maquina: %d \nSeu numero: %d \n\n  Soma: %d\n", nummaq,numusu,soma);

        if((parimp == 'P' && soma % 2 == 0) || (parimp == 'I' && soma % 2 != 0)){
            printf("VOCE GANHOU, parabens!\n\n");
            vitorias++;
        }else{
            printf("VOCE PERDEU, sem ajuda para você!\n\n");
            derrotas++;
        }
    }

    if(vitorias > derrotas){
        printf("Parabens, voce conquistou sua ajuda!\n");
        return 0;//mostra q usuario ganhou
    }else{
        printf("Infelizmente, voce nao sera ajudado, mais sorte na proxima!\n");
        return 1;//mostra q usuario perdeu
    }
}

int Jokempo(){

}
