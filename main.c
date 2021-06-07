#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define JOGADORES 4
#define CASAS 50
#define LADOS 6

int main() {

    srand(time(NULL));

    int winner = -1;
    int cavalos[JOGADORES];
    memset(cavalos, 0, JOGADORES*4);

    while( winner == -1 ) {

        printf("CORRIDA DE CAVALOS\n");

        for(int i=0; i<JOGADORES; i++) {

            for(int j=0; j<CASAS; j++) {

                if(cavalos[i] == j)
                    printf("C%d", i);
                else
                    printf("-");

            }

            int dice = (rand() % LADOS) + 1;
            cavalos[i] += dice;

            if( cavalos[i] >= CASAS )
                winner = i;

            printf("\n");

        }

        sleep(1);
        system("clear");

    }

    printf("VENCEDOR: %d\n", winner);

}