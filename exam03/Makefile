LDFLAGS=-lpthread
CFLAGS=-Wall -g
INC=-I.

default: server client

client: client.o common.o
	gcc ${CFLAGS} ${INC} ${LDFLAGS} common.o client.o -o client

server: server.o common.o
	gcc ${CFLAGS} ${INC} ${LDFLAGS} common.o server.o -o server

common.o: common.c
	gcc ${CFLAGS} ${INC} -c common.c -o common.o

client.o: client.c
	gcc ${CFLAGS} ${INC} -c client.c -o client.o

server.o: server.c
	gcc ${CFLAGS} ${INC} -c server.c -o server.o

clean:
	rm server.o server client.o client common.o
