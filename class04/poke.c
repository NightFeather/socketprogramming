#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]){
  struct addrinfo* ai = NULL;
  char buffer[256] = {0};
  int gai_res = 0;
  if(argc < 2){
    printf("Usage: %s <host>\n", argv[0]);
    return -1;
  }

  gai_res = getaddrinfo(argv[1], argv[2], NULL, &ai);

  if(gai_res != 0){
    printf("%s \n", gai_strerror(gai_res));
    return -1;
  }

  while(ai != NULL){
    if(ai->ai_family == AF_INET){
      strcpy(buffer, inet_ntoa(((struct sockaddr_in*)(ai->ai_addr))->sin_addr));
    } else {
      inet_ntop(AF_INET6, &((struct sockaddr_in6*)(ai->ai_addr))->sin6_addr, buffer, sizeof(struct sockaddr_in6));
    }

    if(ai->ai_protocol == IPPROTO_IP){
      printf("%s\n", buffer);
    }

    ai = ai->ai_next;
  }
}
