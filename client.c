// Inclusao de bibliotecas
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

// Porta usada
#define PORT 8000

// Variaveis da logica do jogo
#define JOGADORES 4
#define CASAS 30
#define LADOS 6

// Struct do package, contendo:
//  - int[4] para as 4 posicoes de cada um dos jogadores
//  - int ready que serve como flag de prontidao
//  - int win que demarca vitoria no jogo
typedef struct package
{
  int pos[4];
  int ready;
  int win;
} package;

// Funcao de imprimir os detalhes do jogo na tela para o cliente
//  INPUT: void
//  OUTPUT: void (jogo-cliente eh tratado inteiramente dentro da main)
int main()
{
  // Inicializando as variaveis para conexao
  int client_sock = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in addr;
  int done = 1;

  void imprimirJogo(int *jogadores);

  if (client_sock == -1) // Erro no socket
  {
    printf("Error creating socket\n");
    return 1;
  }

  // Atribuindo valores para as variaveis do address
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  memset(&addr.sin_zero, 0, sizeof(addr.sin_zero));

  printf("Connecting to server...\n");

  if (connect(client_sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) // Erro na conexao com servidor
  {
    printf("Can't connect.\n");
    return 1;
  }

  printf("Client connected!\n");

  // Inicializando as variaveis usadas no jogo e na comunicacao cliente-servidor
  int bytes_rec, bytes_send, id, pos = 0, seed;
  bytes_rec = recv(client_sock, &id, sizeof(int), 0);

  printf("You are horse %d\n", id);

  package p; // Package que sera enviado back-and-forth em cliente->servidor
  do
  {
    printf("Insira tua jogada:\n");
    scanf("%d", &seed); // Cada valor enviado sera usado como seed no random

    bytes_send = send(client_sock, &seed, sizeof(int), 0); // Envia sua jogada
    printf("Aguardando todos os jogadores...\n");
    bytes_rec = recv(client_sock, &p, sizeof(package), 0); // Recupera a resposta

    // printf("Your current position is %d\n", pos);
    imprimirJogo(p.pos); // Atualiza a tela com os dados dos players

  } while (p.win == -1); // Enquanto o int win do package p nao significar vitoria, continue rodando

  if (p.win == id) // Caso o player ganhe
    printf("YOU WIN!!!!!!\n");
  else
    printf("DERROTA\n");

  return 0;
}

// Funcao de imprimir os detalhes do jogo na tela
//  INPUT: vetor de jogadores
//  OUTPUT: void (os prints sao feitos dentro da funcao)
void imprimirJogo(int *jogadores)
{

  system("clear");

  for (int i = 0; i < JOGADORES; i++) // Uma pista de corrida para cada jogador
  {

    for (int j = 0; j < CASAS; j++) // Varias casas por pista de corrida
    {

      if (jogadores[i] == j) // Se a posicao bate com a ocupada pelo jogador i
        printf("C%d", i);    // Printa o cavalo
      else
        printf("-"); // Printa espaco em branco
    }

    printf("\n");
  }
}