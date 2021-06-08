# Trabalho 1 de Redes de Computadores (SSC0142)

Andre Santana Fernandes - 11208537

Daniel Martins Vieira - 11215719

Diogo Castanho Emídio - 11297274

Leonardo Antonetti da Motta - 11275338

Marcus Vinicius Santos Rodrigues - 11218862

# 1. Introdução

## 1.1 Sobre o trabalho

O trabalho funciona atualmente como um jogo randomizado de dados para corrida de cavalos. Como jogador, você insere uma *seed* para seu dado ser randomizado, sendo que esse define quanto seu cavalo irá avançar na pista de corrida. O primeiro cavalo que chegar ao final ganha o jogo, terminando-o.

## 1.2 Como instalar

1. Fazer o download do trabalho (através do botão _Code_);
2. Digitar os comandos `make server` e `make client` no terminal para compilar o jogo;
3. Digitar os comandos `./server` e `./client` no terminal para iniciar o jogo.

# 2. Descrição do Software

## 2.1 Funcionamento do jogo

O jogo necessita de 4 clientes conectados para iniciar. Para cada cliente, o servidor cria uma nova thread para lidar com cada cliente individualmente, enquanto a thread main lida com informações compartilhadas entre as threads.
O jogo consiste em uma corrida de cavalos -- cada cavalo sendo um usuário. O usuário precisa inserir um número inteiro pra realizar sua jogada, que será o lançamento virtual de um dado. O cavalo que chegar à linha final primeiro ganha o jogo e o programa é encerrado (ainda com alguns bugs).

## 2.2 Sistema Operacional e Informações Técnicas

Todo o código foi desenvolvido e testado no 21.0.6 Manjaro Linux e Ubuntu LTS 20.04, usando o compilador GCC.

