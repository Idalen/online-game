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

int  client_sock;
struct sockaddr_in addr;
void *sendmessage();
void *listener();
void *handle_connection();
int done=1; 

int main(){

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

if(bind(server_sock,(struct sockaddr*)&addr,sizeof(addr)) == -1){
  printf("Binding error\n");
  return 1;
}

if(listen(server_sock,4) == -1){
  printf("Listening error!\n");
  return 1;
}

while(1){
  printf("Waiting client to connect on port %d...\n", PORT);

  client_sock = accept(server_sock, (struct sockaddr_in*)&cli_addr, &cli_addr_len);

  if(client_sock == -1){
    printf("Client not accepted!\n");
    return 1;
  }

  inet_ntop(AF_INET, &cli_addr, cli_address, MAXLINE);
  printf("Client %s connected\n", cli_address);

  void* retval;
  int* pclient = malloc(sizeof(int));
  *pclient = client_sock;

  pthread_t t;
  
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  pthread_create(&t, &attr, handle_connection, pclient);
}


        
return 0;

}

void* handle_connection(void* p_client_sock){
  int client_sock = *(int*)p_client_sock;
  free(p_client_sock);
  char txt[14];

  while(recv(client_sock, txt, 14, 0)>0){
    printf("%s%d\n", txt, client_sock);
  }
}
