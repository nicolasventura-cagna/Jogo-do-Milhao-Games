#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arquivo.h"
#include "cadastro.h"


//include guard é só pra header :

//int pra saber se deu certo ou n

/**
 * @brief Lê arquivo binário
 * 
 * @param perguntas  Ponteiro para vetor que possui todas perguntas
 * @param qtde Ponteiro para quantidade de perguntas cadastradas
 * @return int Sucesso (0) na inserção ou Falha (1)
 */
int lerBinario(Pergunta **pergunta, int *qtde){
    long tamanho;
    FILE *arq = fopen("dados.bin", "rb");
    if(arq == NULL){
        return 1;
    }// if

    fseek(arq, 0, SEEK_END);//Vai pro final do arquivo

    tamanho = ftell(arq);//Diz tamanho do arquivo

    (*qtde) = tamanho / sizeof(Pergunta);//Diz quantas perguntas há. Não conta cabeçalho, se houver.

    if (*qtde == 0) {
        *pergunta = NULL;
        fclose(arq);
        return 0;
    }// if

    fseek(arq, 0, SEEK_SET);//Volta ao início

    (*pergunta) = malloc((*qtde) * sizeof(Pergunta));
    if((*pergunta)==NULL){
        fclose(arq);
        return 1;
    }// if

    fread(*pergunta, sizeof(Pergunta), (*qtde), arq);

    fclose(arq);
    
    return 0;
}// lerBinario
/**
 * @brief 
 * 
 * @param perguntas  Vetor que possui todas perguntas
 * @param qtde Quantidade de perguntas cadastradas
 * @return int Sucesso (0) na inserção ou Falha (1)
 */
int escreveBinario(Pergunta *pergunta, int qtde){
    FILE *arq = fopen("dados.bin", "wb");//Mudar wb? nao
    if(arq == NULL){
        return 1;
    }

    fwrite(pergunta, sizeof(Pergunta), qtde, arq);

    fclose(arq);

    return 0;
}// escreveBinario

/**
 * @brief Lê CSV ............ sla
 * 
 * @param perguntas  Vetor que possui todas perguntas
 * @param qtde Quantidade de perguntas cadastradas
 * @return int Sucesso (0) na inserção ou Falha (1)
 */
int lerCSV(Pergunta **pergunta, int *qtde){
    int resultado;
    char linha[500];
    int encontrou = 0;
    int i = 0;
    Pergunta *temp;
    FILE *arq = fopen("./perguntas.csv", "r");

    if(arq == NULL){
        printf("Erro na abertura do arquivo./n");
        return 1;
    }// if

    if(fgets(linha, sizeof(linha), arq) == NULL){
        printf("Arquivo vazio.\n");
        fclose(arq);
        return 1;
    }// if

    while(linha[i] != '\0'){
        if(linha[i] == '\n'){
            encontrou = 1;
            break;
        }// if
        i++;
    }// while

    if(encontrou == 0){
        printf("Encontrou zerou.\n");
        fclose(arq);
        return 1;
    }// if


    while(1){//loop infinito

        temp = realloc(*pergunta, (*qtde +1) * sizeof(Pergunta));
        if(temp == NULL){
            printf("Erro na abertura do arquivo.\n");
            fclose(arq);
            return 1;
        }// if

        *pergunta = temp;

        resultado = fscanf(arq, " %200[^,],%100[^,],%100[^,],%100[^,],%100[^,], %c,%d",(*pergunta)[*qtde].enunciado, (*pergunta)[*qtde].alternativa[0],
        (*pergunta)[*qtde].alternativa[1], (*pergunta)[*qtde].alternativa[2], (*pergunta)[*qtde].alternativa[3], &(*pergunta)[*qtde].correta,
        &(*pergunta)[*qtde].dificuldade);
        //fscanf que não come espaço, quebra nas vírgulas

        if(resultado == EOF){
            break;
        }// if

        if(resultado != 7){
            fclose(arq);            
            return 1;
        }// if

        (*qtde)++;
    } // while infinito

    fclose(arq);

    return 0;
}// lerCSV

/**
 * @brief Salva as perguntas
 * 
 * @param perguntas  Vetor que possui todas perguntas
 * @param qtde Quantidade de perguntas cadastradas
 * @return int Sucesso (0) na inserção ou Falha (1)
 */
int salvaCSV(Pergunta *pergunta, int qtd){
    FILE *arq = fopen("perguntas.csv","w");
    if(arq == NULL){
        return 1;
    }// if
    // manda pro começo do arquivo
    fprintf(arq,"Enunciado,Alternativa1,Alternativa2,Alternativa3,Alternativa4,Correta,Dificuldade\n");

    for (int i = 0; i < qtd; i++)
    {
        if(fprintf(arq,"%s,%s,%s,%s,%s,%c,%d\n",pergunta[i].enunciado,
                pergunta[i].alternativa[0],
                pergunta[i].alternativa[1],
                pergunta[i].alternativa[2],
                pergunta[i].alternativa[3],
                pergunta[i].correta,
                pergunta[i].dificuldade) < 0){
            fclose(arq);
            return 1;
        }// if
    }// for
    fclose(arq);
    return 0;
}// exportaCSV
