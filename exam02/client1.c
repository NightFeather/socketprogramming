#include "common.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
  char buffer[512] = {0};
  int buf_size = 0;
  Socket* socket = socket_setup("127.0.0.1", 9487, udp);
  while((buf_size = read(0, buffer, sizeof(buffer))) > 0){

    // trim trailing '\n'
    for(buf_size--; buf_size > 0 && buffer[buf_size] != '\n' && buffer[buf_size]; buf_size--);
    buffer[buf_size] = 0;

    socket_write(socket, buffer, strlen(buffer));

    // select preparation
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

    buf_size = socket_read(socket, buffer, sizeof(buffer));
    buffer[buf_size] = 0;
    puts(buffer);

  }

  socket_close(socket);
  socket_free(socket);
}