#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[]){
  struct sockaddr_in server, client;
  int sockfd = -1, readsize, cli_sock;
  unsigned address_size;
  char buf[256];
  memset(&server, 0, sizeof(server));

  server.sin_family = PF_INET;
  server.sin_addr.s_addr = inet_addr("127.0.0.1"); //127.0.0.1
  server.sin_port = htons(9487);

  sockfd = socket(PF_INET, SOCK_STREAM, 0);
  bind(sockfd, (struct sockaddr*)&server, sizeof(server));
  listen(sockfd, 100);

  address_size = sizeof(client);

  cli_sock = accept(sockfd, (struct sockaddr*)&server,  &address_size);

  readsize = recv(cli_sock, buf, sizeof(buf), 0);

  buf[readsize] = '\0';

  puts(buf);

  close(sockfd);
}
