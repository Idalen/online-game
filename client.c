#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(){

    int client_socket;
    struct sockaddr_in addr;

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

    int received, sended;
    char message[256];
    char respond[256];


    do {
  
        printf("Cliente: ");
        fgets(message,256,stdin);
        message[strlen(message)-1] = '\0';
        sended = send(client_socket, message, strlen(message),0);  


        received = recv(client_socket,respond,256,0);
        respond[received] = '\0';
        printf("Servidor: %s\n",respond);


    }while(sended != -1);


    return 0;

}