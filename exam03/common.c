#include "common.h"
#include <stdio.h>

Socket* socket_setup(char* host, int port, int type){
  Socket* conn = calloc(sizeof(Socket), 1);
  memcpy(conn->host, host, strlen(host));

  conn->type = type;
  conn->port = port;
  conn->sock = -1;
  conn->handle.sin_family = PF_INET;
  conn->handle.sin_addr.s_addr = inet_addr(host);
  conn->handle.sin_port = htons(port);

  if(type == tcp){
    conn->sock = socket(PF_INET, SOCK_STREAM, 0);
  } else if (type == udp) {
    conn->sock = socket(PF_INET, SOCK_DGRAM, 0);
  } else {
    free(conn);
    return (Socket*)(0x0);
  }

  return conn;
}

// only call listen() when using tcp
int socket_listen(Socket* sock){
  bind(sock->sock, (struct sockaddr*)(&sock->handle), sizeof(sock->handle));
  if(sock->type == tcp) {
    return listen(sock->sock, 100);
  } else {
    return 0;
  }
}

Socket* socket_accept(Socket* sock){
  if(sock->type == udp){ return 0; }
  Socket* client = calloc(sizeof(Socket), 1);
  client->type = tcp;
  unsigned size = sizeof(client->handle);
  client->sock = accept(sock->sock, (struct sockaddr*)&(client->handle), &size);
  return client;
}

int socket_connect(Socket* sock){
  if(sock->type == udp) { return 0; }
  return connect(sock->sock, (struct sockaddr*)&(sock->handle), sizeof(sock->handle));
}

int socket_write(Socket* sock, char* buf, int buf_size){
  unsigned handle_size = sizeof(sock->handle);
  switch(sock->type){
    case tcp:
      return send(sock->sock, buf, buf_size, 0);
    break;
    case udp:
      return sendto(sock->sock, buf, buf_size, 0, (struct sockaddr*)&(sock->handle), handle_size);
    break;
    default:
      return -1;
  }
}

int socket_read(Socket* sock, char* buf, int buf_size){
    unsigned handle_size = sizeof(sock->handle);
    switch(sock->type){
    case tcp:
      return recv(sock->sock, buf, buf_size, 0);
    break;
    case udp:
      return recvfrom(sock->sock, buf, buf_size, 0, (struct sockaddr*)&(sock->handle), &handle_size);
    break;
    default:
      return -1;
  }
}

int socket_close(Socket* sock){
  return close(sock->sock);
}

void socket_free(Socket* sock){
  free(sock);
}

int fd_wait_write(int fd, int secs){
  int ret = 0;
  fd_set wfds;
  struct timeval tv = {0};

  FD_ZERO(&wfds);

  tv.tv_sec = secs;
  tv.tv_usec = 0;

  FD_SET(fd, &wfds);

  if(secs > 0){
    ret = select(fd+1, NULL, &wfds, NULL, &tv);
  } else {
    ret = select(fd+1, NULL, &wfds, NULL, NULL);
  }

  if(ret < 0) {
    puts("something wrong");
    return ret;
  }

  if(FD_ISSET(fd, &wfds)){
    return 1;
  } else {
    return 0;
  }

}

int fd_wait_read(int fd, int secs){
  int ret = 0;

  fd_set rfds;
  FD_ZERO(&rfds);

  struct timeval tv = {0};
  tv.tv_sec = secs;
  tv.tv_usec = 0;

  FD_SET(fd, &rfds);

  if(secs > 0){
    ret = select(fd+1, &rfds, NULL, NULL, &tv);
  } else {
    ret = select(fd+1, &rfds, NULL, NULL, NULL);
  }

  if(ret < 0) {
    puts("something wrong with select()");
    return ret;
  }

  if(FD_ISSET(fd, &rfds)){
    return 1;
  } else {
    return 0;
  }

}

int socket_wait_read(Socket* sock, int secs){
  return fd_wait_read(sock->sock, secs);
}

int socket_wait_write(Socket* sock, int secs){
  return fd_wait_write(sock->sock, secs);
}
