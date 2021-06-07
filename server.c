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

typedef struct user_info{
  char* client_adress;
  int client_socket;
  int user_id;
  int* pos;
  int* ready; 
} user_info;

void *handle_connection();

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

  if(listen(server_sock,4) == -1){
    printf("Listening error!\n");
    return 1;
  }

  printf("Waiting client to connect on port %d...\n", PORT);
  int users = 0;
  int *pos = malloc(4*sizeof(int)), *ready = malloc(4*sizeof(int));

  while(1){

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
    p_info->user_id       = ++users;
    p_info->pos           = pos;
    p_info->ready         = ready;

    pthread_t t;

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    pthread_create(&t, &attr, handle_connection, p_info);
  }


  return 0;

}

void* handle_connection(void* p_info){
  
  int byte_rec, byte_send, seed = -1, result, user_pos = 0;
  
  user_info info = *(user_info*) p_info;
  free(p_info);

  char* client_adress = info.client_adress;
  int client_sock = info.client_socket;
  int user_id = info.user_id;
  int* pos = info.pos;
  int* ready = info.ready;   
  
  
  byte_send = send(client_sock, &client_sock, sizeof(int), 0);

  do
  {
    byte_rec = recv(client_sock, &seed, sizeof(int), 0);
    
    srand(seed);
    result = rand()%6 + 1;
    user_pos+=result;

    byte_send = send(client_sock, &user_pos, sizeof(int), 0);
    printf("Client %d new pos = %d, seed = %d, dice_result = %d\n", client_sock, user_pos, seed, result);

  }while (byte_rec != (int) 'q');
  
}
