#include "common.h"
#include <stdio.h>

int main(){
  TCPSocket* sock = tcpsock_setup("127.0.0.1", 9487);
  char buffer[256] = {0};
  int read_size = 0;
  if(tcp_client(sock) == -1){
    puts("Cannot connect to server!");
    return 1;
  }

  while(1){
    read_size = read(0, buffer, sizeof(buffer));
    if(read_size <= 0){ break; }

    tcp_write(sock, buffer, strlen(buffer));

    read_size = tcp_read(sock, buffer, sizeof(buffer));
    if(read_size <= 0){ break; }
    buffer[read_size] = '\0';
    puts(buffer);

  }

  tcp_close(sock);
  tcpsock_destroy(sock);
}
