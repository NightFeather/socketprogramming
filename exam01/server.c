#include "common.h"
#include <stdio.h>

/* 
 * 0 1 2 3 4 5 6
 * 0 1 1 2 2 2 2
 */

void winner_sort(int arr[], int size){
  for(int leaf = size-1; leaf > 0; leaf--){
    for(int pin = leaf; pin > 0; pin--){
      if(arr[(pin-1)/2] < arr[pin]){
        {
          int t;
          t = arr[(pin-1)/2];
          arr[(pin-1)/2] = arr[pin];
          arr[pin] = t;
        }
      }
    }
    {
      int t;
      t = arr[leaf];
      arr[leaf] = arr[0];
      arr[0] = t;
    }
  }
}

int main(){
  TCPSocket* serv = tcpsock_setup("127.0.0.1", 9487);
  char buffer[256] = {0};
  int off = 0;
  int data[5] = {0};
  int len;

  tcp_server(serv);

  while(1){

    TCPSocket* cli = tcp_accept(serv);

    len = tcp_read(cli, buffer, sizeof(buffer));
    if(len <= 0){ break; }
    if(len == 1 && buffer[0] == 0 ){ break; }

    buffer[len] = '\0';
    puts(buffer);

    off = 0;

    for(int i = 0; i < 5; i++){
      sscanf(buffer+off, " %d%n", &data[i], &len);
      off += len;
      printf("%6d", data[i]);
    }

    puts("");

    winner_sort(data,5);

    sprintf(buffer, "%d %d %d %d %d", data[0], data[1], data[2], data[3], data[4]);
    tcp_write(cli, buffer, strlen(buffer));
    // sorter end

    // summer start
    int sum = 0;
    int holder = 0;
    while(1){
      len = tcp_read(cli, buffer, sizeof(buffer));
      if(len <= 0){ break; }
      if(len == 1 && buffer[0] == 0 ){ break; }

      buffer[len] = '\0';
      puts(buffer);

      off = 0;

      while(sscanf(buffer+off, " %d%n", &holder, &len) != EOF){
        off += len;
        sum += holder;
      }

      sprintf(buffer, "%d", sum);
      tcp_write(cli, buffer, strlen(buffer));
    }

    puts("");

    puts("Client Disconnected!");
    tcp_close(cli);
    tcpsock_destroy(cli);
  }
  tcp_close(serv);
  tcpsock_destroy(serv);
}

