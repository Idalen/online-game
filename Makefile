all: build

build: clean client server

client:
	gcc client.c -o client -pthread

server:
	gcc server.c -o server -pthread

clean:
	rm -R client & rm -R server