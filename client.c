#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

typedef struct in_addr{
    
} in_addr;

typedef struct socket_addr_in{
    short int sin_family;
    unsigned short int sin_port;
    in_addr sin_addr;
    unsigned char sin_zero[8];
} socket_addr_in;

int main(){
    int client_socket;
    socket_addr_in addr;

    client_socket   = socket(AF_INET, SOCK_STREAM, 0); //socket TCP
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(4143); 
    addr.sin_addr   = inet_addr(/*O que por aqui?*/); 
    memset(/*O que por aqui?*/)

}