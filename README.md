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

O jogo aceita exatamente 4 jogadores. O servidor trata o gerenciamento das threads e os inputs de cada jogador.

## 2.2 Sistema Operacional e Informações Técnicas

Produzido no Linux, Kernel: 5.9.16-1-MANJARO x86_64

Feito em Linguagem C, com compilador GCC

## 2.3 Conexão e transmissão de informações

Para a transmissão das informações, foi usado o protocolo TCP/IP
