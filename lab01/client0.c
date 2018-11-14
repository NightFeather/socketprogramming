#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(){
  struct sockaddr_in server;
  int sockfd = -1, readsize;
  char input[256] = {0};
  memset(&server, 0, sizeof(server));
  server.sin_family = PF_INET;
  server.sin_addr.s_addr = inet_addr("127.0.0.1"); //127.0.0.1
  server.sin_port = htons(9487);
  sockfd = socket(PF_INET, SOCK_STREAM, 0);
  connect(sockfd, (struct sockaddr*)&server, sizeof(server));

  readsize = read(0,input,256);

  if(input[readsize-1] == '\n') { input[readsize-1] = '\0'; }

  send(sockfd, input, readsize, 0);

  close(sockfd);
}
