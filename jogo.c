#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#include "jogo.h"
#include "cadastro.h"

#define ACERTOS_KILLSTREAK 4
#define VIDA_MAX 5
#define TOTAL_RODADAS 15
#define PERGUNTAS_POR_NIVEL 3

//quanto cada pergunta vale
int valorBase[6] = {0, 1000, 2000, 5000, 10000, 50000};

//Coloca vidas e multiplicador conforme o modo e zera o resto
void iniciarPartida(Partida *p, int modo){
    p->modo = modo;

    if (modo == 0)//pra atribuir as coisas de cada modo/dificuldade
    {
     p->vidas = 3;
        p->multiplicador = 1.0;
    }
    else if(modo == 1){
        p->vidas = 2;
        p->multiplicador = 1.5;
    }
    else{
        p->vidas = 1;
        p->multiplicador = 2.0;
    }

    p->bonus = 0;
    p->cash = 0;
    p->acertosSeg = 0;
    p->rodada = 1;//pra começar na rodada 1
    p->usadas = NULL;//sem sorteio ainda
    p->qtdUsadas = 0;

    //Zero alternativas removidas ainda
    p->altRemovida[0] = 0;
    p->altRemovida[1] = 0;
    p->altRemovida[2] = 0;
    p->altRemovida[3] = 0;
}

//Esse checa a pergunta já saiu nessa partida (1 = saiu, 0 = não)
int jaForam(Partida *p, int indice){
    for(int i = 0; i < p->qtdUsadas; i++){
        if(p->usadas[i] == indice){
            return 1;
        }
    }
    return 0;
}

//Sorteia uma pergunta do nivel que ainda não saiu
int sortearPergunta(Partida *p, Pergunta *perguntas, int qtde, int nivel){
    int total = 0;//quantas perguntas servem
    int sorteado;
    int *candidatas;
    int *temp;

    if(qtde <= 0){
        return -1;
    }

    candidatas = malloc(qtde * sizeof(int));
    if(candidatas == NULL){
        return -1;
    }

    //guarda as do nivel que ainda não foram usadas
    for(int i = 0; i < qtde; i++){
        if(perguntas[i].dificuldade == nivel && jaForam(p, i) == 0){
            candidatas[total] = i;
            total++;
        }
    }

    if(total == 0){
        free(candidatas);
        return -1;
    }

    sorteado = candidatas[rand() % total];
    free(candidatas);

    //aumenta o vetor de perguntas usadas pra 1
    temp = realloc(p->usadas, (p->qtdUsadas + 1) * sizeof(int));
    if(temp == NULL){
        return -1;
    }

    p->usadas = temp;
    p->usadas[p->qtdUsadas] = sorteado;
    p->qtdUsadas++;

    //pergunta nova = nenhuma alternativa removida
    p->altRemovida[0] = 0;
    p->altRemovida[1] = 0;
    p->altRemovida[2] = 0;
    p->altRemovida[3] = 0;

    return sorteado;
}

//Vê se o jogador acertou, duh
int conferirResposta(Partida *p, Pergunta pergunta, char resposta){
    int valor;

    //toupper caso digite a resposta em minuscula
    if(toupper(resposta) == pergunta.correta){
        valor = valorBase[pergunta.dificuldade] * (p->multiplicador + p->bonus);
        p->cash += valor;
        p->acertosSeg++;
        p->bonus = 0;//bonus é so pra uma pergunta
        return 1;
    }

    p->vidas--;
    p->acertosSeg = 0;
    p->bonus = 0;
    return 0;
}

//Tira uma alternativa errada 
int removerAlternativaErrada(Partida *p, Pergunta pergunta){
    int candidatas[4];
    int total = 0;

    for(int i = 0; i < 4; i++){
        //'A' + i indica a letra da alternativa (A,B,C,D)
        if('A' + i != pergunta.correta && p->altRemovida[i] == 0){
            candidatas[total] = i;
            total++;
        }
    }

    if(total == 0){
        return -1;
    }

    int escolhida = candidatas[rand() % total];
    p->altRemovida[escolhida] = 1;

    return escolhida;
}

//Com 4 acertos seguidos sorteia um premio
int aplicarKillstreak(Partida *p){
    int sorteio;

    if(p->acertosSeg < ACERTOS_KILLSTREAK){
        return -1;
    }

    p->acertosSeg = 0;
    sorteio = rand() % 100;

    if(sorteio < 35){
        p->cash += 1000;
        return 0;
    }

    if(sorteio < 65){
        //remove alternativa na proxima pergunta 
        return 1;
    }

    if(sorteio < 90){
        p->bonus = 0.5;
        return 2;
    }

    //vida extra
    if(p->vidas < VIDA_MAX){
        p->vidas++;
    }
    return 3;
}

//Limpa a memória das perguntas usadas
void liberarPartida(Partida *p){
    free(p->usadas);
    p->usadas = NULL;
    p->qtdUsadas = 0;
}

//Mostra a pergunta pro jogador sem a resposta
void exibirPerguntaJogo(Pergunta pergunta, Partida *p){
    printf("\n%s\n", pergunta.enunciado);

    for(int i = 0; i < 4; i++){
        if(p->altRemovida[i] == 0){
            printf("%c) %s\n", 'A' + i, pergunta.alternativa[i]);
        }
    }
}

//Confere se tem pergunta suficiente pra jogar o jogo.
int temPerguntasSuficientes(Pergunta *perguntas, int qtde){
    int contador;

    for(int nivel = 1; nivel <= 5; nivel++){
        contador = 0;

        for(int i = 0; i < qtde; i++){
            if(perguntas[i].dificuldade == nivel){
                contador++;
            }
        }

        if(contador < PERGUNTAS_POR_NIVEL){
            printf("Faltam perguntas do nivel %d (tem %d, precisa de %d).\n",
                   nivel, contador, PERGUNTAS_POR_NIVEL);
            return 0;
        }
    }

    return 1;
}

//A partida inteira basicamente. :p
int jogar(Pergunta *perguntas, int qtde){
    Partida p;
    int modo;
    int nivel;
    int idx;
    int premio = -1;//premio da killstreak do round anterior  
    int parou = 0;
    char resposta;

    if(temPerguntasSuficientes(perguntas, qtde) == 0){
        return 0;
    }

    do{
        printf("Escolha o modo:\n");
        printf("0 = Facil (3 vidas e 1.0x dinheiro)\n");
        printf("1 = Medio (2 vidas e 1.5x dinheiro)\n");
        printf("2 = Dificil (1 vida e 2.0x dinheiro)\n");
        printf("Modo: ");
        scanf("%d", &modo);
    }while(modo < 0 || modo > 2);

    iniciarPartida(&p, modo);

    while(p.vidas > 0 && p.rodada <= TOTAL_RODADAS && parou == 0){
        nivel = (p.rodada - 1) / 3 + 1;//faz aquela separação de rodadas e nivel

        idx = sortearPergunta(&p, perguntas, qtde, nivel);
        if(idx == -1){
            printf("Acabaram as perguntas do nivel %d.\n", nivel);
            break;
        }

        //premio da killstreak: tira uma alternativa
        if(premio == 1){
            removerAlternativaErrada(&p, perguntas[idx]);
        }

        printf("\n--- Rodada %d/%d | Nivel %d ---\n", p.rodada, TOTAL_RODADAS, nivel);
        printf("Vidas: %d | Dinheiro: %d | Acertos seguidos: %d\n", p.vidas, p.cash, p.acertosSeg);

        if(p.bonus > 0){
            printf("Essa pergunta vale +%.1fx!\n", p.bonus);
        }

        exibirPerguntaJogo(perguntas[idx], &p);

        printf("Resposta (A-D) ou P para parar:");
        scanf(" %c", &resposta);
        resposta = toupper(resposta); //btw achei esse toupper muito bom.

        if(resposta == 'P'){
            parou = 1;
            continue;
        }

        //se digitar letra invalida ele pede de novo
        while(resposta < 'A' || resposta > 'D' || p.altRemovida[resposta - 'A'] == 1){
            printf("Resposta invalida. Por favor digite de novo: ");
            scanf(" %c", &resposta);
            resposta = toupper(resposta);
        }

        if(conferirResposta(&p, perguntas[idx], resposta) == 1){
            printf("Acertou!\n");

            premio = aplicarKillstreak(&p);

            if(premio == 0){
                printf("KILLSTREAK! Ganhou R$ 1000 de bonus!\n");
            }
            else if(premio == 1){
                printf("KILLSTREAK! Uma alternativa vai ser eliminada na proxima pergunta.\n");
            }
            else if(premio == 2){
                printf("KILLSTREAK! A proxima pergunta vale +0.5x de dinheiro.\n");
            }
            else if(premio == 3){
                printf("KILLSTREAK! WoW! Ganhou uma vida extra!\n");
            }
        }
        else{
            printf("Errou! A correta era a %c.\n", perguntas[idx].correta);
            premio = -1;//errou, perde o premio que tinha
        }

        p.rodada++;
    }

    //tela de fim de jogo
    printf("\nFIM DE JOGO\n");

    if(parou == 1){
        printf("Voce parou e levou R$ %d\n", p.cash);
    }
    else if(p.vidas <= 0){
        printf("Acabaram as vidas. Voce terminou com R$ %d\n", p.cash);
    }
    else{
        printf("PARABENS! Terminou todas as rodadas com R$ %d\n", p.cash);
    }

    int total = p.cash;
    liberarPartida(&p);

    return total;
}

//gcc main.c cadastro.c arquivo.c jogo.c -o jogo
// ./jogo

//pra testar ai