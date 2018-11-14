
CFLAGS=-Wall -g
INC=-I.

default: server1 client1 server2 client2 server3 client3

client1: client1.o common.o
	gcc ${CFLAGS} ${INC} common.o client1.o -o client1

server1: server1.o common.o
	gcc ${CFLAGS} ${INC} common.o server1.o -o server1

client2: client2.o common.o
	gcc ${CFLAGS} ${INC} common.o client2.o -o client2

server2: server2.o common.o
	gcc ${CFLAGS} ${INC} common.o server2.o -o server2

client3: client3.o common.o
	gcc ${CFLAGS} ${INC} common.o client3.o -o client3

server3: server3.o common.o
	gcc ${CFLAGS} ${INC} common.o server3.o -o server3

common.o: common.c
	gcc ${CFLAGS} ${INC} -c common.c -o common.o

client1.o: client1.c
	gcc ${CFLAGS} ${INC} -c client1.c -o client1.o

server1.o: server1.c
	gcc ${CFLAGS} ${INC} -c server1.c -o server1.o

client2.o: client2.c
	gcc ${CFLAGS} ${INC} -c client2.c -o client2.o

server2.o: server2.c
	gcc ${CFLAGS} ${INC} -c server2.c -o server2.o

client3.o: client3.c
	gcc ${CFLAGS} ${INC} -c client3.c -o client3.o

server3.o: server3.c
	gcc ${CFLAGS} ${INC} -c server3.c -o server3.o

clean:
	rm server{1,2,3}.o server{1,2,3} client{1,2,3}.o client{1,2,3} common.o
