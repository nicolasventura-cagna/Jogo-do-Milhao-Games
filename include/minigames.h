#ifndef MINIGAMES_H
#define MINIGAMES_H

typedef struct{
    int escolha;      //qual minigame esta rodando: 0 - nenhum, 1 - par/impar, 2 - jokempo

    int parimpar;     //0 - nunca jogou, 1 - ja jogou (nessa partida)
    int jokempo;      //0 - nunca jogou, 1 - ja jogou (nessa partida)

    //Par ou Ímpar
    int opcao;        //clicou em par ou impar
    int maquina;      //0 - nao fez nd, 1 - sorteou numero
    int numusu;       // -1 = não escolheu, 0 a 9 = número escolhido
    int nummaq;
    int rangenumero;  //numero dentro do range, 1- está
    int soma;
    char parimp;      //'P' ou 'I'

    //Jokempo (0 pedra, 1 papel, 2 tesoura)
    int jokusu;       // -1 = não escolheu
    int jokmaq;

    int vitoria;      //1 - se saiu vitorioso
    int pendente;     //1 - resultado ainda nao foi aplicado pela jogatina
}EstadoMinigame;

extern EstadoMinigame minigame;

void Minigame(void);

//Comeca um minigame (1 = par/impar, 2 = jokempo), limpando os dados do anterior
void IniciarMinigame(int qual);

//Zera tudo (inclusive os "ja jogou"), chamar a cada partida nova
void ReiniciarMinigame(void);

#endif