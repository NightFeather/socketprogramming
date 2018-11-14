#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int count = 0;
int active = 0;
Socket* sockets[3] = {0};
int list_size = 3;

typedef struct {
  char username[64];
  char password[128];
  int state;
} User;

int user_cnt = 3;
User users[3] = {
  { "socket", "secret", -1 },
  { "really", "passwd", -1 },
  { "tedius", "guavas", -1 }
};

int authenticate( char* username, char* password ){
  printf("login attempt: %s:%s\n", username, password);
  for(int i = 0; i < 3; i++){
    User user = users[i];
    if(strcmp(user.username, username) == 0 &&
       strcmp(user.password, password) == 0){
      return i;
    }
  }
  return -1;
}

int find_user(char* username){
  for(int i =0; i < 3; i++){
    if(strcmp(users[i].username, username) == 0){
      return i;
    }
  }
  return -1;
}

int broadcast(char* buffer, int size){
  for(int i = 0; i < list_size; i++){
    // since we only nullify the slot when close, we should check here
    if(sockets[i]){
      socket_write(sockets[i], buffer, size);
    }
  }
  return 0;
}

int unicast(int uid, char* buffer, int size){
  for(int i = 0; i < list_size; i++){
    // since we only nullify the slot when close, we should check here
    if(sockets[i] && users[uid].state == sockets[i]->sock){
      socket_write(sockets[i], buffer, size);
    }
  }
  return 0;

}

void *serve(Socket* sock);
void socket_serve(Socket* sock, void*(*cb)(Socket*)){
  pthread_t thread;
  while(1){
    Socket* cli = socket_accept(sock);

    if(active >= 3) {
      socket_write(cli, "Server Full.\n", sizeof("Server Full.\n"));
      socket_close(cli);
      socket_free(cli);
      continue;
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
  int buf_size = 0, idx = count, shift = 0, uid = 0, tuid = -1;
  char buffer[512] = {0};

  { // block of authentication logic
    char username[72] = {0}, password[136] = {0};
    socket_wait_read(sock, 0);
    buf_size = socket_read(sock, username, 64);
    if(buf_size <= 0){ goto serve_cleanup; }

    socket_wait_read(sock, 0);
    buf_size = socket_read(sock, password, 128);
    if(buf_size <= 0){ goto serve_cleanup; }

    if((uid = authenticate(username, password)) >= 0){
      sprintf(buffer, "Hello, %s\n", username);
      socket_write(sock, buffer, strlen(buffer));
      if(users[uid].state < 0){
        socket_write(sock, "You Successfully Logged in.\n", strlen("You Successfully Logged in.\n"));
        users[uid].state = sock->sock;
      } else {
        socket_write(sock, "Your seat have been occupied.\n", strlen("Your seat have been occupied.\n"));
        goto serve_cleanup;
      }
    } else {
      socket_write(sock, "Login Failed.\n", sizeof("Login Failed.\n"));
      goto serve_cleanup;
    }
  }

  { // show all online users
    for(int i = 0; i < 3; i++){
      if(users[i].state >= 0){
        sprintf(buffer, "%s online.\n", users[i].username);
        socket_write(sock, buffer, strlen(buffer));
      }
    }
  }

  while(socket_wait_read(sock, 0)){

    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "%s: %n", users[uid].username, &shift);

    // reading request
    buf_size = socket_read(sock, buffer+shift, sizeof(buffer)-shift);

    // reads 0 byte when socket closed
    if(buf_size > 0) {
      buf_size += shift;
      buffer[buf_size] = 0;
      strcat(buffer, "\n");
      buf_size++;

      // switch into unidirection channel @user
      // @random random select one user
      // @part leave channel
      if(buffer[shift] == '@'){
        {
          char tuna[72] = {0};
          sscanf(buffer+shift+1, "%s ", tuna);
          printf("%s -> %s\n", users[uid].username, tuna);
          if(strcmp(tuna, "random") == 0){
            tuid = random() % 2;
            if(tuid == uid){ tuid = (tuid+1)%3; }
          } else if(strcmp(tuna, "part") == 0){
            tuid = -1;
          } else {
            tuid = find_user(tuna);
          }
        }
      }

      if(tuid < 0){
        broadcast(buffer, strlen(buffer));
      } else {
        printf("%s -> %s\n", users[uid].username, users[tuid].username);
        unicast(tuid, buffer, buf_size);
      }

    } else {
      goto serve_cleanup;
    }
  }

  serve_cleanup:

  active -= 1;

  for(int i = 0; i < list_size; i++){
    if(sockets[i] && sockets[i]->sock == sock->sock){
      sockets[i] = NULL;
    }
  }

  users[uid].state = -1;
  socket_close(sock);
  socket_free(sock);

  sprintf(buffer, "%s left.\n", users[uid].username);
  broadcast(buffer, strlen(buffer));
  printf("client#%d disconnected.\n", idx);
  return NULL;
}

int main(){
  Socket* socket = NULL;
  socket = socket_setup("127.0.0.1", 9487, tcp);

  puts("Loading Users");
  for(int i =0; i < user_cnt; i++){
    printf("User#%d: %s:%s\n", i, users[i].username, users[i].password);
  }

  socket_listen(socket);
  socket_serve(socket, serve);
  socket_close(socket);
  socket_free(socket);
}
