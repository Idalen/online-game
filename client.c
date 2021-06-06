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

void* sendMessage();

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


char message[] = "HELLO THERE!\0";

while(send(client_sock, message, strlen(message), 0) > 0){
    sleep(1);
}

return 0;

}

