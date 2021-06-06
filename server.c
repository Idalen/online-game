#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


int main(){
    int    server_socket;
    int    client_socket;
    struct sockaddr_in addr;
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

    if(client_socket = accept(server_socket, (struct sockaddr*) &addr, (socklen_t*)&addrlen) < 0){
        printf("Client not accepted\n");
        return 1;
    }

    printf("Client connected!\n");

int received, sended;
char message[256];
char respond[256];

do { 

    received = recv(client_socket,respond,256,0);              /* Recebe mensagem do cliente */
    respond[received] = '\0';                                 /* Finaliza a string com o caractere NULO */
    printf("Cliente: %s\n",respond); 	       	              /* Mostra a mensagem do cliente */    
    printf("Servidor: ");                                       /* Simplesmente informa que deve-se preencher uma mensagem */
    fgets(message,256,stdin);                                  /* Obtém uma mensagem digitada */
    message[strlen(message)-1] = '\0';                        /* Finaliza a string */
    sended = send(client_socket,message,strlen(message),0);  /* Envia a string */

}while(sended != -1 ); /* ... enquanto as funções send() e recv() não retornarem -1 = ERRO */

close(client_socket);         
close(server_socket);      

return 0;
}