#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(){
  struct sockaddr_in server, client;
  int sockfd = -1, readsize, address_size;
  char buf[256];
  memset(&server, 0, sizeof(server));
  server.sin_family = PF_INET;
  server.sin_addr.s_addr = inet_addr("127.0.0.1"); //127.0.0.1
  server.sin_port = htons(9487);
  sockfd = socket(PF_INET, SOCK_DGRAM, 0);
  bind(sockfd, (struct sockaddr*)&server, sizeof(server));

  address_size = sizeof(client);
  readsize = recvfrom(sockfd, buf, 256, 0, (struct sockaddr*)&client, &address_size);
  buf[readsize] = 0;
  printf("%s\n", buf);
  close(sockfd);
}
