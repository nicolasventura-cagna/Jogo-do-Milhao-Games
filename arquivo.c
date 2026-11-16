#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arquivo.h"
#include "cadastro.h"


//FAZER INCLUDE GUARD

//int pra saber se deu certo ou n

/**
 * @brief Lê arquivo binário
 * 
 * @param pergunta 
 * @param qtde 
 * @return int 
 */
int lerBinario(Pergunta **pergunta, int *qtde){
    long tamanho;
    FILE *arq = fopen("dados.bin", "rb");
    if(arq == NULL){
        return 1;
    }

    fseek(arq, 0, SEEK_END);//Vai pro final do arquivo

    tamanho = ftell(arq);//Diz tamanho do arquivo

    (*qtde) = tamanho / sizeof(Pergunta);//Diz quantas perguntas há. Não conta cabeçalho, se houver.

    if (*qtde == 0) {
        *pergunta = NULL;
        fclose(arq);
        return 0;
    }

    fseek(arq, 0, SEEK_SET);//Volta ao início

    (*pergunta) = malloc((*qtde) * sizeof(Pergunta));
    if((*pergunta)==NULL){
        fclose(arq);
        return 1;
    }

    fread(*pergunta, sizeof(Pergunta), (*qtde), arq);

    fclose(arq);
    
    return 0;
}

int escreveBinario(Pergunta *pergunta, int qtde){
    FILE *arq = fopen("dados.bin", "wb");//Mudar wb?
    if(arq == NULL){
        return 1;
    }

    fwrite(pergunta, sizeof(Pergunta), qtde, arq);

    fclose(arq);

    return 0;
}

/**
 * @brief Lê CSV ............ sla
 * 
 * @param pergunta 
 * @param qtde 
 * @return int 
 */
int lerCSV(Pergunta **pergunta, int *qtde){
    int resultado;
    char linha[500];
    int encontrou = 0;
    int i = 0;
    Pergunta *temp;
    FILE *arq = fopen("perguntas.csv", "r");
    if(arq == NULL){
        return 1;
    }

    if(fgets(linha, sizeof(linha), arq) == NULL){
        fclose(arq);
        return 1;
    }

    while(linha[i] != '\0'){
        if(linha[i] == '\n'){
            encontrou = 1;
            break;
        }
        i++;
    }

    if(encontrou == 0){
        fclose(arq);
        return 1;
    }


    while(1){//loop infinito

        temp = realloc(*pergunta, (*qtde +1) * sizeof(Pergunta));
        if(temp == NULL){
            fclose(arq);
            return 1;
        }

        *pergunta = temp;

        resultado = fscanf(arq, "%200[^,],%100[^,],%100[^,],%100[^,],%100[^,], %c,%d",(*pergunta)[*qtde].enunciado, (*pergunta)[*qtde].alternativa[0],
        (*pergunta)[*qtde].alternativa[1], (*pergunta)[*qtde].alternativa[2], (*pergunta)[*qtde].alternativa[3], &(*pergunta)[*qtde].correta,
        &(*pergunta)[*qtde].dificuldade);
        //fscanf que não come espaço, quebra nas vírgulas

        if(resultado == EOF){
            break;
        }

        if(resultado != 7){
            fclose(arq);            
            return 1;
        }

        (*qtde)++;
    } 

    fclose(arq);

    return 0;
}

/**
 * @brief WIP
 * 
 */
int exportaCSV(Pergunta *pergunta, int qtd){

    return 0;//falta fazer


}
