#include <sys/socket.h>
#include <stdio.h>

int main(){
  printf("base: %d\n", sizeof(struct sockaddr));
  printf("ipv4: %d\n", sizeof(struct sockaddr_in));
  printf("ipv6: %d\n", sizeof(struct sockaddr_in6));
}
