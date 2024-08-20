#include <stdio.h>
#include <unistd.h>

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#endif

#include "client_handler.h"
#include "file_transfer.h"
#include "logger.h"
#include "protocol.h"

/**
 * @brief Handles communication with a client.
 *
 * This function receives a file name from the client, sends the file to the client,
 * receives a file from the client, and then closes the connection.
 *
 * @param client_socket The socket connected to the client.
 */
void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    int bytes_received;

    if ((bytes_received = recv(client_socket, buffer, sizeof(buffer), 0)) <= 0) {
        log_error("Failed to receive file name");
        close(client_socket);
        return;
    }

    buffer[bytes_received] = '\0';
    log_info("Received file name: %s", buffer);

    send_file(client_socket, buffer);

    receive_file(client_socket, buffer);

    close(client_socket);
    log_info("Client handled and connection closed");
}