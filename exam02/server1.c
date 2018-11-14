#include "common.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
  int buf_size = 0, state = 0;
  char buffer1[512] = {0}, buffer2[512] = {0}, buffer3[1024] = {0};
  Socket* sock = socket_setup("127.0.0.1", 9487, udp);
  socket_listen(sock);

  while(1){
    if(!state){

      // reading request
      buf_size = socket_read(sock, buffer1, sizeof(buffer1));
      buffer1[buf_size] = 0;
      puts(buffer1);
      state = 1;

    } else {
      int ret;
      ret = socket_wait_read(sock, 5);

      if( ret < 0){
        puts("something wrong.");
        exit(-1);
      } else if ( ret == 1 ) {
        // socket is ready
        socket_read(sock, buffer2, sizeof(buffer2));
        puts(buffer2);
        sprintf(buffer3, "%s&%s", buffer2, buffer1);
      } else {
        puts("timeout");
        sprintf(buffer3, "%s", buffer1);
      }

      socket_write(sock, buffer3, strlen(buffer3));

      // clear res, and clear state
      state = 0;
    }
  }
}
