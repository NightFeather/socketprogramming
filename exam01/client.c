#include "common.h"
#include <stdio.h>

int main(){
  char buffer[256] = {0};
  int read_size = 0;
  char host[32] = {0};
  int port = -1;

  scanf("%s %d", host, &port);

  TCPSocket* sock = tcpsock_setup(host, port);
  if(tcp_client(sock) == -1){
    puts("Cannot connect to server!");
    return 1;
  }

  while(1){
    read_size = read(0, buffer, sizeof(buffer));
    if(read_size < 0){ break; }
    if(read_size == 0) { 
      tcp_write(sock, "\0", 1);
      break;
    }

    buffer[read_size] = 0;

    tcp_write(sock, buffer, read_size);

    read_size = tcp_read(sock, buffer, sizeof(buffer));
    if(read_size <= 0){ break; }
    buffer[read_size] = '\0';
    puts(buffer);

  }

  tcp_close(sock);
  tcpsock_destroy(sock);
}
