#include "peer.h"

char name_server_ip[IP_LEN];       // hostname and port of name server - these
char name_server_port[PORT_LEN];   // are passed as command line arguments.
int  name_server_socket = -1;      // socket to the name server. initialized to -1.
rio_t rio_read;
char read_buf[MAXLINE];


char my_ip[IP_LEN];                // my_ip and my_port are set on /login, and are used for listening.
char my_port[PORT_LEN];

char my_username[USERNAME_LEN];

int logged_in = 0;

void protocol_header(int socket, char* command, char* username, char* ip, char* port, char* args_flag, char* args){
        Rio_writen(socket, command, 1);
        Rio_writen(socket, username, USERNAME_LEN);
        Rio_writen(socket, ip, IP_LEN);
        Rio_writen(socket, port, PORT_LEN);
        Rio_writen(socket, args_flag, 1);
        Rio_writen(socket, args, MAX_LINE);
        Rio_writen(socket, "\n", 1);
}

int main(int argc, char **argv) {

  if (argc != MAIN_ARGNUM + 1) {
    fprintf(stderr, "Usage: %s <name server IP> <name server port>.\n", argv[0]);
    exit(EXIT_FAILURE);
  } else if (!is_valid_ip(argv[1])) {
    fprintf(stderr, ">> Invalid name server IP: %s\n", argv[1]);
    exit(EXIT_FAILURE);
  } else if (!is_valid_port(argv[2])) {
    fprintf(stderr, ">> Invalid name server port: %s\n", argv[2]);
    exit(EXIT_FAILURE);
  }

  snprintf(name_server_ip,   IP_LEN,   argv[1]);
  snprintf(name_server_port, PORT_LEN, argv[2]);

  printf(">> Connecting to name server at %s:%s ...\n",
      name_server_ip, name_server_port);

  /*
   * TODO #1
   * TODO: SETUP NAME SERVER CONNECTION HERE
   * HINT: use the specified ip and port to setup a socket to the name server.
   * HINT: remember that you are free to use everything from csapp.c.
   */
  name_server_socket = Open_clientfd(name_server_ip, name_server_port);
  Rio_readinitb(&rio_read, name_server_socket);

  /*
   * we use the RIO library from csapp.c to read user input line by line.
   */
  rio_t rio;
  char rio_buf[MAX_LINE];
  ssize_t num_read;
  Rio_readinitb(&rio, STDIN_FILENO);

  command_t command;          // current command, and array of size MAX_USER_ARGNUM
  args_t    args;             // holding arguments to current command (see peer.h).

  char *username, *password;  // these pointers will serve different
  char *ip, *port;            // purposes based on the current command.
  char *message;

  int running = 1;

  while (running) {
    /*
     * read line of user input and parse
     * the command, storing arguments in args.
     */
    if ((num_read = Rio_readlineb(&rio, rio_buf, MAX_LINE)) < 0) {
      fprintf(stderr, "rio_read() error: %s\n", strerror(errno));
      exit(EXIT_FAILURE);
    } else if (num_read <= 1) continue; // if input is an empty line or EOF.

    command = parse_command(rio_buf, args); // see common.h for a description of parse_command()
    //printf("%s", args);

    switch (command) {

      case LOGIN:
        if (logged_in) {
          printf(">> /login error: already logged in as %s\n", my_username);
          break;
        }

        username = args[0];                // username and password to login with.
        password = args[1];
        ip       = args[2];                // ip and port that the name server should respond to
        port     = args[3];                // (eg. with messages from other users).

        snprintf(my_ip,   IP_LEN,   ip);   // write ip and port to my_ip and my_port
        snprintf(my_port, PORT_LEN, port);
        snprintf(my_username, USERNAME_LEN, username);
        // QUESTION: Should username and password use this aswell?

        /*
         * TODO #2
         * TODO: LOG INTO NAME SERVER HERE.
         * HINT: use the established connection to the
         * HINT: name server to send a login request
         *
         * HINT: write to the name_server_socket set up earlier; recall that a
         * HINT: socket is functionally similar to a file descriptor, and
         * HINT: is written to similarly. you can for example use the RIO library,
         * HINT: but otherwise google is your friend.
         *
         * HINT: eventually, you want to set logged_in to 1, but depending
         * HINT: on your protocol, you may want to somehow confirm the login first :)
         */
        protocol_header(name_server_socket, "0", username, my_ip, my_port, "0", password);
        if (Rio_readlineb(&rio_read, read_buf, MAXLINE) != 0){
          switch (atoi(&read_buf[0])){
            case 0: //succes
              printf("Login succesfull\n");
              logged_in = 1;
              break;
            case 1: //invalid username
              printf("invalid username\n");
              break;
            case 2: //incorrect password
              printf("incorrect password\n");
              break;
            default:
              break;
          }
        }

        break;


      case LOOKUP:
        /*
         * TODO #3
         * TODO: LOOKUP USERS HERE.
         *
         * HINT: recall that the API specifies that the user can
         * HINT: optionally omit a username to the lookup command.
         * HINT: you can choose to either handle it here or server-
         * HINT: side, depending on your chosen protocol.
         */
        if (!logged_in) {
          printf(">> /lookup error: not logged onto name server.\n");
          break;
        }

        username = args[0]; // username to lookup (may be null)

        if (username == NULL){

            protocol_header(name_server_socket, "3", my_username, my_ip, my_port, "0", username);
            if (Rio_readlineb(&rio_read, read_buf, MAXLINE) != 0){
                printf("%s", read_buf);
            }

        }else {

            protocol_header(name_server_socket, "3", my_username, my_ip, my_port, "1", username);
            if (Rio_readlineb(&rio_read, read_buf, MAXLINE) != 0){
                printf("%s", read_buf);
            }

        }
        break;


      case LOGOUT:
        if (!logged_in) {
          printf(">> /logout error: not logged onto name server.\n");
          break;
        }
        /*
         * TODO #4
         * TODO: LOGOUT OF NAME SERVER HERE.
         *
         * HINT: as with /login, you eventually want to set logged_in to 0.
         */
        protocol_header(name_server_socket, "1", my_username, my_ip, my_port, "0", NULL);
        if (Rio_readlineb(&rio_read, read_buf, MAXLINE) != 0){
            printf("%s", read_buf);
        }

        logged_in = 0;


        break;


      case EXIT:
        protocol_header(name_server_socket, "0", my_username, my_ip, my_port, "0", NULL);
        if (Rio_readlineb(&rio_read, read_buf, MAXLINE) != 0){
            printf("%s", read_buf);
        }
        running = 0;
        /*
         * TODO #5
         * TODO: EXIT CLIENT HERE.
         *
         * HINT: as is, the client simply exits. depending on your protocol,
         * HINT: consider what should happen if the user is logged in at exit.
         */

        break;


      case MSG:
        /*
         * NOT REQUIRED FOR A6. we save the actual messaging for A7.
         */

        if (!logged_in) {
          printf(">> /msg error: not logged onto name server.\n");
          break;
        }
        username = args[1]; // username of recipient.
        message  = args[2]; // actual message to send.
        break;


      case SHOW:
        /*
         * NOT REQUIRED FOR A6. we leave the actual messaging for A7
         */

        if (!logged_in) {
          printf(">> /show error: not logged onto name server.\n");
          break;
        }
        username = args[1]; // name of user to show messages from (may be NULL)
        break;


      case ERROR:
        printf(">> Error: unknown command or wrong number of arguments.\n");
        break;
    }
  }

  printf(">> Closing client ...\n");
  /*
   * TODO #6
   * TODO: CLOSE CONNECTION (ie. socket to name server) HERE.
   * HINT: at this point, the client is (should be) properly logged out of
   * HINT: the name server, so this step should be easy :)
   */
  Close(name_server_socket);
  exit(EXIT_SUCCESS);
}
