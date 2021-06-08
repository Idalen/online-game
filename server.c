// Inclusao de bibliotecas
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// Porta usada e Maxline de comunicacao
#define PORT 8000
#define MAXLINE 4096

// Variaveis da logica do jogo
#define JOGADORES 4
#define CASAS 30
#define LADOS 6

// Struct de user_info, contendo:
//    char *client_adress com o endereco do cliente
//    int client_socket com o socket do cliente
//    int user_id que serve para identificar o cliente
//    int *pos que eh a posicao do cliente na pista de corrida
//    int *is_ready eh um vetor que informa se um dado cliente esta pronto (lancou o dado e esta esperando os outros) 
//    int *ready que eh uma flag pra indicar se todos os jogadores estão prontos ou n (poupa ler todo o vetor ready)
//    int *round que informa o round que o cliente esta
//    int *win que informa se o cliente ganhou
typedef struct user_info
{
  char *client_adress;
  int client_socket;
  int user_id;
  int *pos;
  int *is_ready;
  int *ready;
  int *round;
  int *win;
} user_info;

// Struct do package, contendo:
//    int pos[4] para as 4 posicoes de cada um dos jogadores
//    int ready que serve como flag de prontidao
//    int win que demarca vitoria no jogo
typedef struct package
{
  int pos[4];
  int ready;
  int win;
} package;

// Funcoes que sao usadas
void *handle_connection();
int playersAreReady(int *players);
int playersArentReady(int *players);
int winner(int *players);

// Funcao main de gerenciamento de conexoes e do jogo
//    INPUT: void
//    OUTPUT: void (jogo-servidor eh tratado inteiramente dentro da main)
int main()
{
  // Inicializando as variaveis para conexao do servidor
  int client_sock;
  struct sockaddr_in addr;
  int server_sock;
  server_sock = socket(AF_INET, SOCK_STREAM, 0);

  // Inicializando as variaveis para conexao de um dado cliente
  struct sockaddr_in cli_addr;
  socklen_t cli_addr_len;
  char cli_address[MAXLINE + 1];

  if (server_sock == -1)
  {
    printf("Error creating socket\n");
    return 1;
  }

  // Atribuindo valores para as variaveis do address
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = INADDR_ANY;
  memset(&addr.sin_zero, 0, sizeof(addr.sin_zero));

  int yes = 1; // Flag

  // Verificacoes de erros conexao. Tanto de setsocket(), bind() e listen()
  if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
  {
    perror("setsockopt");
    exit(1);
  }
  if (bind(server_sock, (struct sockaddr *)&addr, sizeof(addr)) == -1)
  {
    printf("Binding error\n");
    return 1;
  }
  if (listen(server_sock, JOGADORES) == -1)
  {
    printf("Listening error!\n");
    return 1;
  }

  printf("Waiting client to connect on port %d...\n", PORT);

  // Inicializando as variaveis usadas no gerenciamento do jogo
  int users = 0, round = 0;
  int *pos = malloc(JOGADORES * sizeof(int)), *is_ready = malloc(JOGADORES * sizeof(int)), *ready = malloc(sizeof(int)), *win = malloc(sizeof(int));
  memset(pos, 0, sizeof(int) * JOGADORES);
  memset(pos, 0, sizeof(int) * JOGADORES);
  *ready = 0;
  *win = -1;

  while (users < JOGADORES) // Enquanto os usuarios registrados nao bater o maximo de jogadores aceitos no jogo...
  {

    client_sock = accept(server_sock, (struct sockaddr_in *)&cli_addr, &cli_addr_len);

    if (client_sock == -1) // Erro no cliente
    {
      printf("Client not accepted!\n");
      return 1;
    }

    inet_ntop(AF_INET, &cli_addr, cli_address, MAXLINE);
    printf("Client %s connected\n", cli_address);

    // Atribuindo valores para as variaveis da struct de user_info desse dado cliente
    user_info *p_info = malloc(sizeof(user_info));
    p_info->client_adress = cli_address;
    p_info->client_socket = client_sock;
    p_info->user_id = users++;
    p_info->pos = pos;
    p_info->is_ready = is_ready;
    p_info->ready = ready;
    p_info->round = &round;
    p_info->win = win;

    // Atribuindo valores para a thread desse usuario
    pthread_t t;
    pthread_attr_t attr;

    // Inicializando a thread desse usuario
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&t, &attr, handle_connection, p_info);
  }

  printf("Jogo iniciado\n");

  // Roda enquanto nao ha vencedor
  do
  {

    // Controla estado ready dos jogadores
    if (playersAreReady(is_ready) && *ready == 0)
    {
      round++;
      *ready = 1;
    }
    else if (playersArentReady(is_ready) && *ready == 1)
    {

      *ready = 0;
    }

    *win = winner(pos);

  } while (*win == -1);

  // Prints de fim de jogo
  printf("Winner: %d\n", *win);
  printf("Terminating the game");
  for (int i = 0; i < 3; i++)
  {
    printf(".");
    sleep(3);
  }
  printf("\n");

  // Limpeza de memoria
  free(pos);
  free(is_ready);
  free(ready);
  free(win);

  return 0;
}

// Funcao para gerenciamento da conexao entre servidor-cliente
//    INPUT: uma struct user_info com as informacoes de um dado cliente
//    OUTPUT: void (todo o gerenciamento eh interno aa funcao)
void *handle_connection(void *p_info)
{

  // Transfere as informacoes de p_info para uma struct user_info interna aa funcao, e limpa p_info para um novo uso na main
  user_info info = *(user_info *)p_info;
  free(p_info);

  // Atribuindo valores de p_info para as variaveis internas da funcao
  char *client_adress = info.client_adress;
  int client_sock = info.client_socket;
  int id = info.user_id;
  int *pos = info.pos;
  int *is_ready = info.is_ready;
  int *ready = info.ready;
  int *round = info.round;
  int byte_len, seed = 1, result;
  int *win = info.win;

  package p;

  byte_len = send(client_sock, &id, sizeof(int), 0);

  do
  {
    byte_len = recv(client_sock, &seed, sizeof(int), 0);

    // Randomiza o valor do dado do cliente com base na seed que ele propos
    srand((result + client_sock) * seed);
    result = rand() % 6 + 1;

    is_ready[id] = 1;
    pos[id] += result;

    while (!(*ready))
    {
      // Espera o ready
    }

    // Processa as mudancas do jogo: posicao, se esta pronto, e se venceu
    for (int i = 0; i < JOGADORES; i++)
      p.pos[i] = pos[i];
    p.ready = *ready;
    p.win = *win;

    byte_len = send(client_sock, &p, sizeof(package), 0);

    // Printa as informacoes na tela para informar o cliente
    printf("Round %d:\n", *round);
    for (int i = 0; i < JOGADORES; i++)
      printf("Horse %d: pos %d \n", i, pos[i]);

    is_ready[id] = 0;

  } while (*win == -1); // Enquanto nao houver a flag de vitoria para esse cliente
}

// Funcao para checagem se os jogadores estao prontos
//    INPUT: vetor de players
//    OUTPUT: 1 se estao prontos, 0 se nao estao prontos
int playersAreReady(int *players)
{

  for (int i = 0; i < JOGADORES; i++)
  {

    if (!players[i])
      return 0;
  }

  return 1;
}

// Funcao para checagem se os jogadores nao estao prontos (ela so existe para facilitar entendimento no codigo da main)
//    INPUT: vetor de players
//    OUTPUT: 0 se estao prontos, 1 se nao estao prontos
int playersArentReady(int *players)
{

  for (int i = 0; i < JOGADORES; i++)
  {
    if (players[i])
      return 0;
  }

  return 1;
}

// Funcao para definicao do vencedor do jogo
//    INPUT: vetor de players
//    OUTPUT: -1 se nao houver vencedor, [i] se houver (onde [i] eh o jogador vencedor)
int winner(int *players)
{

  for (int i = 0; i < JOGADORES; i++)
  {
    if (players[i] >= CASAS)
      return i;
  }

  return -1;
}