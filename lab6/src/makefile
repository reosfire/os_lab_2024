CC=gcc
CFLAGS=-I.

all : server client

server : common.o server.c
	$(CC) -o server common.o server.c $(CFLAGS)

client : common.o client.c
	$(CC) -o client common.o client.c $(CFLAGS)

common.o : common.c
	$(CC) -o common.o -c common.c $(CFLAGS)

clean :
	rm server client common.o
