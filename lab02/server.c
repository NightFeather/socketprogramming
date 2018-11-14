#include "common.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
  int num = 0, res = 1, flag = 0;
  Socket* sock = socket_setup("127.0.0.1", 9487, udp);
  socket_listen(sock);

  while(1){
    if(!flag){

      // reading request
      socket_read(sock, (char*)&num, sizeof(num));
      num = ntohl(num);
      printf("%d\n", num);

      // multiply request into res, res should be 1 atm
      res *= num;
      flag = 1;

    } else {
      int ret;
      ret = socket_wait_read(sock, 10);
      num = 1;

      if( ret < 0){
        puts("something wrong.");
        exit(-1);
      } else if ( ret == 1 ) {
        // socket is ready
        socket_read(sock, (char*)&num, sizeof(num));
        num = ntohl(num);
        res *= num;
      } else {
        puts("timeout");
      }

      // set res back
      res = htonl(res);
      socket_write(sock, (char*)&res, sizeof(res));

      // clear res, and clear flag
      res = 1;
      flag = 0;
    }
  }
}
