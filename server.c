#include <stdio.h> 
#include <string.h> 
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define PORT 8000
#define MAXLINE 4096

#define JOGADORES 4
#define CASAS 50
#define LADOS 6

typedef struct user_info{
  char* client_adress;
  int client_socket;
  int user_id;
  int* pos;
  int* is_ready; 
  int* ready;
  int* round;
} user_info;

typedef struct package{
  int pos[4];
  int ready;
}package;

void *handle_connection();
int playersAreReady(int *players);
int playersArentReady(int *players);
int winner(int *players);

int main(){

  int  client_sock;
  struct sockaddr_in addr;
  int server_sock;
  server_sock = socket(AF_INET,SOCK_STREAM,0);

  struct sockaddr_in cli_addr;
  socklen_t cli_addr_len;
  char cli_address[MAXLINE+1];

  if(server_sock == -1){
    printf("Error creating socket\n");
    return 1;
  }

  addr.sin_family      = AF_INET;
  addr.sin_port        = htons(PORT);
  addr.sin_addr.s_addr = INADDR_ANY;
  memset(&addr.sin_zero,0,sizeof(addr.sin_zero));

  int yes=1;

  if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
      perror("setsockopt");
      exit(1);
  }

  if(bind(server_sock,(struct sockaddr*)&addr,sizeof(addr)) == -1){
    printf("Binding error\n");
    return 1;
  }

  if(listen(server_sock,JOGADORES) == -1){
    printf("Listening error!\n");
    return 1;
  }

  printf("Waiting client to connect on port %d...\n", PORT);
  
  
  int users = 0, round=0;
  int *pos = malloc(JOGADORES*sizeof(int)), *is_ready = malloc(JOGADORES*sizeof(int)), *ready = malloc(sizeof(int));
  memset(pos, 0, sizeof(int)*JOGADORES);
  memset(pos, 0, sizeof(int)*JOGADORES);
  *ready = 0;

  while(users < JOGADORES){

    client_sock = accept(server_sock, (struct sockaddr_in*)&cli_addr, &cli_addr_len);

    if(client_sock == -1){
      printf("Client not accepted!\n");
      return 1;
    }

    inet_ntop(AF_INET, &cli_addr, cli_address, MAXLINE);
    printf("Client %s connected\n", cli_address);

    user_info *p_info = malloc(sizeof(user_info));
    p_info->client_adress = cli_address;
    p_info->client_socket = client_sock;
    p_info->user_id       = users++;
    p_info->pos           = pos;
    p_info->is_ready      = is_ready;
    p_info->ready         = ready;
    p_info->round         = &round;

    pthread_t t;

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    pthread_create(&t, &attr, handle_connection, p_info);
  }

  printf("Jogo iniciado\n");

  // Runs while there is no winner
  while( winner(pos) == -1 ) {

    // Controls ready state
    if( playersAreReady(is_ready) ) {
      round++;
      *ready = 1;

    } else if( playersArentReady(is_ready) ) {

      *ready = 0;

    }

  }

  printf("Vencedor: %d\n", winner(pos));

  return 0;

}

void* handle_connection(void* p_info){
  
  user_info info = *(user_info*) p_info;
  free(p_info);

  char* client_adress = info.client_adress;
  int client_sock = info.client_socket;
  int id = info.user_id;
  int* pos = info.pos;
  int* is_ready = info.is_ready; 
  int* ready = info.ready;
  int* round = info.round;
  int byte_len, seed = 1, result;

  package p;
  
  byte_len = send(client_sock, &id, sizeof(int), 0);

  do
  {
    byte_len = recv(client_sock, &seed, sizeof(int), 0);
    
    srand(result*seed);
    result = rand()%6 + 1;

    is_ready[id] = 1;
    
    while( !(*ready) ) {

      // wait

    }
    
    pos[id]+=result;

    for(int i=0; i<JOGADORES; i++)
      p.pos[i] = pos[i];
    p.ready = *ready;

    byte_len = send(client_sock, &p, sizeof(package), 0);

    printf("Round %d:\n", *round);
    for(int i = 0; i<JOGADORES; i++)
      printf("Horse %d: pos %d \n", i, pos[i]);

    is_ready[id] = 0;

  }while (byte_len != (int) 'q');
  
}

int playersAreReady(int *players) {

  for(int i=0; i<JOGADORES; i++) {

    if( !players[i] ) return 0;

  }

  return 1;

}

int playersArentReady(int *players) {

  for(int i=0; i<JOGADORES; i++) {

    if( players[i] ) return 0;

  }

  return 1;

}

int winner(int *players) {

  for(int i=0; i<JOGADORES; i++) {

    if( players[i] >= CASAS ) return i;

  }

  return -1;

}