#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct {
  enum { tcp, udp } type;
  char host[16];
  int port;
  int sock;
  struct sockaddr_in handle;
} Socket;

Socket* socket_setup(char*, int, int);

// tcp only
int socket_listen(Socket*);
Socket* socket_accept(Socket*);
int sokcet_connect(Socket*);

int socket_write(Socket*, char*, int);
int socket_read(Socket*, char*, int);

int socket_close(Socket*);
void socket_free(Socket*);

int fd_wait_read(int, int);
int fd_wait_write(int, int);
int socket_wait_read(Socket*, int);
int socket_wait_write(Socket*, int);
