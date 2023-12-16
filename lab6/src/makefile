CC=gcc
CFLAGS= -I.

all : server client

server : server.c common.o common.h
	$(CC) -pthread -o server server.c common.o $(CFLAGS)

client : client.c common.o common.h
	$(CC) -pthread -o client client.c common.o $(CFLAGS)

common.o : common.h
	$(CC) -o common.o -c common.c $(CFLAGS)

clean :
	rm server client common.o
