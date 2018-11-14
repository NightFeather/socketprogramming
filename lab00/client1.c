#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(){
  struct sockaddr_in server;
  int sockfd = -1, read_chars = 0;
  char buf[] = "D0671296";
  char input[256] = {0};

  memset(&server, 0, sizeof(server));
  server.sin_family = PF_INET;
  server.sin_addr.s_addr = inet_addr("127.0.0.1"); //127.0.0.1
  server.sin_port = htons(9487);

  sockfd = socket(PF_INET, SOCK_DGRAM, 0);

  
  
  
  
  
  sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr*)&server, sizeof(server));

  system("stty -icanon");

  while(1){
    read_chars = read(0,input, 255);
    if(read_chars == 0){ break; }
    input[read_chars] = 0;
    sendto(sockfd, input, strlen(input), 0, (struct sockaddr*)&server, sizeof(server));
  }

  system("stty +icanon");
  close(sockfd);
}
