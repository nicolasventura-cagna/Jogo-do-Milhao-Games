#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "cadastro.h"
#include "arquivo.h"

//FAZER INCLUDE GUARD


/**
 * @brief Insere quantas perguntas o usuário desejar
 * 
 * @param perguntas 
 * @param qtde 
 * @return int 
 */
int inserir(Pergunta **perguntas, int *qtde){
    int novas;//perguntas a adicionar
    int funcionou;
    Pergunta *temp = NULL;//Caso der erro no realloc, não ferra tudo :)

    do{
        printf("Quantas perguntas quer adicionar?: ");
        scanf("%d", &novas);
        getchar();
    }while(novas <= 0);

    temp = realloc(*perguntas, ((*qtde) + novas) * sizeof(Pergunta));
    if(temp == NULL){
        perror("ERRO");
        exit(1);
    }

    *perguntas = temp;

    for(int contador = (*qtde); contador < (*qtde) + novas; contador++){
        printf("Digite o enunciado da pergunta: ");
        fgets((*perguntas)[contador].enunciado, 200, stdin);
        (*perguntas)[contador].enunciado[strcspn((*perguntas)[contador].enunciado, "\n")] = '\0';

        for(int i=0; i<4; i++){
            printf("Digite a %d alternativa: ", i+1);
            fgets((*perguntas)[contador].alternativa[i], 100, stdin);
            (*perguntas)[contador].alternativa[i][strcspn((*perguntas)[contador].alternativa[i], "\n")] = '\0';
        }

        //getchar();
        printf("Qual a correta?: ");
        scanf(" %c", &(*perguntas)[contador].correta);


        printf("Digite a dificuldade (1(Mt facil) - 5(mt dificil)):");
        scanf("%d", &(*perguntas)[contador].dificuldade);
    }

    (*qtde) += novas;
    
    funcionou = escreveBinario(*perguntas, *qtde);

    if(funcionou == 1){
        perror("Erro, tente novamente");
        return 1;
    }

    return 0;
}

/**
 * @brief Lista perguntas cadastradas
 * 
 * @param pergunta 
 * @param qtde 
 */
void listar(Pergunta *pergunta, int qtde){
    if(qtde == 0){
        printf("Nenhuma pergunta cadastrada.\n");
        return;
    }

    for(int i = 0; i<qtde; i++){
        exibirPergunta(pergunta[i]);
    }
}

/**
 * @brief Pesquisa perguntas cadastradas
 * 
 * @param pergunta 
 * @param qtde 
 * @return int 
 */
int pesquisar(Pergunta *pergunta, int qtde){
    char pesquisa[201];
    int dificuldade;
    int opcao;
    int encontrou = 0;

    printf("Deseja pesquisar por enunciado (1) ou dificuldade (2)?: ");
    scanf("%d", &opcao);
    getchar();

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
                }
            }

            if(encontrou == 0){
                printf("Nenhum enunciado semelhante.\n");
            }

            break;

        case 2:
            printf("-Dificuldades-\n");
            printf("1 - Muito Fácil\n");
            printf("2 - Fácil\n");
            printf("3 - Média\n");
            printf("4 - Difícil\n");
            printf("5 - Muito Difícil\n");
            printf("Digite a dificuldade: ");
            scanf("%d", &dificuldade);

            for(int i=0; i<qtde; i++){
                if(pergunta[i].dificuldade == dificuldade){
                    exibirPergunta(pergunta[i]);
                    encontrou++;
                }       
            }

            if(encontrou == 0){
                printf("Nenhuma pergunta nessa categoria.\n");
            }

            break;

        default:
            printf("Opção inválida.\n");
            return 0;
    }


    return 0;
}

/**
 * @brief Altera pergunta cadastrada
 * 
 * @param pergunta 
 * @param qtde 
 * @return int 
 */
int alterar(Pergunta *pergunta, int qtde){
    int indice;//Qual pergunta a alterar
    int escolha;//O que alterar
    char letra;//Caso alteração de ALTERNATIVAS
    int intletra;//Letra dps de transformada indice.... Letra - (seu valor em ASCII)

    if(qtde == 0){
        printf("Cadastre uma pergunta primeiro.\n");
        return 0;
    }

    do{
    printf("Indice a pergunta a alterar: ");
    scanf("%d", &indice);
    }while(indice < 0 || indice >= qtde);

    exibirPergunta(pergunta[indice]);

    printf("\n");

    printf("--O que deseja alterar?--\n");
    printf("1 - Enunciado\n");
    printf("2 - Alternativas\n");
    printf("3 - Correta\n");
    printf("4 - Dificuldade\n");
    printf("Escolha: ");
    scanf("%d", &escolha);
    getchar();

    switch(escolha){
        case 1:
            printf("Digite o novo enunciado: ");
            fgets(pergunta[indice].enunciado, 200, stdin);
            pergunta[indice].enunciado[strcspn(pergunta[indice].enunciado, "\n")] = '\0';

            break;

        case 2:
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

        case 3:
            printf("Nova alternativa correta: ");
            scanf(" %c", &pergunta[indice].correta);//Dá pra usar a mesma coisa da case 2, mas fiquei com preguiça

            break;

        case 4:
            do{
            printf("Nova dificuldade (1-5): ");
            scanf("%d", &pergunta[indice].dificuldade);
            }while(pergunta[indice].dificuldade < 1 || pergunta[indice].dificuldade > 5);

            break;

        default:
            printf("Opção inválida.\n");
            break;
        
    }

    return 0;
}

/**
 * @brief Exclui pergunta cadastrada
 * 
 * @param pergunta 
 * @param qtde 
 * @return int 
 */
int excluir(Pergunta **pergunta, int *qtde){
    //.......................................

}

/**
 * @brief Exibe pergunta
 * Função feita para facilitar as demais
 * @param pergunta 
 */
void exibirPergunta(Pergunta pergunta){
    printf("%-200s\n", pergunta.enunciado);
    printf("A) %-100s\n", pergunta.alternativa[0]);
    printf("B) %-100s\n", pergunta.alternativa[1]);
    printf("C) %-100s\n", pergunta.alternativa[2]);
    printf("D) %-100s\n", pergunta.alternativa[3]);
    printf("%c\n", pergunta.correta);
    printf("%d\n", pergunta.dificuldade);
}