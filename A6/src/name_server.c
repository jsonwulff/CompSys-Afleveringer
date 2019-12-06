#include <stdio.h>
#include "name_server.h"

char listen_port[PORT_LEN];
int  listen_socket     = -1;       // listen socket. initialized to -1.

int num_active_clients = 0;
client_t *clients[MAX_USERS];      // array of pointers to structs representing active clients.

void *thread(void *vargp);
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

  for (int i = 0; i < MAX_USERS; i++)
    clients[i] = NULL;

  printf(">> Accepting peers on port %s ...\n", listen_port);

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
    clientlen=sizeof(struct sockaddr_storage);
    connfdp = Malloc(sizeof(int));
    *connfdp = Accept(listen_socket, (SA *) &clientaddr, &clientlen);
    Pthread_create(&tid, NULL, thread, connfdp);
    Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE, client_port, MAXLINE, 0);
    printf("Connected to (%s, %s) via thread %d\n", client_hostname, client_port, (int) tid);
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
  Close(listen_socket)

  exit(EXIT_SUCCESS);
}

void routine(int connfd){
  size_t n;
  char buf[MAXLINE];
  rio_t rio;

  Rio_readinitb(&rio, connfd);
  while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {  // line:netp:echo:eof
    printf("server received %d bytes\n", (int)n);
    printf("And it was %s", (char*) buf);
  }
}

void *thread(void *vargp){
  int connfd = *((int *)vargp);
  Pthread_detach(pthread_self());
  Free(vargp);
  routine(connfd);
  Close(connfd);
  return NULL;
}