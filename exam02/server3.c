#include "common.h"
#include <stdio.h>
#include <stdlib.h>

void serve(Socket* sock){
  int buf_size = 0, state = 0;
  char buffer[512] = {0};
  while(1){
    if(!state){
      // reading request
      buf_size = socket_read(sock, buffer, sizeof(buffer));
      if(buf_size <= 0){
        puts("client disconnected.");
        break;
      }
      buffer[buf_size] = 0;
      puts(buffer);
      state = 1;

    } else {
      int ret;
      ret = socket_wait_read(sock, 3);

      if( ret < 0){
        puts("something wrong.");
        exit(-1);
      } else if ( ret == 1 ) {
        // sock is ready
        int tmp = 0;
        buffer[buf_size++] = '&';
        tmp = socket_read(sock, buffer+buf_size, sizeof(buffer));

        if(tmp <= 0){
          puts("client disconnected.");
          break;
        }

        buf_size += tmp;
        puts(buffer);
      } else {
        puts("timeout");
      }

      socket_write(sock, buffer, strlen(buffer));

      // clear res, and clear state
      state = 0;
    }
  }
}

int main(){
  Socket* socket = NULL;

  printf("0:TCP, 1:UDP> ");

  {
    int choice;
    scanf("%d", &choice);
    if(choice < 0 || choice > 1){
      puts("I dont understand.");
      return -1;
    }

    socket = socket_setup("127.0.0.1", 9487, choice);
  }

  socket_listen(socket);
  socket_serve(socket, serve);
  socket_close(socket);
  socket_free(socket);
}
