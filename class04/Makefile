
CFLAGS=-Wall -g
INC=-I.

default: server1 client1 server2 client2 server3 client3

client1: client1.o common.o
	gcc ${CFLAGS} ${INC} common.o client1.o -o client1

server1: server1.o common.o
	gcc ${CFLAGS} ${INC} common.o server1.o -o server1

common.o: common.c
	gcc ${CFLAGS} ${INC} -c common.c -o common.o

client1.o: client1.c
	gcc ${CFLAGS} ${INC} -c client1.c -o client1.o

server1.o: server1.c
	gcc ${CFLAGS} ${INC} -c server1.c -o server1.o

clean:
	rm server{1,2,3}.o server{1,2,3} client{1,2,3}.o client{1,2,3} common.o
