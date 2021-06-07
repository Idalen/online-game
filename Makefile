all: build

build: client server

client:
	gcc client.c -o client -pthread & ./client

server:
	gcc server.c -o server -pthread & ./server

clean:
	rm -R client & rm -R server