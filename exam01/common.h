#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct {
  char host[16];
  int port;
  int sock;
  int closed;
  struct sockaddr_in handle;
} TCPSocket;

TCPSocket* tcpsock_setup(const char*, int);
void tcpsock_destroy(TCPSocket*);
int tcp_server(TCPSocket*);
int tcp_client(TCPSocket*);
TCPSocket* tcp_accept(TCPSocket*);
int tcp_read(TCPSocket*, char*, int);
int tcp_write(TCPSocket*, char*, int);
int tcp_close(TCPSocket*);
