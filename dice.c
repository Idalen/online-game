#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define JOGADORES 4
#define CASAS 50
#define LADOS 6
#define CLOCK 200

int main() {

    printf("Pressione uma tecla para rolar o dado:\n");     
    char ch;
    ch = getchar();
    printf("%c\n", ch);     

    srand(time(NULL));
    int vezes = (rand() % 10) + 5;

    srand(ch);

    for(int i=0; i<vezes; i++) {

        system("clear");

        int dice = (rand() % LADOS) + 1;
        printf("%d\n", dice);
        
        usleep(CLOCK * 1000);

    }
  
}