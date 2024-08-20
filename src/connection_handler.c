#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef _WIN32
typedef int socklen_t;
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#endif

#include "connection_handler.h"
#include "file_transfer.h"
#include "logger.h"
#include "client_handler.h"

#define PORT 8080
#define BACKLOG 10

#ifdef _WIN32
/**
 * @brief Initializes Winsock for Windows.
 *
 * This function initializes the Winsock library, which is required for network programming on Windows.
 */  
void initialize_winsock() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        log_error("WSAStartup failed");
        exit(EXIT_FAILURE);
    }
}
#endif

/**
 * @brief Initializes the server.
 *
 * This function creates a socket, sets socket options, binds the socket to an address and port,
 * and starts listening for incoming connections.
 *
 * @return The file descriptor for the server socket.
 */
int initialize_server() {
#ifdef _WIN32
    initialize_winsock();
#endif

    int server_fd;
    struct sockaddr_in address;
    int opt = 1;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        log_error("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt))) {
        log_error("setsockopt failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        log_error("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, BACKLOG) < 0) {
        log_error("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    log_info("Server initialized");
    return server_fd;
}

/**
 * @brief Accepts and handles client connections.
 *
 * This function continuously accepts incoming client connections and handles each client
 * by calling the handle_client function.
 *
 * @param server_fd The file descriptor for the server socket.
 * @param file_name The name of the file to be transferred.
 */
void accept_client(int server_fd, const char *file_name) {
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int client_socket;

    while (1) {
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            log_error("Accept failed");
            continue;
        }

        log_info("Client connected");
        handle_client(client_socket);
    }
}

/**
 * @brief Handles communication with a client.
 *
 * This function receives messages from the client and echoes them back. It also handles
 * client disconnection and errors.
 *
 * @param socket_desc A pointer to the socket descriptor for the client connection.
 */
void connection_handler(void *socket_desc) {
    int sock = *(int*)socket_desc;
    int read_size;
    char client_message[2000];

    while((read_size = recv(sock, client_message, 2000, 0)) > 0) {
        write(sock, client_message, strlen(client_message));
    }

    if(read_size == 0) {
        log_info("Client disconnected");
    } else if(read_size == -1) {
        log_error("recv failed");
    }
    
    free(socket_desc);
}