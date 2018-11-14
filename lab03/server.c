#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int count = 0;
int active = 0;
Socket** sockets = NULL;
int list_size = 2;

void *serve(Socket* sock);

void socket_serve(Socket* sock, void*(*cb)(Socket*)){
  pthread_t thread;
  while(1){
    Socket* cli = socket_accept(sock);
    if(active >= list_size){
      {
        Socket** tmp = NULL;
        // dynamic expand socket list
        tmp = (Socket**)realloc(sockets, (int)(sizeof(Socket*)*list_size*1.5));
        if(tmp <= 0){
          printf("Error expanding\n");
          socket_write(cli, "Server Error.\n", strlen("Server Error.\n"));
          socket_close(cli);
          socket_free(cli);
          continue;
        } else {
          sockets = tmp;
          memset(sockets+list_size, 0, (int)((list_size*1.5 - list_size)*(sizeof(Socket*))));
          fprintf(stderr, "Expand client pool size %d -> %d\n", list_size, (int)(list_size*1.5));
          list_size*= 1.5;
        }
      }
    }

    for(int i = 0; i < list_size; i++){
      if(!sockets[i]){
        sockets[i] = cli;
        break;
      }
    }
    active += 1;
    count += 1;

    pthread_create(&thread, NULL, (void *(*)(void *))serve, (void*)cli);
    pthread_detach(thread);
    printf("client#%d connected.\n", count);
  }
}

void *serve(Socket* sock){
  int buf_size = 0, idx = count, shift = 0;
  char buffer[512] = {0};

  while(socket_wait_read(sock, 0)){
    // reading request
    sprintf(buffer, "%d: %n", idx, &shift);

    buf_size = socket_read(sock, buffer+shift, sizeof(buffer)-shift);

    // reads 0 byte when socket closed
    if(buf_size > 0) {
      buf_size += shift;
  
      buffer[buf_size] = 0;
      strcat(buffer, "\n");
      for(int i = 0; i < list_size; i++){
        // since we only nullify the slot when close, we should check here
        if(sockets[i]){
          socket_write(sockets[i], buffer, strlen(buffer));
        }
      }
    } else {
      break;
    }
  }

  active -= 1;

  for(int i = 0; i < list_size; i++){
    if(sockets[i] && sockets[i]->sock == sock->sock){
      sockets[i] = NULL;
    }
  }

  socket_close(sock);
  socket_free(sock);

  printf("client#%d disconnected.\n", idx);
  return NULL;
}

int main(){
  Socket* socket = NULL;
  socket = socket_setup("127.0.0.1", 9487, tcp);

  sockets = calloc(sizeof(Socket*), list_size);

  socket_listen(socket);
  socket_serve(socket, serve);
  socket_close(socket);
  socket_free(socket);
}
