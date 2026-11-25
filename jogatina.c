#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jogo.h"
#include "minigames.h"
#include "interface.h"
#include "cadastro.h"
#include "arquivo.h"

#define LARGBLOCO 1000 //largura do bloco central do jogo

//Estado do menu de ajuda (os minigames ficam em minigame, em minigames.c)
typedef struct{
    int aberta;      //painel de ajuda aberto
    int usouUniv;    //1 = ja usou universitarios nessa partida
    int usouRemover; //1 = ja usou remover pergunta nessa partida
    char msg[100];   //aviso da ultima ajuda
    int usouTiro;    //1 = ja usou o tiro duplo nessa partida
    int tiroAtivo;   //1 = tiro duplo ligado nessa pergunta
    char tiroPrimeira;//primeira alternativa escolhida (0 = nenhuma)
    char tiroSegunda; //segunda alternativa escolhida
}EstadoAjuda;

#define CHANCE_SABOTADOR 30 //% de chance de ter um sabotador entre os 3 universitários

//Caixa de fala dos universitários
typedef struct{
    int aberto;         //caixa de fala aberta
    int consultou;      //1 = consultou nessa pergunta (pra revelar o sabotador depois de responder)
    int personagem[3];  //quem fala (indice na lista de personagens do interface.c)
    char palpite[3];    //letra que cada um deu
    char fala[3][120];  //o que cada um diz
    int sabotador;      //posição (0 a 2) do sabotador, -1 = não tem
    float tempo;        //segundos desde que abriu (efeito de digitar)
}EstadoConsulta;

static EstadoJogo estado = {0};
static EstadoAjuda ajuda = {0};
static EstadoConsulta consulta = { .sabotador = -1 };
static int somVitoriaTocado = 0;//pra tocar a fanfarra uma vez só

//Ranking (caixa de nome na tela de fim de jogo)
static CaixaTexto caixaNome;
static int nomeIniciado = 0;//caixa já foi preparada nessa partida
static int nomeSalvo = 0;//1 = já registrou no ranking
static char msgRanking[60] = "";

//Falas de cada personagem (%c = letra do palpite), na mesma ordem dos nomes em interface.c (p1 a p6)
static const char *falas[QTD_PERSONAGENS][2] = {
    //p1 - Jebediah Kerman
    {"Pelos meus cálculos, é a %c. Se explodir, foi culpa do foguete!", "Lançamento em 3, 2, 1... %c! Ops, esqueci o paraquedas."},
    //p2 - Mario
    {"Let's-a go! Eu voto na %c, mamma mia!", "Fácil, fácil! É a %c. Só não me peça pra pular o cano."},
    //p3 - Leon Kennedy
    {"Isso não é nada bom... mas a %c parece segura.", "Fica atrás de mim. É a %c, pode confiar."},
    //p4 - Steve
    {"Craftei essa resposta num bloco só: %c.", "Cuidado com os creepers! Ah, e a resposta é %c."},
    //p5 - Sonic
    {"Rápido demais pra pensar! É a %c, bora!", "Mais rápido que o som: %c! Já fui!"},
    //p6 - Pikachu
    {"Pika pika! (Tradução: é a %c)", "Pikaaa-chu! Quer dizer %c, com choque!"}
};

int jaForam(Partida *p, int indice);//está em jogo.c

//Devolve o nivel que nao tem perguntas suficientes (0 = tudo certo)
static int NivelFaltando(Pergunta *perguntas, int qtde){
    int cont;

    for(int nivel = 1; nivel <= 5; nivel++){
        cont = 0;

        for(int i = 0; i < qtde; i++){
            if(perguntas[i].dificuldade == nivel){
                cont++;
            }
        }

        if(cont < PERGUNTAS_POR_NIVEL){
            return nivel;
        }
    }

    return 0;
}

//Começa a partida se tiver perguntas suficientes, senão escreve o aviso em msg
static void ComecarJogo(Pergunta *perguntas, int qtde, Partida *partida, int modo, char *msg){
    int faltando = NivelFaltando(perguntas, qtde);

    if(faltando != 0){
        sprintf(msg, "Faltam perguntas do nivel %d (minimo %d)", faltando, PERGUNTAS_POR_NIVEL);
        return;
    }

    msg[0] = '\0';
    ReiniciarJogatina();//zera estado, ajudas e minigames da partida anterior
    liberarPartida(partida);//limpa a partida anterior (free(NULL) é seguro)
    iniciarPartida(partida, modo);
    telaAtual = 2;
}

//Botão do painel de ajuda: se já foi usado fica cinza e não clica
static int BotaoAjuda(Botao botao, int usada){
    if(!usada){
        return DesenhaBotao(botao, mouse);
    }

    DrawRectangleRounded(botao.retangulo, 0.3f, 8, DARKGRAY);
    DrawText(botao.palavra.caracteres, botao.palavra.posx, botao.palavra.posy, botao.palavra.tamanho, LIGHTGRAY);

    return 0;
}

//Quantas alternativas ainda estão na tela (as removidas não contam)
static int AlternativasRestantes(Partida *partida){
    int cont = 0;

    for(int i = 0; i < 4; i++){
        if(partida->altRemovida[i] == 0){
            cont++;
        }
    }

    return cont;
}

//Dá pra trocar a pergunta? Precisa sobrar uma pra troca + as das rodadas que faltam no nivel
static int PodeRemoverPergunta(Partida *partida, Pergunta *perguntas, int qtde, int nivel){
    int livres = 0;
    int rodadasFaltando = 2 - ((partida->rodada - 1) % 3);//rodadas do nivel depois dessa

    for(int i = 0; i < qtde; i++){
        if(perguntas[i].dificuldade == nivel && jaForam(partida, i) == 0){
            livres++;
        }
    }

    return livres >= 1 + rodadasFaltando;
}

//Escreve so os primeiros "restante" caracteres da linha (efeito de digitar)
static void DesenhaLinhaFala(const char *linha, int x, int y, int *restante){
    int tam = (int)strlen(linha);
    int mostrar = (*restante < tam) ? *restante : tam;

    //não corta no meio de uma letra com acento (UTF-8 usa 2 bytes)
    while(mostrar > 0 && mostrar < tam && ((unsigned char)linha[mostrar] & 0xC0) == 0x80){
        mostrar++;
    }

    if(mostrar > 0){
        DrawText(TextSubtext(linha, 0, mostrar), x, y, 20, BLACK);
    }

    *restante -= tam + 1;//+1 do espaço
    if(*restante < 0){
        *restante = 0;
    }
}

//Quebra a fala na largura do balão e escreve só os primeiros "ncars" caracteres
static void DesenhaFala(const char *texto, int x, int y, int largura, int ncars){
    char copia[160];
    char linha[160] = "";
    char teste[160];
    char *palavra;
    int restante = ncars;
    int linhas = 0;

    strncpy(copia, texto, sizeof(copia) - 1);
    copia[sizeof(copia) - 1] = '\0';

    palavra = strtok(copia, " ");

    while(palavra != NULL){
        strcpy(teste, linha);
        if(linha[0] != '\0'){
            strcat(teste, " ");
        }
        strcat(teste, palavra);

        if(MeasureText(teste, 20) > largura && linha[0] != '\0'){
            DesenhaLinhaFala(linha, x, y + linhas * 24, &restante);
            linhas++;
            strcpy(linha, palavra);
        }else{
            strcpy(linha, teste);
        }

        palavra = strtok(NULL, " ");
    }

    if(linha[0] != '\0'){
        DesenhaLinhaFala(linha, x, y + linhas * 24, &restante);
    }
}

//3 universitários (personagens sorteados) dão palpite. Quanto mais difícil o nível mais eles erram
//e pode ter um sabotador entre eles, que sempre dá uma resposta errada
static void ConsultarUniversitarios(Partida *partida, Pergunta pergunta, int nivel){
    int chance = 90 - nivel * 10;//nivel 1 = 80%, nivel 5 = 40%
    int ordem[QTD_PERSONAGENS];
    int cand[4];
    int total;
    int sorteio;
    int troca;
    int acerta;
    char letra;

    //embaralha os personagens, os 3 primeiros falam
    for(int i = 0; i < QTD_PERSONAGENS; i++){
        ordem[i] = i;
    }

    for(int i = QTD_PERSONAGENS - 1; i > 0; i--){
        sorteio = rand() % (i + 1);
        troca = ordem[i];
        ordem[i] = ordem[sorteio];
        ordem[sorteio] = troca;
    }

    consulta = (EstadoConsulta){0};
    consulta.sabotador = (rand() % 100 < CHANCE_SABOTADOR) ? rand() % 3 : -1;

    for(int k = 0; k < 3; k++){
        acerta = (rand() % 100 < chance) && k != consulta.sabotador;

        if(acerta){
            letra = pergunta.correta;
        }else{
            total = 0;
            for(int i = 0; i < 4; i++){
                if('A' + i != pergunta.correta && partida->altRemovida[i] == 0){
                    cand[total] = i;
                    total++;
                }
            }
            letra = (total > 0) ? 'A' + cand[rand() % total] : pergunta.correta;
        }

        consulta.personagem[k] = ordem[k];
        consulta.palpite[k] = letra;
        snprintf(consulta.fala[k], sizeof(consulta.fala[k]), falas[ordem[k]][rand() % 2], letra);
    }

    consulta.aberto = 1;
    consulta.consultou = 1;
    consulta.tempo = 0;

    //resumo que fica na tela depois de fechar a caixa
    sprintf(ajuda.msg, "Universitarios: %c, %c e %c", consulta.palpite[0], consulta.palpite[1], consulta.palpite[2]);
}

//Caixa de fala dos 3 universitários, desenhada por cima de tudo (tipo um NPC falando)
static void CaixaUniversitarios(void){
    int altPainel = 410;
    int larg = 300;//largura de cada coluna
    int folga = 30;//espaço entre colunas
    int px = (COMPTELA - LARGBLOCO) / 2;
    int py = (ALTTELA - altPainel) / 2;
    int cx;
    int ncars;
    const char *nome;

    Rectangle painel = {px, py, LARGBLOCO, altPainel};
    Botao entendi = CriarBotao(px + (LARGBLOCO - 240) / 2, py + 345, 240, ALTRET, "Entendi");

    consulta.tempo += GetFrameTime();

    //clicar fora do botão pula o efeito de digitar (0.2s de folga pro clique que abriu a caixa)
    if(consulta.tempo > 0.2f && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
       && !CheckCollisionPointRec(mouse, entendi.retangulo)){
        consulta.tempo = 99;
    }

    DrawRectangle(0, 0, COMPTELA, ALTTELA, Fade(BLACK, 0.55f));
    DrawRectangleRounded(painel, 0.04f, 8, Fade(BLACK, 0.85f));

    DrawText("UNIVERSITÁRIOS", px + (LARGBLOCO - MeasureText("UNIVERSITÁRIOS", 28))/2, py + 12, 28, CORTEXTO);

    for(int k = 0; k < 3; k++){
        cx = px + 20 + k * (larg + folga);
        nome = NomePersonagem(consulta.personagem[k]);

        Rectangle retrato = {cx + (larg - 128) / 2, py + 50, 128, 128};
        Rectangle balao = {cx, py + 222, larg, 110};

        //retrato com moldura e nome
        DrawRectangleRounded((Rectangle){retrato.x - 6, retrato.y - 6, 140, 140}, 0.15f, 8, Fade(WHITE, 0.15f));
        DesenhaPersonagem(consulta.personagem[k], retrato);
        DrawText(nome, cx + (larg - MeasureText(nome, 24))/2, py + 188, 24, YELLOW);

        //balão de fala, com a pontinha apontando pro personagem
        DrawRectangleRounded(balao, 0.15f, 8, RAYWHITE);
        DrawTriangle((Vector2){cx + larg/2 - 12, py + 222}, (Vector2){cx + larg/2 + 12, py + 222},
                     (Vector2){cx + larg/2, py + 210}, RAYWHITE);

        //cada um começa um pouquinho depois do anterior
        ncars = (int)((consulta.tempo - k * 0.4f) * 40.0f);
        if(ncars < 0){
            ncars = 0;
        }

        DesenhaFala(consulta.fala[k], cx + 12, py + 232, larg - 24, ncars);
    }

    if(DesenhaBotao(entendi, mouse)){
        consulta.aberto = 0;
    }
}

/**
 * @brief Usuário clicou em Jogar, agora escolhe o modo
 * telaAtual = 1
 */
void OpcJogarModo(Pergunta *perguntas, int qtde, Partida *partida){
    static char msg[80] = "";//precisa sobreviver entre os frames

    int posx = (COMPTELA / 2) - (LARGRET / 2);
    int posy = (ALTTELA / 2) - ALTRET;

    Botao back = CriarBotao(POSBACK, POSBACK, MEDBACK, MEDBACK, "<");

    Botao facil = CriarBotao(posx, posy, LARGRET, ALTRET, "Fácil");

    Botao medio = CriarBotao(posx, posy + (ESPACAMENTO), LARGRET, ALTRET, "Médio");

    Botao dificil = CriarBotao(posx, posy + (2 * ESPACAMENTO), LARGRET, ALTRET, "Difícil");

    BeginDrawing();

    DesenhaFundo();

    DrawText("ESCOLHA O MODO", (COMPTELA - MeasureText("ESCOLHA O MODO", 50))/2, posy - 110, 50, CORTEXTO);

    //BACK
    if(DesenhaBotao(back, mouse)){
        msg[0] = '\0';
        telaAtual = 0;
    }

    //FÁCIL
    if(DesenhaBotao(facil, mouse)){
        ComecarJogo(perguntas, qtde, partida, 0, msg);
    }

    //MÉDIO
    if(DesenhaBotao(medio, mouse)){
        ComecarJogo(perguntas, qtde, partida, 1, msg);
    }

    //DIFÍCIL
    if(DesenhaBotao(dificil, mouse)){
        ComecarJogo(perguntas, qtde, partida, 2, msg);
    }

    DrawText(msg, (COMPTELA - MeasureText(msg, 24))/2, posy + (3 * ESPACAMENTO), 24, RED);

    EndDrawing();
}

/**
 * @brief Tela do jogo todo, basicamente, mostra as perguntas
 * telaAtual = 2
 */
void JogoPerg(Pergunta *perguntas, int qtde, Partida *partida){
    int mx = (COMPTELA - LARGBLOCO) / 2;//margem esquerda do bloco central
    int posy = (ALTTELA / 2) - 100;     //onde começam as alternativas
    int largAlt = (LARGBLOCO - 20) / 2; //largura de cada alternativa (2 colunas)
    int yres = posy + 245;              //onde aparece o resultado
    char status[120];
    char letraFinal;

    int nivel;
    int idx; //indice pergunta

    Botao back = CriarBotao(POSBACK, POSBACK, MEDBACK, MEDBACK, "<");

    Botao continuar = CriarBotao(COMPTELA - mx - LARGRET, ALTTELA - 90, LARGRET, ALTRET, "Continuar");

    //Ajuda: embaixo, depois da zona do Parar (SIM/NAO ocupam de mx até mx + 220)
    Botao ajudaBtn = CriarBotao(mx + 250, ALTTELA - 90, 150, ALTRET, "Ajuda");

    //Parar: embaixo à esquerda (a confirmação SIM/NAO aparece no lugar dele)
    Botao parar = CriarBotao(mx, ALTTELA - 90, 150, ALTRET, "Parar");

    BeginDrawing();

    DesenhaFundo();

    if(estado.fimDeJogo != 2){//Se usuário n desistiu, continua

        if(partida->rodada > TOTAL_RODADAS){
            estado.fimDeJogo = 0; // Vitória
            telaAtual = 9;//tela winlose
            EndDrawing();
            return;
        }

        //USUÁRIO CLICA EM BACK (fecha a ajuda pra nunca ficar uma em cima da outra)
        if(DesenhaBotao(back, mouse)){
            estado.saida = 1;
            ajuda.aberta = 0;
        }

        //USUÁRIO DECIDE SE QUER SAIR
        if(estado.saida == 1){
            Botao sim = CriarBotao(mx, ALTTELA - 90, 100, ALTRET, "SIM");
            Botao nao = CriarBotao(mx + 120, ALTTELA - 90, 100, ALTRET, "NAO");

            DrawText("Parar agora leva so metade do dinheiro. Deseja parar?", mx, ALTTELA - 130, 24, CORTEXTO);

            if(DesenhaBotao(sim, mouse)){
                estado.fimDeJogo = 2;//Desistiu
                partida->cash = partida->cash / 2;//desistir leva so metade
                telaAtual = 9;//vai pra tela de fim
            }
            if(DesenhaBotao(nao, mouse)){
                estado.saida = 0;
            }
        }

        //BOTÃO PARAR (some enquanto a confirmação está aberta, que ocupa o mesmo lugar)
        if(estado.saida == 0 && DesenhaBotao(parar, mouse) && !consulta.aberto){
            estado.saida = 1;
            ajuda.aberta = 0;
        }

        //USUÁRIO CLICA EM AJUDA (só antes de responder) - abre/fecha o painel
        if(estado.respostaProcessada == 0 && DesenhaBotao(ajudaBtn, mouse) && !consulta.aberto){
            ajuda.aberta = !ajuda.aberta;
            estado.saida = 0;
        }

        //COMEÇO
        if(estado.iniciado == 0){
            estado.iniciado = 1;
            estado.idxPergunta = -1;
        }

        if(estado.idxPergunta == -1){

            nivel = (partida->rodada - 1) / 3 + 1;

            estado.idxPergunta = sortearPergunta(
                partida, perguntas, qtde, nivel);

            //premio da killstreak anterior: tira uma alternativa
            //(tem que ser depois do sorteio, que zera as removidas)
            if(estado.idxPergunta != -1 && estado.premioPendente == 1){
                removerAlternativaErrada(partida, perguntas[estado.idxPergunta]);
            }
            estado.premioPendente = -1;
        }

        idx = estado.idxPergunta;
        if(idx == -1){
            estado.fimDeJogo = 3;//acabaram as perguntas do nivel
            telaAtual = 9;

            EndDrawing();
            return;
        }

        nivel = (partida->rodada - 1) / 3 + 1;

        //VOLTOU DE UM MINIGAME: aplica o resultado (só uma vez)
        if(minigame.pendente == 1){
            minigame.pendente = 0;

            if(minigame.vitoria == 1){
                removerAlternativaErrada(partida, perguntas[idx]);
                strcpy(ajuda.msg, "Ganhou! Uma alternativa foi eliminada.");
            }else{
                strcpy(ajuda.msg, "Perdeu no minigame. Sem ajuda.");
            }
        }

        //Barra de status, alinhada com o bloco
        DrawText(TextFormat("Rodada %d/%d | Nivel %d", partida->rodada, TOTAL_RODADAS, nivel), mx, 25, 24, CORTEXTO);

        sprintf(status, "Vidas: %d | R$ %d | Acertos seguidos: %d", partida->vidas, partida->cash, partida->acertosSeg);
        DrawText(status, mx + LARGBLOCO - MeasureText(status, 24), 25, 24, CORTEXTO);

        if(partida->bonus > 0){
            DrawText(TextFormat("+%.1fx nessa pergunta!", partida->bonus), mx, 55, 24, YELLOW);
        }

        //Enunciado
        QuebraTexto(perguntas[idx].enunciado, mx, 100, LARGBLOCO, 1);

        //Alternativas 2x2, com quebra de linha dentro da caixa
        for(int i = 0; i < 4; i++){
            Rectangle ret = {mx + (largAlt + 20) * (i % 2), posy + 120 * (i / 2), largAlt, 100};
            int removida = partida->altRemovida[i];
            int clicavel = !removida && estado.respostaProcessada == 0 && estado.saida == 0 && !ajuda.aberta && !consulta.aberto;
            int emcima = clicavel && CheckCollisionPointRec(mouse, ret);
            Color cor = CORSMOUSE;
            char alt[150];

            if(removida){
                cor = DARKGRAY;
            }else if(emcima){
                cor = CORCMOUSE;
            }

            //tiro duplo: se a primeira escolhida foi eliminada (minigame), solta a escolha
            if(removida && ajuda.tiroPrimeira == 'A' + i){
                ajuda.tiroPrimeira = 0;
            }

            //tiro duplo: a primeira escolhida fica em laranja
            if(ajuda.tiroAtivo && ajuda.tiroPrimeira == 'A' + i){
                cor = NEON_LARANJA;
            }

            //depois de responder: certa em verde, errada escolhida em vermelho
            if(estado.respostaProcessada == 1){
                if('A' + i == perguntas[idx].correta){
                    cor = DARKGREEN;
                }else if('A' + i == estado.respostaFeita || (ajuda.tiroAtivo && 'A' + i == ajuda.tiroSegunda)){
                    cor = MAROON;
                }
            }

            DrawRectangleRounded(ret, 0.15f, 8, cor);

            if(!removida){
                sprintf(alt, "%c) %s", 'A' + i, perguntas[idx].alternativa[i]);
                QuebraTexto(alt, ret.x + 15, ret.y + 10, ret.width - 30, 1);
            }

            if(emcima && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                if(ajuda.tiroAtivo == 0){
                    estado.respostaFeita = 'A' + i;
                    estado.respondeu = 1;
                }else if(ajuda.tiroPrimeira == 0){
                    ajuda.tiroPrimeira = 'A' + i;//1a escolha do tiro duplo
                }else if(ajuda.tiroPrimeira == 'A' + i){
                    ajuda.tiroPrimeira = 0;//clicou de novo na mesma: desmarca
                }else{
                    ajuda.tiroSegunda = 'A' + i;//2a escolha: responde
                    estado.respostaFeita = ajuda.tiroPrimeira;
                    estado.respondeu = 1;
                }
            }
        }

        if(estado.respondeu == 1 && estado.respostaProcessada == 0){

            letraFinal = estado.respostaFeita;

            if(ajuda.tiroAtivo){
                partida->tiroDuplo = 1;//acertando, vale metade

                if(ajuda.tiroSegunda == perguntas[idx].correta){
                    letraFinal = ajuda.tiroSegunda;//uma das duas era a certa
                }
            }

            estado.acertou = conferirResposta(
                partida, perguntas[idx], letraFinal);

            estado.respostaProcessada = 1;

            if(estado.acertou){
                estado.premioPendente = aplicarKillstreak(partida);
                TocarSom(estado.premioPendente >= 0 ? SOM_KILLSTREAK : SOM_ACERTO);
            }else{
                estado.premioPendente = -1;
            }

            //revela quem era o sabotador (se consultou os universitários nessa pergunta)
            if(consulta.consultou == 1){
                if(consulta.sabotador >= 0){
                    sprintf(ajuda.msg, "O sabotador era %s!", NomePersonagem(consulta.personagem[consulta.sabotador]));
                }else{
                    strcpy(ajuda.msg, "Nao tinha sabotador dessa vez.");
                }
            }
        }

        //TIRO DUPLO: instrução enquanto escolhe
        if(ajuda.tiroAtivo && estado.respostaProcessada == 0){
            if(ajuda.tiroPrimeira == 0){
                DrawText("TIRO DUPLO: escolha 2 alternativas (vale metade)", mx, yres, 24, YELLOW);
            }else{
                DrawText(TextFormat("TIRO DUPLO: escolheu %c, falta mais 1 (vale metade)", ajuda.tiroPrimeira), mx, yres, 24, YELLOW);
            }
        }

        if(estado.respostaProcessada == 1){
            if(estado.acertou){
                DrawText("ACERTOU!", mx, yres, 32, GREEN);

                if(estado.premioPendente == 0){
                    DrawText("KILLSTREAK! Ganhou R$ 1000 de bonus!", mx, yres + 45, 24, CORTEXTO);
                }else if(estado.premioPendente == 1){
                    DrawText("KILLSTREAK! Uma alternativa sera eliminada na proxima pergunta.", mx, yres + 45, 24, CORTEXTO);
                }else if(estado.premioPendente == 2){
                    DrawText("KILLSTREAK! A proxima pergunta vale +0.5x de dinheiro.", mx, yres + 45, 24, CORTEXTO);
                }else if(estado.premioPendente == 3){
                    DrawText("KILLSTREAK! Ganhou uma vida extra!", mx, yres + 45, 24, CORTEXTO);
                }

                if(ajuda.tiroAtivo){
                    DrawText("Tiro duplo: valeu metade do valor.", mx, yres + 80, 24, YELLOW);
                }
            }else{
                DrawText("ERROU!", mx, yres, 32, RED);

                DrawText(TextFormat("Resposta correta: %c",
                    perguntas[idx].correta),
                    mx, yres + 45, 24, CORTEXTO);
            }
        }

        //Aviso da última ajuda, ao lado do botão
        if(ajuda.msg[0] != '\0' && estado.saida == 0){
            DrawText(ajuda.msg, mx + 250, ALTTELA - 125, 24, CORTEXTO);
        }

        //PAINEL DE AJUDA (desenhado por último pra ficar por cima das alternativas)
        if(ajuda.aberta == 1 && estado.respostaProcessada == 0){
            int px = (COMPTELA - 380) / 2;
            int py = 130;

            Rectangle painel = {px, py, 380, 465};

            Botao jok = CriarBotao(px + 20, py + 65, 340, ALTRET, "Jokempo");
            Botao parimp = CriarBotao(px + 20, py + 130, 340, ALTRET, "Par ou Ímpar");
            Botao univ = CriarBotao(px + 20, py + 195, 340, ALTRET, "Universitários");
            Botao remover = CriarBotao(px + 20, py + 260, 340, ALTRET, "Remover pergunta");
            Botao tiro = CriarBotao(px + 20, py + 325, 340, ALTRET, "Tiro Duplo");
            Botao fechar = CriarBotao(px + 20, py + 390, 340, ALTRET, "Fechar");

            DrawRectangleRounded(painel, 0.05f, 8, Fade(BLACK, 0.8f));
            DrawText("AJUDAS", px + (380 - MeasureText("AJUDAS", ALTTEXTO))/2, py + 15, ALTTEXTO, CORTEXTO);

            //JOKEMPO
            if(BotaoAjuda(jok, minigame.jokempo)){
                IniciarMinigame(2);
                ajuda.aberta = 0;
                telaAtual = 3;
            }

            //PAR OU ÍMPAR
            if(BotaoAjuda(parimp, minigame.parimpar)){
                IniciarMinigame(1);
                ajuda.aberta = 0;
                telaAtual = 3;
            }

            //UNIVERSITÁRIOS
            if(BotaoAjuda(univ, ajuda.usouUniv)){
                ConsultarUniversitarios(partida, perguntas[idx], nivel);
                ajuda.usouUniv = 1;
                ajuda.aberta = 0;
            }

            //REMOVER PERGUNTA (sorteia outra do mesmo nivel, a rodada não muda)
            if(BotaoAjuda(remover, ajuda.usouRemover)){
                if(PodeRemoverPergunta(partida, perguntas, qtde, nivel)){
                    ajuda.usouRemover = 1;
                    ajuda.tiroPrimeira = 0;//pergunta nova, escolha do tiro duplo recomeça
                    estado.idxPergunta = -1;
                    strcpy(ajuda.msg, "Pergunta trocada!");
                }else{
                    strcpy(ajuda.msg, "Sem perguntas sobrando nesse nivel.");
                }
                ajuda.aberta = 0;
            }

            //TIRO DUPLO (escolhe 2 alternativas, mas vale metade do valor)
            if(BotaoAjuda(tiro, ajuda.usouTiro)){
                if(AlternativasRestantes(partida) >= 3){//com só 2 sobrando seria garantido
                    ajuda.usouTiro = 1;
                    ajuda.tiroAtivo = 1;
                    ajuda.tiroPrimeira = 0;
                    ajuda.tiroSegunda = 0;
                }else{
                    strcpy(ajuda.msg, "Poucas alternativas pro tiro duplo.");
                }
                ajuda.aberta = 0;
            }

            //FECHAR
            if(DesenhaBotao(fechar, mouse)){
                ajuda.aberta = 0;
            }
        }

        //CAIXA DE FALA DOS UNIVERSITÁRIOS (por cima de tudo)
        if(consulta.aberto == 1){
            CaixaUniversitarios();
        }

        if(estado.respostaProcessada == 1){
            if(DesenhaBotao(continuar, mouse)){
                ajuda.msg[0] = '\0';
                consulta.consultou = 0;
                ajuda.tiroAtivo = 0;//tiro duplo vale só pra uma pergunta
                ajuda.tiroPrimeira = 0;
                ajuda.tiroSegunda = 0;

                if(partida->vidas <= 0){
                    //Manter info pra mandar pra tela winlose
                    estado.fimDeJogo = 1;// 1 perde

                    estado.idxPergunta = -1;
                    estado.respondeu = 0;
                    estado.respostaProcessada = 0;
                    estado.respostaFeita = '\0';
                    telaAtual = 9;
                }else{
                    partida->rodada++;

                    estado.idxPergunta = -1;
                    estado.respondeu = 0;
                    estado.respostaProcessada = 0;
                    estado.respostaFeita = '\0';
                }
            }
        }
    }

    EndDrawing();
}

/**
 * @brief Tela vitória/derrota
 * 
 */
void WinLose(Partida partida){
    char *titulo = "FIM DE JOGO";
    char *texto;
    char valor[40];
    int meio = ALTTELA / 2;

    Botao menu = CriarBotao((COMPTELA - LARGRET) / 2, ALTTELA - 120, LARGRET, ALTRET, "Menu");

    BeginDrawing();
    
    DesenhaFundo();

    //fanfarra da vitória, uma vez só (WinLose roda todo frame)
    if(estado.fimDeJogo == 0 && somVitoriaTocado == 0){
        TocarSom(SOM_VITORIA);
        somVitoriaTocado = 1;
    }

    if(estado.fimDeJogo == 0){
        titulo = "VITORIA!";
        texto = "Voce terminou todas as rodadas com:";
    }else if(estado.fimDeJogo == 1){
        texto = "Acabaram as vidas. Voce terminou com:";
    }else if(estado.fimDeJogo == 2){
        texto = "Voce parou e levou metade do dinheiro:";
    }else{
        texto = "Acabaram as perguntas do nivel. Voce terminou com:";
    }

    sprintf(valor, "R$ %d", partida.cash);

    DrawText(titulo, (COMPTELA - MeasureText(titulo, 60))/2, meio - 180, 60, CORTEXTO);
    DrawText(texto, (COMPTELA - MeasureText(texto, ALTTEXTO))/2, meio - 60, ALTTEXTO, CORTEXTO);
    DrawText(valor, (COMPTELA - MeasureText(valor, 70))/2, meio, 70, GREEN);

    //RANKING: só quem venceu (0) ou parou (2) registra o nome, e só se tiver dinheiro
    if((estado.fimDeJogo == 0 || estado.fimDeJogo == 2) && partida.cash > 0){
        if(nomeIniciado == 0){
            IniciarCaixa(&caixaNome, 0, 0, 360, 45, 20);
            caixaNome.ativa = 1;//já deixa pronta pra digitar
            nomeIniciado = 1;
        }

        //posição recalculada todo frame (janela redimensionável)
        caixaNome.retangulo = (Rectangle){(COMPTELA - 360) / 2, meio + 125, 360, 45};

        if(nomeSalvo == 0){
            Botao salvar = CriarBotao((COMPTELA - 200) / 2, meio + 185, 200, ALTRET, "Salvar");
            int enter;
            int temLetra = 0;

            DrawText("Digite seu nome para o ranking:",
                     (COMPTELA - MeasureText("Digite seu nome para o ranking:", 24))/2, meio + 90, 24, CORTEXTO);

            DesenhaCaixa(&caixaNome, mouse);

            enter = IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER);

            if(DesenhaBotao(salvar, mouse) || (enter && caixaNome.ativa)){
                for(int i = 0; i < caixaNome.tamanho; i++){//nome só com espaços não vale
                    if(caixaNome.texto[i] != ' '){
                        temLetra = 1;
                    }
                }

                if(temLetra == 0){
                    strcpy(msgRanking, "Digite um nome.");
                }else{
                    Registro novo;

                    strcpy(novo.nome, caixaNome.texto);
                    novo.cash = partida.cash;
                    novo.modo = partida.modo;

                    if(salvarRanking(novo) == 0){
                        nomeSalvo = 1;
                        strcpy(msgRanking, "Salvo no ranking!");
                    }else{
                        strcpy(msgRanking, "Erro ao salvar o ranking.");
                    }
                }
            }

            DrawText(msgRanking, (COMPTELA - MeasureText(msgRanking, 24))/2, meio + 245, 24, RED);
        }else{
            DrawText(msgRanking, (COMPTELA - MeasureText(msgRanking, ALTTEXTO))/2, meio + 110, ALTTEXTO, GREEN);
        }
    }

    if(DesenhaBotao(menu, mouse)){
        telaAtual = 0;
    }

    EndDrawing();
}

/**
 * @brief Reinicia jogatina (chamado a cada partida nova)
 * 
 */
void ReiniciarJogatina(void){
    estado = (EstadoJogo){0};
    ajuda = (EstadoAjuda){0};
    consulta = (EstadoConsulta){0};
    consulta.sabotador = -1;
    somVitoriaTocado = 0;
    nomeIniciado = 0;
    nomeSalvo = 0;
    msgRanking[0] = '\0';
    ReiniciarMinigame();

    estado.idxPergunta = -1;
    estado.premioPendente = -1;
}