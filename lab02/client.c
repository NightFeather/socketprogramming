#include "common.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
  int num = 0, res = 0;
  Socket* socket = socket_setup("127.0.0.1", 9487, udp);
  while(scanf("%d", &num) != EOF){
    num = htonl(num);
    socket_write(socket, (char*)&num, sizeof(num));
    {
      fd_set rfds;
      int ret = 0;

      FD_ZERO(&rfds);

      FD_SET(0, &rfds);
      FD_SET(socket->sock, &rfds);

      ret = select(socket->sock+1, &rfds, NULL, NULL, NULL);

      if(ret < 0){
        printf("Error!\n");
      } else {
        if(FD_ISSET(0, &rfds)){ continue; }
      }

    }
    socket_read(socket, (char*)&res, sizeof(res));
    res = ntohl(res);
    printf("%d\n", res);
  }

  socket_close(socket);
  socket_free(socket);
}