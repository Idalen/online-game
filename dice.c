// Inclusao de bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

// Variaveis da logica do jogo
#define JOGADORES 4
#define CASAS 50
#define LADOS 6
#define CLOCK 200 // Tempo de espera

// Esse arquivo serve para suprir as informacoes do dado para o jogo
// Ele é independente, pode ate ser rodado por fora do socket, mas e essencial para o funcionamento do projeto como um todo
// O objetivo principal dele e rolar os dados de um certo usuario um numero de vezes N
int main()
{

    printf("Pressione uma tecla para rolar o dado:\n");
    char ch;
    ch = getchar();
    printf("%c\n", ch);

    srand(time(NULL));
    int vezes = (rand() % 10) + 5; // O dado sera lancado dentre 5 a 10 vezes

    srand(ch); // Com o char que o usuario digitou funcionando como seed

    for (int i = 0; i < vezes; i++)
    {

        system("clear");

        int dice = (rand() % LADOS) + 1;
        printf("%d\n", dice);

        usleep(CLOCK * 1000);
    }
}