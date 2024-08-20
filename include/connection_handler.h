#ifndef CONNECTION_HANDLER_H
#define CONNECTION_HANDLER_H

void connection_handler(void *socket_desc);
int initialize_server();
void accept_client(int server_fd, const char *file_name);

#ifdef _WIN32
void initialize_winsock();
#endif

#endif
