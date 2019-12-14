#include "name_server.h"
#include <stdio.h>

char listen_port[PORT_LEN];
int listen_socket = -1;  // listen socket. initialized to -1.

int num_active_clients = 0;
client_t *clients[MAX_USERS];  // array of pointers to structs representing
                               // active clients.

void *thread(void *vargp);
void *client_init(struct client_t *c, int *conn_sock);

int find_user(char *username) {
  for (int i = 0; i < MAX_USERS; i++) {
    if (clients[i] != NULL) {
      if (strcmp(clients[i]->username, username) == 0) {
        return i;
      }
    }
  }
  return -1;
}

int main(int argc, char **argv) {
  if (argc != MAIN_ARGNUM + 1) {
    fprintf(stderr, "Usage: %s <listening port>\n", argv[0]);
    exit(EXIT_FAILURE);
  } else if (!is_valid_port(argv[1])) {
    fprintf(stderr, ">> Invalid port number: %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  snprintf(listen_port, PORT_LEN, argv[1]);

  for (int i = 0; i < MAX_USERS; i++) clients[i] = NULL;

  printf(">> Accepting peers on port %s ...\n", listen_port);

  // NOTE: maybe move this out
  struct client_t client0;
  client0.username = "julian";
  client0.password = "hamster";
  clients[0] = &client0;

  struct client_t client2;
  client2.username = "pelle";
  client2.password = "h";
  clients[2] = &client2;

  /*
   * TODO: SETUP LISTENING SOCKET HERE
   * HINT: remember that you are free to use everything from csapp.c ;) ;)
   */
  int *connfdp;
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;
  pthread_t tid;
  char client_hostname[MAXLINE], client_port[MAXLINE];
  listen_socket = Open_listenfd(listen_port);

  int running = 1;
  while (running) {
    clientlen = sizeof(struct sockaddr_storage);
    connfdp = Malloc(sizeof(int));
    *connfdp = Accept(listen_socket, (SA *)&clientaddr, &clientlen);
    Pthread_create(&tid, NULL, thread, connfdp);

    Getnameinfo((SA *)&clientaddr, clientlen, client_hostname, MAXLINE,
                client_port, MAXLINE, 0);
    printf("Connected to (%s, %s) via thread %d\n", client_hostname,
           client_port, (int)tid);
  }

  exit(EXIT_SUCCESS);
}

void routine(int connfd) {
  size_t n;
  char buf[MAXLINE];
  rio_t rio;

  int user_index;

  Rio_readinitb(&rio, connfd);
  while (((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0)) {  // line:netp:echo:eof
    char *temp_com = strndup(buf, 1);
    int command = atoi(temp_com);
    free(temp_com);

    char *return_statement;

    switch (command) {
      case LOGIN:
        printf(">> Login request received.\n");

        char *username = strndup(buf + 1, USERNAME_LEN);
        char *password = strndup(buf + 33, PASSWORD_LEN);
        char *ip = strndup(buf + 65, IP_LEN);
        char *port = strndup(buf + 81, PORT_LEN);

        int user_index = find_user(username);

        if (user_index == -1) {
          return_statement = "3\n"; // Username dosn't exsist
          break;
        } else if ( strcmp(clients[user_index]->password, password) != 0 ) {
          return_statement = "2\n"; // Password dosn't match
          break;
        } else if ( strcmp(clients[user_index]->password, password) == 0 ) {
          if (clients[user_index]->logged_in == 1) {
            return_statement = "1\n";
          } else {
            clients[user_index]->logged_in = 1;
            clients[user_index]->ip = ip;
            clients[user_index]->port = port;
            clients[user_index]->conn_socket = connfd;
            num_active_clients++;
            return_statement = "0\n";
            break;
          }
        }
        free(username);
        free(password);
        free(ip);
        free(port);
        break;

      case LOGOUT:
        printf(">> Logout request recieved.\n");
        if (clients[user_index]->logged_in == 1) {
          clients[user_index]->logged_in = 0;
          num_active_clients--;
          return_statement = "0\n";
          break;
        } else {
          return_statement = "1\n";
          break;
        }

        break;


      case LOOKUP: ;
        printf(">> Lookup request recieved.\n");

        char *temp_use_args = strndup(buf + 1, 1);
        int use_args = atoi(temp_use_args);
        free(temp_use_args);

        char msg[MAX_LINE];

        if (use_args) {  // Lookup specific user
          char *username = strndup(buf + 2,USERNAME_LEN);
          return_statement = "1\n";
          for (int i = 0; i < MAX_USERS; i++) {
            if (clients[i] != NULL){
              if (strcmp(clients[i]->username, username) == 0) {
                sprintf(msg, ">> %s is online.\n>> IP: %s\n>> Port: %s\n", clients[i]->username,
                  clients[i]->ip, clients[i]->port);
                Rio_writen(connfd, msg, strlen(msg));
                return_statement = "0\n";
                free(username);
                break;
              }
            }
          }

        } else {
          sprintf(msg, ">> %d user online. The list follows:\n", num_active_clients);
          Rio_writen(connfd, msg, strlen(msg));
          for (int i = 0; i < MAX_USERS; i++) {
            if (clients[i] != NULL){
              if (clients[i]->logged_in == 1) {
                sprintf(msg, ">> User: %s\n>> IP: %s\n>> Port: %s\n", clients[i]->username,
                  clients[i]->ip, clients[i]->port);
                Rio_writen(connfd, msg, strlen(msg));
                return_statement = "0\n";
              }
            }
          }
        }

        break;
    }

    Rio_writen(connfd, return_statement, strlen(return_statement));

    printf("server received %d bytes\n", (int)n);
  }
  clients[user_index]->logged_in == 0;
}

void *thread(void *vargp) {
  int connfd = *((int *)vargp);
  Pthread_detach(pthread_self());
  Free(vargp);
  routine(connfd);
  Close(connfd);
  return NULL;
}