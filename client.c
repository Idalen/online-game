#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#define PORT 8000

#define JOGADORES 2
#define CASAS 15
#define LADOS 6

typedef struct package{
  int pos[4];
  int ready;
  int win;
}package;

int main(){

int client_sock = socket(AF_INET,SOCK_STREAM,0);
int done=1; 
struct sockaddr_in addr;

void imprimirJogo(int *jogadores);

if(client_sock == -1){
    printf("Error creating socket\n");
    return 1;
}

addr.sin_family      = AF_INET;
addr.sin_port        = htons(PORT);
addr.sin_addr.s_addr = inet_addr("127.0.0.1");

memset(&addr.sin_zero,0,sizeof(addr.sin_zero));

printf("Connecting to server...\n");

if(connect(client_sock,(struct sockaddr*)&addr,sizeof(addr)) == -1){
  printf("Can't connect.\n");
  return 1;
}

printf("Client connected!\n");

int bytes_rec, bytes_send, id, pos=0, seed;
bytes_rec = recv(client_sock, &id, sizeof(int), 0);

printf("You are horse %d\n", id);

package p;
do
{
  printf("Insira tua jogada:\n");
  scanf("%d", &seed);
  
  bytes_send = send(client_sock, &seed, sizeof(int), 0);
  printf("Aguardando todos os jogadores...\n");
  bytes_rec = recv(client_sock, &p, sizeof(package), 0);
  
  // printf("Your current position is %d\n", pos);
  imprimirJogo(p.pos);

}while(p.win == -1);

if(p.win == id)
  printf("YOU WIN!!!!!!\n");
else
  printf("DERROTA\n");  

return 0;
}

void imprimirJogo(int *jogadores) {

  system("clear");

  for(int i=0; i<JOGADORES; i++) {

      for(int j=0; j<CASAS; j++) {

          if(jogadores[i] == j)
              printf("C%d", i);
          else
              printf("-");

      }

      printf("\n");

  }

}