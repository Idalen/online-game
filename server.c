#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>

int client_socket;
struct sockaddr_in addr;
pthread_mutex_t mutexsum = PTHREAD_MUTEX_INITIALIZER; 
int done = 1;

//--------------------------------------------------------------------------------------------------//

void *sendmessage(){
    int  submitted;
    char message[256];

    do{  
      printf("Server: ");
      fgets(message,256,stdin);
      message[strlen(message)-1] = '\0';
      submitted = send(client_socket,message,strlen(message),0);
    }while(strcmp(message,"exit")!=0);
}

//--------------------------------------------------------------------------------------------------//

void *listener(){
    int received;
    char respond[256];
    
    do{
        received = recv(client_socket,respond, 256, 0);              /* Recebe mensagem do cliente */
        respond[received] = '\0';
        printf("Cliente: %s\n",respond);
    }while(strcmp(respond,"exit")!=0); 
    
    pthread_mutex_destroy(&mutexsum);
    pthread_exit(NULL);
    done=0;
}

//--------------------------------------------------------------------------------------------------//

int main(){
    int server_socket;
    int addrlen = sizeof(addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(4143);
    addr.sin_addr.s_addr = INADDR_ANY;
    memset(&addr.sin_zero, 0, sizeof(addr.sin_zero));

    if(bind(server_socket, (struct sockaddr*)&addr, sizeof(addr)) < 0){
        printf("Binding error.\n");
        return 1;
    }

    if(listen(server_socket, 4) < 0){
        printf("Listening error.\n");
        return 1;
    }

    printf("Waiting for client\n");
    if(client_socket = accept(server_socket, (struct sockaddr*) &addr, (socklen_t*)&addrlen) < 0){
        printf("Client not accepted\n");
        return 1;
    }

    printf("Client connected!\n");

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