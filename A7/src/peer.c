#include "peer.h"

char name_server_ip[IP_LEN];      // hostname and port of name server - these
char name_server_port[PORT_LEN];  // are passed as command line arguments.
int name_server_socket = -1;  // socket to the name server. initialized to -1.

char my_ip[IP_LEN];  // my_ip and my_port are set on /login, and are used for listening.
char my_port[PORT_LEN];
char my_username[USERNAME_LEN];
char my_password[PASSWORD_LEN];

int logged_in = 0;

rio_t rio_read;
char read_buf[MAXLINE];

// Login protocol
void login_protocol(int socket, char* command, char* username, char* password, char* ip,
                     char* port) {
  Rio_writen(socket, command, 1);
  Rio_writen(socket, username, USERNAME_LEN);
  Rio_writen(socket, password, PASSWORD_LEN);
  Rio_writen(socket, ip, IP_LEN);
  Rio_writen(socket, port, PORT_LEN);
  Rio_writen(socket, "\n", 1);
}

int main(int argc, char** argv) {
  if (argc != MAIN_ARGNUM + 1) {
    fprintf(stderr, "Usage: %s <name server IP> <name server port>.\n",
            argv[0]);
    exit(EXIT_FAILURE);
  } else if (!is_valid_ip(argv[1])) {
    fprintf(stderr, ">> Invalid name server IP: %s\n", argv[1]);
    exit(EXIT_FAILURE);
  } else if (!is_valid_port(argv[2])) {
    fprintf(stderr, ">> Invalid name server port: %s\n", argv[2]);
    exit(EXIT_FAILURE);
  }

  snprintf(name_server_ip, IP_LEN, argv[1]);
  snprintf(name_server_port, PORT_LEN, argv[2]);

  printf(">> Connecting to name server at %s:%s ...\n", name_server_ip,
         name_server_port);


  name_server_socket = Open_clientfd(name_server_ip, name_server_port);
  Rio_readinitb(&rio_read, name_server_socket);

  /*
   * we use the RIO library from csapp.c to read user input line by line.
   */
  rio_t rio;
  char rio_buf[MAX_LINE];
  ssize_t num_read;
  Rio_readinitb(&rio, STDIN_FILENO);

  command_t command;  // current command, and array of size MAX_USER_ARGNUM
  args_t args;        // holding arguments to current command (see peer.h).

  char *username, *password;  // these pointers will serve different
  char *ip, *port;            // purposes based on the current command.
  char* message;

  int running = 1;

  while (running) {
    /*
     * read line of user input and parse
     * the command, storing arguments in args.
     */
    if ((num_read = Rio_readlineb(&rio, rio_buf, MAX_LINE)) < 0) {
      fprintf(stderr, "rio_read() error: %s\n", strerror(errno));
      exit(EXIT_FAILURE);
    } else if (num_read <= 1)
      continue;  // if input is an empty line or EOF.

    command = parse_command(rio_buf, args);  // see common.h for a description of parse_command()
    // printf("%s", args);

    switch (command) {
      case LOGIN:
        if (logged_in) {
          printf(">> /login error: already logged in as %s\n", my_username);
          break;
        }

        username = args[0];  // username and password to login with.
        password = args[1];
        ip = args[2];    // ip and port that the name server should respond to
        port = args[3];  // (eg. with messages from other users).

        snprintf(my_ip, IP_LEN, ip);  // write ip and port to my_ip and my_port
        snprintf(my_port, PORT_LEN, port);
        snprintf(my_username, USERNAME_LEN, username);
        snprintf(my_password, USERNAME_LEN, password);

        login_protocol(name_server_socket, LOGIN_COM, my_username, my_password, my_ip, my_port);
        if (Rio_readlineb(&rio_read, read_buf, MAXLINE) != 0) {
          switch (atoi(&read_buf[0])) {
            case 0:  // Login successfull
              printf(">> Welcome to the name server.\n");
              logged_in = 1;
              break;
            case 1:  // invalid username
              printf(">> User is already logged in.\n");
              break;
            case 2:  // invalid username
              printf(">> Incorrect password, try again.\n");
              break;
            case 3:  // incorrect password
              printf(">> Invalid username, try again.\n");
              break;
            default:
              printf(">> Unrecognized error, try again\n");
              break;
          }
        }

        break;

      case LOOKUP:
        if (!logged_in) {
          printf(">> /lookup error: not logged onto name server.\n");
          break;
        }

        username = args[0];  // username to lookup (may be null)

        int done = 0;

        if (username != NULL) {
          Rio_writen(name_server_socket, LOOKUP_COM, 1);
          Rio_writen(name_server_socket, USE_ARGS, 1);
          Rio_writen(name_server_socket, username, USERNAME_LEN);
          Rio_writen(name_server_socket, "\n", 1);
          while(!done) {
            if (Rio_readlineb(&rio_read, read_buf, MAXLINE) != 0) {
              if(strcmp(read_buf, "0\n") == 0) {
                done = 1;
                break;
              } else if(strcmp(read_buf, "1\n") == 0) {
                printf("%s is not online (or username invalid).\n", username);
                done = 1;
                break;
              } else {
                printf("%s", read_buf);
              }
            }
          }
        } else {
          Rio_writen(name_server_socket, LOOKUP_COM, 1);
          Rio_writen(name_server_socket, IGN_ARGS, 1);
          Rio_writen(name_server_socket, "\n", 1);
          while(!done) {
            if (Rio_readlineb(&rio_read, read_buf, MAXLINE) != 0) {
              if(strcmp(read_buf, "0\n") == 0) {
                done = 1;
                break;
              } else {
                printf("%s", read_buf);
              }
            }
          }
        }

        break;

      case LOGOUT:
        if (!logged_in) {
          printf(">> /logout error: not logged onto name server.\n");
          break;
        }

        Rio_writen(name_server_socket, LOGOUT_COM, 1);
        Rio_writen(name_server_socket, "\n", 1);
        if (Rio_readlineb(&rio_read, read_buf, MAXLINE) != 0) {
            switch (atoi(&read_buf[0])){
              case 0:
                printf(">> Goodbye ...\n");
                logged_in = 0;
                break;
              case 1:
                printf(">> Logout failed\n");
                break;
            }
        }
        break;

      case EXIT:
        if (logged_in){
          Rio_writen(name_server_socket, LOGOUT_COM, 1);
          Rio_writen(name_server_socket, "\n", 1);
          if (Rio_readlineb(&rio_read, read_buf, MAXLINE) != 0) {
            switch (atoi(&read_buf[0])){
              case 0:
                printf(">> Logging out...\n");
                logged_in = 0;
                printf(">> Closing client...\n");
                running = 0;
                break;
              case 1:
                printf(">> Logout failed...\n");
                break;
            }
          }
        } else {
            printf(">> Closing client...\n");
            running = 0;
            break;
        }

        break;

      case MSG:
        if (!logged_in) {
          printf(">> /msg error: not logged onto name server.\n");
          break;
        }
        username = args[1];  // username of recipient.
        message = args[2];   // actual message to send.
        break;

      case SHOW:
       if (!logged_in) {
          printf(">> /show error: not logged onto name server.\n");
          break;
        }
        username = args[1];  // name of user to show messages from (may be NULL)
        break;

      case ERROR:
        printf(">> Error: unknown command or wrong number of arguments.\n");
        break;
    }
  }

  Close(name_server_socket);
  exit(EXIT_SUCCESS);
}
