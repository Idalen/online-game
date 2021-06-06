#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <string.h>

int client_sock;
pthread_mutex_t mutexsum = PTHREAD_MUTEX_INITIALIZER;  
struct sockaddr_in addr;
void *sendmessage();
void *listener();
int done=1; 

int main(){

client_sock = socket(AF_INET,SOCK_STREAM,0);

if(client_sock == -1){
    printf("Error creating socket\n");
    return 1;
}

addr.sin_family      = AF_INET;
addr.sin_port        = htons(1234);
addr.sin_addr.s_addr = inet_addr("127.0.0.1");

memset(&addr.sin_zero,0,sizeof(addr.sin_zero));

printf("Connecting to server...\n");

if(connect(client_sock,(struct sockaddr*)&addr,sizeof(addr)) == -1){
  printf("Can't connect.\n");
  return 1;
}

printf("Client connected!\n\n");

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

    do{  
        printf("Cliente: ");
        fgets(msg,256,stdin);
        msg[strlen(msg)-1] = '\0';
        sended = send(client_sock,msg,strlen(msg),0);

    }while(strcmp(msg,"exit")!=0);

    pthread_mutex_destroy(&mutexsum);
    pthread_exit(NULL);
    close(client_sock);         
    done=0;
}

void *listener(){
    int received;
    char answer[256];

    do{

        received = recv(client_sock,answer,256,0);
        answer[received] = '\0';
        printf("\n Servidor: %s\n",answer);

    }while(received != -1); 
}
