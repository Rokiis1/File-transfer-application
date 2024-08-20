#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#include <direct.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#endif
#include "connection_handler.h"
#include "file_transfer.h"
#include "logger.h"
#include "protocol.h"

#define RECEIVED_FILES_DIR "./received_files/"
#define SEND_FILES_DIR "./send_files/"


/**
 * @brief Creates a directory if it does not already exist.
 *
 * This function checks if a directory exists, and if not, creates it.
 *
 * @param directory The path of the directory to create.
 */
void create_directory_if_not_exists(const char *directory) {
    struct stat st = {0};
    if (stat(directory, &st) == -1) {
#ifdef _WIN32
        if (_mkdir(directory) == 0) {
            log_info("Directory created: %s", directory);
        } else {
            log_error("Failed to create directory: %s", directory, strerror(errno));
        }
#else
        if (mkdir(directory, 0700) == 0) {
            log_info("Directory created: %s", directory);
        } else {
            log_error("Failed to create directory: %s", directory, strerror(errno));
        }
#endif
    } else {
        log_info("Directory already exists: %s", directory);
    }
}

/**
 * @brief Initializes the directories for file transfer.
 *
 * This function ensures that the directories for received and send files exist.
 */
void initialize_directories() {
    create_directory_if_not_exists(RECEIVED_FILES_DIR);
    create_directory_if_not_exists(SEND_FILES_DIR);
}

/**
 * @brief Sends a file over a socket.
 *
 * This function opens a file for reading and sends its contents over the specified socket.
 *
 * @param socket The socket to send the file over.
 * @param file_name The name of the file to send.
 */
void send_file(int socket, const char *file_name) {
    char absolute_path[256];
    snprintf(absolute_path, sizeof(absolute_path), "%s%s", SEND_FILES_DIR, file_name);

    int file = open(absolute_path, O_RDONLY);
    if (file < 0) {
        log_error("Error opening file for reading %s, Error: %s", absolute_path, strerror(errno));
        return;
    }

    char buffer[BUFFER_SIZE];
    int bytes_read;
    while ((bytes_read = read(file, buffer, BUFFER_SIZE)) > 0) {
        if (send(socket, buffer, bytes_read, 0) < 0) {
            log_error("Error sending file %s, Error: %s", absolute_path, strerror(errno));
            close(file);
            return;
        }
    }

    if (bytes_read < 0) {
        log_error("Error reading file %s, Error: %s", absolute_path, strerror(errno));
    }

    close(file);
    log_info("File sent successfully %s", absolute_path);
}

/**
 * @brief Receives a file over a socket.
 *
 * This function receives data from the specified socket and writes it to a file.
 *
 * @param socket The socket to receive the file from.
 * @param file_name The name of the file to receive.
 */
void receive_file(int socket, const char *file_name) {
    if (file_name == NULL) {
        log_error("File name is NULL");
        return;
    }

    char absolute_path[256];
    int snprintf_result = snprintf(absolute_path, sizeof(absolute_path), "%s%s", RECEIVED_FILES_DIR, file_name);
    if (snprintf_result < 0 || snprintf_result >= sizeof(absolute_path)) {
        log_error("Error constructing file path for %s", file_name);
        return;
    }

    int file = open(absolute_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (file < 0) {
        log_error("Error opening file for writing %s, Error: %s", absolute_path, strerror(errno));
        return;
    } 

    char buffer[BUFFER_SIZE];
    int bytes_received;
    while ((bytes_received = recv(socket, buffer, BUFFER_SIZE, 0)) > 0) {
        if (write(file, buffer, bytes_received) < 0) {
            log_error("Error writing to file %s, Error: %s", absolute_path, strerror(errno));
            close(file);
            return;
        }
    }

    if (bytes_received == 0) {
        log_info("File received successfully %s", absolute_path);
    } else if (bytes_received < 0) {
        log_error("Error receiving file %s, recv returned %d, Error: %s", absolute_path, bytes_received, strerror(errno));
    }

    close(file);
    log_info("File closed: %s", absolute_path);
}