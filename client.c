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

int main(){

int client_sock = socket(AF_INET,SOCK_STREAM,0);
int done=1; 
struct sockaddr_in addr;

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

printf("Client connected!\n\n");

int bytes_rec, bytes_send, id, pos=0, seed;
bytes_rec = recv(client_sock, &id, sizeof(int), 0);

printf("You are horse %d\n", id);

while(1){
  do
  {
    scanf("%d", &seed);
    bytes_send = send(client_sock, &seed, sizeof(int), 0);

    bytes_rec = recv(client_sock, &pos, sizeof(int), 0);
    printf("Your current position is %d\n", pos);

  }while(bytes_rec != -1);

}
return 0;
}

