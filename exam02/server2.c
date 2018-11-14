#include "common.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
  int buf_size = 0, state = 0;
  char buffer[1024] = {0};
  Socket* sock = socket_setup("127.0.0.1", 9487, udp);
  socket_listen(sock);

  while(1){
    if(!state){

      // reading request
      buf_size = socket_read(sock, buffer, sizeof(buffer));
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
        // socket is ready
        int tmp = 0;
        buffer[buf_size++] = '&';
        tmp = socket_read(sock, buffer+buf_size, sizeof(buffer));
        buf_size += tmp;
        puts(buffer);
      } else {
        puts("timeout");
      }

      ret = socket_wait_read(sock, 3);

      if( ret < 0){
        puts("something wrong.");
        exit(-1);
      } else if ( ret == 1 ) {
        // socket is ready
        int tmp = 0;
        buffer[buf_size++] = '&';
        tmp = socket_read(sock, buffer+buf_size, sizeof(buffer));
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
