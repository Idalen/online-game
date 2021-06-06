#include <stdio.h> 
#include <string.h> 
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <string.h>

#define PORT 8000
#define MAXLINE 4096

int  client_sock;
struct sockaddr_in addr;
void *sendmessage();
void *listener();
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

if(listen(server_sock,1) == -1){
  printf("Listening error!\n");
  return 1;
}

printf("Waiting client to connect on port %d...\n", PORT);

client_sock = accept(server_sock, (struct sockaddr_in*)&cli_addr, &cli_addr_len);

if(client_sock == -1){
  printf("Client not accepted!\n");
  return 1;
}

inet_ntop(AF_INET, &cli_addr, cli_address, MAXLINE);
printf("Client %s connected\n", cli_address);

pthread_t threads[2];

void *status;

pthread_attr_t attr;
pthread_attr_init(&attr);
pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

pthread_create(&threads[0], &attr, sendmessage, NULL);
pthread_create(&threads[1], &attr, listener, NULL);



while(done){}
        
return 0;

}

void *sendmessage(){
    int  sended;
    char msg[256];

    do
    {  
        printf("Server: ");
        fgets(msg,256,stdin);
        msg[strlen(msg)-1] = '\0';
        sended = send(client_sock,msg,strlen(msg),0);

    }while(strcmp(msg,"exit")!=0);
}

void *listener(){
    int received;
    char answer[256];
    do
    {
        received = recv(client_sock,answer,256,0);              /* Recebe mensagem do cliente */
        answer[received] = '\0';
        printf("Cliente: %s\n",answer);
    }while(strcmp(answer,"exit")!=0); 
    	
    pthread_exit(NULL);
    done=0;
}
