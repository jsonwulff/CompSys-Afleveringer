void protocol_header(int socket, char* command, char* username, char* ip,
                     char* port, char* args_flag, char* args) {
  Rio_writen(socket, command, 1);
  Rio_writen(socket, username, USERNAME_LEN);
  Rio_writen(socket, ip, IP_LEN);
  Rio_writen(socket, port, PORT_LEN);
  Rio_writen(socket, args_flag, 1);
  Rio_writen(socket, args, MAX_LINE);
  if (args != NULL){Rio_writen(socket, args, MAX_LINE);}
  Rio_writen(socket, "\n", 1);
}