#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arquivo.h"
#include "cadastro.h"

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
}// lerBinario

/**
 * @brief 
 * 
 * @param perguntas  Vetor que possui todas perguntas
 * @param qtde Quantidade de perguntas cadastradas
 * @return int Sucesso (0) na inserção ou Falha (1)
 */
int escreveBinario(Pergunta *pergunta, int qtde){
    FILE *arq = fopen("dados.bin", "wb");//Mudar wb?
    if(arq == NULL){
        return 1;
    }

    /*
    if (fwrite(pergunta, sizeof(Pergunta), (size_t)qtde, arq) != (size_t)qtde) {
    fclose(arq);
    return 1;
    Dava Warning, comparacao de tipos dif
    }*/
    if(fwrite(pergunta, sizeof(Pergunta), (size_t)qtde, arq) != (size_t)qtde){
        fclose(arq);
        return 1;
    }

    fclose(arq);

    return 0;
}// escreveBinario

/**
 * @brief Lê CSV 
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

        // \" : mostra aspas dentro da string
        resultado = fscanf(arq, " \"%200[^\"]\", \"%100[^\"]\", \"%100[^\"]\", \"%100[^\"]\", \"%100[^\"]\", %c,%d",(*pergunta)[*qtde].enunciado, (*pergunta)[*qtde].alternativa[0],
        (*pergunta)[*qtde].alternativa[1], (*pergunta)[*qtde].alternativa[2], (*pergunta)[*qtde].alternativa[3], &(*pergunta)[*qtde].correta,
        &(*pergunta)[*qtde].dificuldade);
        //fscanf que não come espaço, quebra nas vírgulas

        if(resultado == EOF){
            break;
        }

        if(resultado != 7){
            printf("Erro na leitura\n");
            fclose(arq);            
            return 1;
        }

        (*qtde)++;
    }//while infinito

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
    }
    // manda pro começo do arquivo
    fprintf(arq,"Enunciado,Alternativa1,Alternativa2,Alternativa3,Alternativa4,Correta,Dificuldade\n");

    for (int i = 0; i < qtd; i++)
    {
        if(fprintf(arq,"\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",%c,%d\n",pergunta[i].enunciado,
            pergunta[i].alternativa[0],
            pergunta[i].alternativa[1],
            pergunta[i].alternativa[2],
            pergunta[i].alternativa[3],
            pergunta[i].correta,
            pergunta[i].dificuldade) < 0){
            fclose(arq);
            return 1;//Retorna 1 se deu ruim
        }
    }
    fclose(arq);
    return 0;
}// exportaCSV

/**
 * @brief Lê o ranking do arquivo (já ordenado do maior pro menor)
 * 
 * @param vet Vetor onde os registros serão colocados
 * @param max Tamanho do vetor
 * @return int Quantidade de registros lidos (0 se o arquivo não existir)
 */
int carregarRanking(Registro *vet, int max){
    size_t lidos;
    FILE *arq = fopen("ranking.bin", "rb");
    if(arq == NULL){
        return 0;
    }

    lidos = fread(vet, sizeof(Registro), (size_t)max, arq);

    fclose(arq);

    return (int)lidos;
}// carregarRanking

/**
 * @brief Coloca o jogador no ranking, na posição certa. Guarda só os RANKING_MAX melhores
 * 
 * @param novo Jogador a inserir
 * @return int Sucesso (0) ou Falha (1)
 */
int salvarRanking(Registro novo){
    Registro vet[RANKING_MAX + 1];
    int qtde = carregarRanking(vet, RANKING_MAX);
    int pos = qtde;//se não for maior que ninguém, vai pro final
    FILE *arq;

    for(int i = 0; i < qtde; i++){
        if(novo.cash > vet[i].cash){
            pos = i;
            break;
        }
    }

    for(int i = qtde; i > pos; i--){//abre espaço
        vet[i] = vet[i-1];
    }

    vet[pos] = novo;
    qtde++;

    if(qtde > RANKING_MAX){//o último cai fora
        qtde = RANKING_MAX;
    }

    arq = fopen("ranking.bin", "wb");
    if(arq == NULL){
        return 1;
    }

    if(fwrite(vet, sizeof(Registro), (size_t)qtde, arq) != (size_t)qtde){
        fclose(arq);
        return 1;
    }

    fclose(arq);

    return 0;
}// salvarRanking