#include "common.h"

TCPSocket* tcpsock_setup(const char* host, int port){
  TCPSocket* conn = calloc(sizeof(TCPSocket), 1);
  memcpy(conn->host, host, strlen(host));
  conn->port = port;
  conn->sock = -1;
  conn->handle.sin_family = PF_INET;
  conn->handle.sin_addr.s_addr = inet_addr(host);
  conn->handle.sin_port = htons(port);
  conn->sock = socket(PF_INET, SOCK_STREAM, 0);
  conn->closed = 0;
  return conn;
}

void tcpsock_destroy(TCPSocket* conn){
  tcp_close(conn);
  free(conn);
}

int tcp_server(TCPSocket* conn){
  bind(conn->sock, (struct sockaddr*)(&conn->handle), sizeof(conn->handle));
  return listen(conn->sock, 100);
}

int tcp_client(TCPSocket* conn){
  if(conn->closed) {
    conn->sock = socket(PF_INET, SOCK_STREAM, 0);
    conn->closed = 0;
  }
  return connect(conn->sock, (struct sockaddr*)&(conn->handle), sizeof(conn->handle));
}

TCPSocket* tcp_accept(TCPSocket* serv){
  TCPSocket* client = calloc(sizeof(TCPSocket), 1);
  unsigned size = sizeof(client->handle);
  client->sock = accept(serv->sock, (struct sockaddr*)&(client->handle), &size);
  client->port = ntohs(client->handle.sin_port);
  memcpy(client->host, inet_ntoa(client->handle.sin_addr), sizeof(client->handle.sin_addr.s_addr));
  return client;
}

int tcp_read(TCPSocket* sock, char* buf, int buf_size){
  return recv(sock->sock, buf, buf_size, 0);
}

int tcp_write(TCPSocket* sock, char* buf, int buf_size){
  return send(sock->sock, buf, buf_size, 0);
}

int tcp_close(TCPSocket* sock){
  sock->closed = 1;
  return close(sock->sock);
}

