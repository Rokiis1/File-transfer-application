#ifndef FILE_TRANSFER_H
#define FILE_TRANSFER_H

void send_file(int socket, const char *file_name);
void receive_file(int socket, const char *file_name);
void initialize_directories();

#endif