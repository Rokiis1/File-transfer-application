#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")
#define close closesocket
#else
#include <arpa/inet.h>
#include <unistd.h>
#endif

#include "file_transfer.h"
#include "logger.h"
#include "connection_handler.h"
#include "client_handler.h"

/**
 * @brief Main function for the client application.
 *
 * This function initializes a connection to the server, sends a file name, transfers the file,
 * and then receives a response from the server.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return 0 on success, 1 on failure.
 */
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server-ip> <file-to-transfer>\n", argv[0]);
        return 1;
    }

    char *server_ip = argv[1];
    char *file_to_transfer = argv[2];

#ifdef _WIN32
    initialize_winsock();
#endif

    int sock;
    struct sockaddr_in server;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        log_error("Socket creation failed");
        return 1;
    }

    server.sin_addr.s_addr = inet_addr(server_ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        log_error("Connection failed");
        return 1;
    }

    log_info("Connected to server");

    if (send(sock, file_to_transfer, strlen(file_to_transfer) + 1, 0) < 0) {
        log_error("Failed to send file name");
        close(sock);
        return 1;
    }

    send_file(sock, file_to_transfer);

#ifdef _WIN32
    shutdown(sock, SD_SEND);
#else
    shutdown(sock, SHUT_WR);
#endif

    receive_file(sock, file_to_transfer);

    close(sock);
#ifdef _WIN32
    WSACleanup();
#endif

    log_info("File transfer complete");
    return 0;
}