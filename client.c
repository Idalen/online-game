#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>

int client_socket;
pthread_mutex_t mutexsum = PTHREAD_MUTEX_INITIALIZER;  
struct sockaddr_in addr;
int done = 1;

//--------------------------------------------------------------------------------------------------//

void *sendmessage(){
    int  submitted;
    char message[256];
    do{  
        printf("Cliente: ");
        fgets(message,256,stdin);
        message[strlen(message)-1] = '\0';
        submitted = send(client_socket ,message,strlen(message),0);

    }while(strcmp(message,"exit")!=0);

	pthread_mutex_destroy(&mutexsum);
	pthread_exit(NULL);
	close(client_socket);         
	done=0;
}

//--------------------------------------------------------------------------------------------------//

void *listener(){
    int received;
    char respond[256];
    do{
        received = recv(client_socket,respond,256,0);
        respond[received] = '\0';
        printf("Servidor: %s\n",respond);
    }while(received != -1); 
}

//--------------------------------------------------------------------------------------------------//

int main(){

    client_socket   = socket(AF_INET, SOCK_STREAM, 0); //socket TCP

    addr.sin_family         = AF_INET;
    addr.sin_port           = htons(4143); 
    addr.sin_addr.s_addr    = inet_addr("127.0.0.1"); // servidor local
    memset(&addr.sin_zero, 0, sizeof(addr.sin_zero));

    printf("Connectiong to server...\n");

    if(connect(client_socket,(struct sockaddr_in*)& addr, sizeof(addr)) < 0){
        printf("Connection error... Try again later.\n");
        return 1;
    }

    printf("Connected!\n\n");

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

