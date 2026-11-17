#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "cadastro.h"
#include "arquivo.h"
#include "limpabuffer.h"

//FAZER INCLUDE GUARD


/**
 * @brief Insere quantas perguntas o usuário desejar
 * 
 * @param perguntas  Ponteiro para vetor que possui todas perguntas
 * @param qtde Ponteiro para quantidade de perguntas cadastradas
 * @return int Sucesso (0) na inserção ou Falha (1)
 */
int inserir(Pergunta **perguntas, int *qtde){
    int novas;//perguntas a adicionar
    int funcionou;
    Pergunta *temp = NULL;//Caso der erro no realloc, não ferra tudo :)

    do{
        printf("Quantas perguntas quer adicionar?: ");
        while (scanf("%d", &novas) != 1) {
        printf("Entrada invalida! Digite apenas um numero: ");
        limparBuffer(); // Joga a letra fora do buffer
        }
        limparBuffer();
        getchar();
    }while(novas <= 0);

    temp = realloc(*perguntas, ((*qtde) + novas) * sizeof(Pergunta));
    if(temp == NULL){
        perror("ERRO");
        exit(1);
    }// if

    *perguntas = temp;

    for(int contador = (*qtde); contador < (*qtde) + novas; contador++){
        printf("Digite o enunciado da pergunta: ");
        fgets((*perguntas)[contador].enunciado, 200, stdin);
        (*perguntas)[contador].enunciado[strcspn((*perguntas)[contador].enunciado, "\n")] = '\0';

        for(int i=0; i<4; i++){
            printf("Digite a %d alternativa: ", i+1);
            fgets((*perguntas)[contador].alternativa[i], 100, stdin);
            (*perguntas)[contador].alternativa[i][strcspn((*perguntas)[contador].alternativa[i], "\n")] = '\0';
        }// for

        //getchar();
        printf("Qual a correta?(A/B/C/D): ");
        scanf(" %c", &(*perguntas)[contador].correta);


        printf("Digite a dificuldade (1(Mt facil) - 5(mt dificil)):");
        while (scanf("%d", &(*perguntas)[contador].dificuldade) != 1) {
        printf("Entrada invalida! Digite apenas um numero: ");
        limparBuffer(); // Joga a letra fora do buffer
        }
        limparBuffer();
    }// for

    (*qtde) += novas;
    
    funcionou = escreveBinario(*perguntas, *qtde);

    if(funcionou == 1){
        perror("Erro, tente novamente");
        return 1;
    }// if

    return 0;
}// inserir

/**
 * @brief Lista perguntas cadastradas
 * 
 * @param perguntas  Ponteiro para vetor que possui todas perguntas
 * @param qtde Quantidade de perguntas cadastradas
 */
void listar(Pergunta *pergunta, int qtde){
    if(qtde == 0){
        printf("Nenhuma pergunta cadastrada.\n");
        return;
    }// if

    for(int i = 0; i<qtde; i++){
        printf("\n");
        printf("Pergunta %d\n",i+1);
        exibirPergunta(pergunta[i]);
    }// for
}// listar

/**
 * @brief Pesquisa perguntas cadastradas
 * 
 * @param perguntas  Ponteiro para vetor que possui todas perguntas
 * @param qtde Quantidade de perguntas cadastradas
 * @return int Sucesso (0) na inserção ou Falha (1)
 */
int pesquisar(Pergunta *pergunta, int qtde){
    char pesquisa[201];
    int dificuldade;
    int opcao;
    int encontrou = 0;

    if(qtde == 0){
        printf("Nenhuma pergunta cadastrada.\n");
        return 0;
    }// if

    printf("Deseja pesquisar por enunciado (1) ou dificuldade (2)?: ");
    while (scanf("%d", &opcao) != 1){
    printf("Entrada invalida! Digite apenas um numero: ");
    limparBuffer(); // Joga a letra fora do buffer
    }
    limparBuffer();

    switch(opcao){
        case 1:
            printf("Digite o enunciado a pesquisar: ");
            fgets(pesquisa, 200, stdin);
            pesquisa[strcspn(pesquisa, "\n")] = '\0';

            // strstr - procura uma substring dentro de uma string.

            for(int i = 0; i < qtde; i++){
                if(strstr(pergunta[i].enunciado, pesquisa) != NULL){
                    exibirPergunta(pergunta[i]);
                    encontrou++;
                }// if
            }// for

            if(encontrou == 0){
                printf("Nenhum enunciado semelhante.\n");
            }// if

            break;

        case 2:
            printf("-Dificuldades-\n"
            "1 - Muito Fácil\n"
            "2 - Fácil\n"
            "3 - Média\n"
            "4 - Difícil\n"
            "5 - Muito Difícil\n"
            "Digite a dificuldade: ");
            while (scanf("%d", &opcao) != 1){
            printf("Entrada invalida! Digite apenas um numero: ");
            limparBuffer(); // Joga a letra fora do buffer
            }
            limparBuffer();
            for(int i=0; i<qtde; i++){
                if(pergunta[i].dificuldade == dificuldade){
                    exibirPergunta(pergunta[i]);
                    encontrou++;
                }// if
            }// for

            if(encontrou == 0){
                printf("Nenhuma pergunta nessa categoria.\n");
            }// if

            break;

        default:
            printf("Opção inválida.\n");
            return 0;
    }// switch


    return 0;
}// pesquisar

typedef enum{ENUNCIADO = 1, ALTERNATIVAS, ALT_CORRETA, DIFICULDADE}Opcoes;
// usado na funcao "alterar" - o debug para de reclamar quando fica fora de uma função :)
/**
 * 
 * @brief Altera pergunta cadastrada
 * 
 * @param perguntas  Ponteiro para vetor que possui todas perguntas
 * @param qtde Quantidade de perguntas cadastradas
 * @return int Sucesso (0) na inserção ou Falha (1)
 */
int alterar(Pergunta *pergunta, int qtde){
    int indice;//Qual pergunta a alterar
    int escolha;//O que alterar
    char letra;//Caso alteração de ALTERNATIVAS
    int intletra;//Letra dps de transformada indice.... Letra - (seu valor em ASCII)

    if(qtde == 0){
        printf("Cadastre uma pergunta primeiro.\n");
        return 0;
    }// if

    for (int i = 0; i < qtde; i++)
    {
        exibeEnunciado(pergunta[i]);
    }// for
    
    do{
        printf("Indique a posicao de uma pergunta a ser alterada: ");
        while (scanf("%d", &indice) != 1){
        printf("Entrada invalida! Digite apenas um numero: ");
        limparBuffer(); // Joga a letra fora do buffer
        }
        limparBuffer();
    }while(indice < 0 || indice >= qtde);
    
    exibirPergunta(pergunta[indice]);

    printf("\n");

    printf("--O que deseja alterar?--\n"
    "1 - Enunciado\n"
    "2 - Alternativas\n"
    "3 - Correta\n"
    "4 - Dificuldade\n"
    "Escolha: ");
    while (scanf("%d", &escolha) != 1) {
    printf("Entrada invalida! Digite apenas um numero: ");
    limparBuffer(); // Joga a letra fora do buffer
    }
    limparBuffer();
    
    switch(escolha){
        case ENUNCIADO:
            printf("Digite o novo enunciado: ");
            fgets(pergunta[indice].enunciado, 200, stdin);
            pergunta[indice].enunciado[strcspn(pergunta[indice].enunciado, "\n")] = '\0';

            break;

        case ALTERNATIVAS:
            do{
                printf("Qual alternativa? (A/B/C/D):");
                scanf(" %c", &letra);
                getchar();
                letra = toupper(letra);//Deixa a letra maiúscula
            }while(letra < 'A' || letra > 'D');//letra < 65 || letra > xx


            printf("Digite a nova alternativa %c: ", letra);

            intletra = letra - 'A';//Se A:  65 - 65 = 0 ..... B: 66 - 65 = 1...

            fgets(pergunta[indice].alternativa[intletra], 100, stdin);
            pergunta[indice].alternativa[intletra][strcspn(pergunta[indice].alternativa[intletra], "\n")] = '\0';

            break;

        case ALT_CORRETA:
            do{
                printf("Qual a nova alternativa correta? (A/B/C/D):");
                scanf(" %c", &letra);
                getchar();
                pergunta[indice].correta = toupper(letra);//Deixa a letra maiúscula
            }while(letra < 'A' || letra > 'D');//letra < 65 || letra > xx

            break;

        case DIFICULDADE:
            do{
            printf("Nova dificuldade (1-5): ");
            scanf("%d", &pergunta[indice].dificuldade);
            }while(pergunta[indice].dificuldade < 1 || pergunta[indice].dificuldade > 5);

            break;

        default:
            printf("Opção inválida.\n");
            break;
        
    }// switch

    if (escreveBinario(pergunta, qtde) == 1) {
        printf("Erro ao atualizar o arquivo.\n");
        return 1;
    }// if

    return 0;
}

/**
 * @brief Exclui pergunta cadastrada
 * 
 * @param perguntas  Ponteiro para vetor que possui todas perguntas
 * @param qtde Ponteiro para quantidade de perguntas cadastradas
 * @return int Sucesso (0) na inserção ou Falha (1)
 */
int excluir(Pergunta **pergunta, int *qtde){
    int indice;// pergunta escolhida para ser deletada

    if(qtde == 0){
        printf("Cadastre uma pergunta primeiro.\n");
        return 0;
    }// if

    do{
        printf("Digite um indice uma pergunta a ser excluida: ");
        for(int i = 0; i < (*qtde);i++){
            printf("%d,");
        }// for
        while (scanf("%d", &indice) != 1) {
        printf("Entrada invalida! Digite apenas um numero: ");
        limparBuffer(); // Joga a letra fora do buffer
        }
        limparBuffer();
    }while(indice < 0 || indice >= (*qtde));

    exibirPergunta((*pergunta)[indice]);

    (*qtde)--;

    for (int i = indice; i < (*qtde); i++)// puxa todas as questões para trás
    {
        (*pergunta)[i] = (*pergunta)[i+1];
    }// for

    if((*qtde) == 0){// não afeta o código de forma negativa e ainda previne erros
        free(*pergunta);
        *pergunta = NULL;
    }else{
        Pergunta *temp = realloc(*pergunta, (*qtde) * sizeof(Pergunta));// diminui o tamanho em 1
        if (temp != NULL){
            *pergunta = temp;
        }// if
    }// else

    if (escreveBinario(*pergunta, *qtde) == 1) {
        printf("Erro ao atualizar o arquivo.\n");
        return 1;
    }// if

    return 0;
}// excluir

/**
 * @brief Exibe pergunta
 * Função feita para facilitar as demais
 * @param pergunta Uma pergunta
 */
void exibirPergunta(Pergunta pergunta){
    printf("%-200s\n", pergunta.enunciado);
    printf("A) %-100s\n", pergunta.alternativa[0]);
    printf("B) %-100s\n", pergunta.alternativa[1]);
    printf("C) %-100s\n", pergunta.alternativa[2]);
    printf("D) %-100s\n", pergunta.alternativa[3]);
    printf("Alternativa correta - %c\n", pergunta.correta);
    printf("Dificuldade - %d\n", pergunta.dificuldade);
}// exibirPergunta

/**
 * @brief Exibe um enunciado
 * 
 */
void exibeEnunciado(Pergunta pergunta){
    printf("%-200s\n",pergunta.enunciado);
}// exibeEnunciado