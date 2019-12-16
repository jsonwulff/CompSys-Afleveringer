// Login format
Rio_writen(socket, command, 1);
Rio_writen(socket, username, USERNAME_LEN);
Rio_writen(socket, password, PASSWORD_LEN);
Rio_writen(socket, ip, IP_LEN);
Rio_writen(socket, port, PORT_LEN);
Rio_writen(socket, "\n", 1);

// Lookup format for specific user
Rio_writen(name_server_socket, LOOKUP_COM, 1);
Rio_writen(name_server_socket, USE_ARGS, 1);
Rio_writen(name_server_socket, username, USERNAME_LEN);
Rio_writen(name_server_socket, "\n", 1);

// Lookup format for all users
Rio_writen(name_server_socket, LOOKUP_COM, 1);
Rio_writen(name_server_socket, IGN_ARGS, 1);
Rio_writen(name_server_socket, "\n", 1);

// Logout format
Rio_writen(name_server_socket, LOGOUT_COM, 1);
Rio_writen(name_server_socket, "\n", 1);
