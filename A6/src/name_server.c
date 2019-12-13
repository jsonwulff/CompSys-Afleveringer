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

/*
 * TODO: for implementing name_server.c, you will not receive as many
 * TODO: hints as you did for peer.c :) the two TODO's within the main()
 * TODO: function should therefore NOT be considered exhaustible;
 * TODO: you will need to figure out the rest on your own.
 */
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
    // client_init(clients[0], (int *) connfdp);
    // Pthread_create(&tid, NULL, thread, (void *) clients[0]);
    Pthread_create(&tid, NULL, thread, connfdp);

    Getnameinfo((SA *)&clientaddr, clientlen, client_hostname, MAXLINE,
                client_port, MAXLINE, 0);
    printf("Connected to (%s, %s) via thread %d\n", client_hostname,
           client_port, (int)tid);
    /*
     * TODO: LISTEN FOR PEERS HERE
     * HINT: accept new peers on the listen_socket set up earlier.
     * HINT: you may want to represent clients with a struct. a template
     * HINT: struct client_t is given in name_server.h, which you can extend.
     *
     * HINT: consider why this is wrapped in a loop - once a
     * HINT: client connects, the server should delegate servicing
     * HINT: the client and continue listening for new clients :)
     *
     *
     * HINT: read Beej's guide: http://beej.us/guide/bgnet/html/
     * HINT: (in particular the "System Calls or Bust" section)
     *
     */
  }

  exit(EXIT_SUCCESS);
}
// void *client_init(struct client_t *c, int *conn_suck){
//   c->conn_socket = (int) *conn_suck;
//   c->username = NULL;
//   c->password = NULL;
//   c->port = NULL;
//   c->ip = NULL;
// }

void routine(int connfd) {
  size_t n;
  char buf[MAXLINE];
  rio_t rio;

  Rio_readinitb(&rio, connfd);
  while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {  // line:netp:echo:eof
    int command = atoi(&buf[0]);

    //research if free(3) should be used here..
    char *usrname = strndup(buf + 1, USERNAME_LEN);
    char *ip = strndup(buf + 33, IP_LEN);
    char *port = strndup(buf + 49, PORT_LEN);
    int args_flag = atoi(&buf[57]);
    char* args;
    if (args_flag){args = strndup(buf + 58, MAX_LINE);}

    //printf("recieved request: %d, from %s:%s:%s\n", command, usrname, ip, port);
    int user_index = find_user(usrname);
    char *return_statement = NULL;

    switch (command) {
      case LOGIN:
        printf("login request. Informing client\n");
        // Rio_writen(connfd, "login request recieved\n", 23);
        char *password = strndup(args, PASSWORD_LEN);
        printf("%s", password);

        for (int i = 0; i < MAX_USERS; i++) {
          if (clients[i] != NULL) {
            if (strcmp(clients[i]->username, usrname) == 0) {
              if (strcmp(clients[i]->password, password) == 0) {
                printf("Password match\n");
                clients[i]->logged_in = 1;
                clients[i]->ip = ip;
                clients[i]->port = port;
                clients[i]->conn_socket = connfd;
                num_active_clients++;
                return_statement = "0\n";
                break;
              } else {
                return_statement = "2\n";
              }
            }
          }
        }
        if (return_statement == NULL) {
          return_statement = "1\n";
        }
        break;
      case LOGOUT:
        printf("Logout request recieved\n");
        if (user_index != -1){
          printf("user exists: %d\n", user_index);
          clients[user_index]->logged_in = 0;
          num_active_clients--;
          return_statement = "0\n";
        }else {return_statement = "1\n";}
        break;
      case EXIT:
        return_statement = "Exit request recieved\n";
        break;
      case LOOKUP:
        for (int i = 0; i < MAX_USERS; i++) {
          char msg[MAX_LINE];
          sprintf(msg, "%d user(s) online. The list follows:\n", num_active_clients);
          Rio_writen(connfd, msg, sizeof(msg));
          if (clients[i] != NULL){
            if (clients[i]->logged_in == 1) {
              char msg2[MAX_LINE];
              sprintf(msg2, "\nUser: %s  IP: %s  Port: %s\n", clients[i]->username, clients[i]->ip, clients[i]->port);
              Rio_writen(connfd, msg2, sizeof(msg2));
            }
          }
        }
        return_statement = "\nDONE\n";
        break;
      case MSG:
        return_statement = "MSG request recieved\n";
        break;
      case SHOW:
        return_statement = "Show request recieved\n";
        break;
      case ERROR:
        return_statement = "Error request recieved\n";
        break;
      default:
        break;
    }

    Rio_writen(connfd, return_statement, strlen(return_statement));

    // printf("from user: %s\n", usrname);
    // printf("length of username: %d \n", sizeof(usrname));
    // printf("length of client: %d \n", sizeof(clients[0]->username));
    // printf("comparison: %d\n", (strcmp(usrname, clients[0]->username)));

    // if(command == 0) {
    //   printf("Login reqeust recieved\n");
    // }
    printf("server received %d bytes\n", (int)n);
  }
}

void *thread(void *vargp) {
  // struct client_t *c = vargp;
  // int connfd = c->conn_socket;
  int connfd = *((int *)vargp);
  Pthread_detach(pthread_self());
  Free(vargp);
  routine(connfd);
  Close(connfd);
  return NULL;
}