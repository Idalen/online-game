# Trabalho 1 de Redes de Computadores (SSC0142)

Andre Santana Fernandes - 11208537

Daniel Martins Vieira - 11215719

Diogo Castanho Emídio - 11297274

Leonardo Antonetti da Motta - 11275338

Marcus Vinicius Santos Rodrigues - 11218862

# 1. Introdução

## 1.1 Sobre o trabalho

O presente trabalho é um jogo randomizado de dados para corrida de cavalos. O jogador insere uma *seed* para seu dado ser randomizado, sendo que esse define quanto seu cavalo irá avançar na pista de corrida. O primeiro cavalo a chegar ao final ganha o jogo, terminando-o.

## 1.2 Como instalar

1. Fazer o download do trabalho (através do botão _Code_);
2. Digitar os comandos `make server` e `make client` no terminal para compilar o jogo;
3. Digitar os comandos `./server` e `./client` no terminal para iniciar o jogo.

# 2. Descrição do Software

## 2.1 Funcionamento do jogo

O jogo necessita de 4 clientes conectados para iniciar. O servidor cria uma *thread* para lidar com cada cliente individualmente, enquanto a *main* trabalha com informações compartilhadas entre as individuais.

O usuário, por sua vez, corresponde a um cavalo dentro do jogo e precisa inserir um número inteiro pra realizar sua jogada, que será o lançamento de um dado virtual. O cavalo que chegar à linha final primeiro ganha o jogo e o programa é encerrado.

## 2.2 Informações Técnicas

Todo o código foi desenvolvido e testado em Manjaro Linux 21.0.6 e Ubuntu LTS 20.04, usando o compilador GCC.
