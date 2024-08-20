#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <direct.h>
#include <stdarg.h>
#include "logger.h"

#define LOG_DIR "../logs"
#define LOG_FILE "../logs/p2p_file_sharing.log"

/**
 * @brief Creates the log directory if it does not exist.
 *
 * This function checks if the log directory exists, and if not, creates it.
 *
 * @return 0 on success, -1 on failure.
 */
int create_log_directory() {
    struct stat st;
    if (stat("../logs", &st) == -1) {
        if (_mkdir("../logs") == -1) {
            perror("Error creating log directory");
            return -1;
        }
    }
    return 0;
}

/**
 * @brief Logs a message to the log file.
 *
 * This function logs a message with a specified log level to the log file.
 *
 * @param level The log level ("INFO", "ERROR").
 * @param format The format string for the log message.
 * @param args The arguments for the format string.
 */
void log_message(const char *level, const char *format, va_list args) {
    if (create_log_directory() == -1) {
        return;
    }

    FILE *log_file = fopen(LOG_FILE, "a");
    if (log_file == NULL) {
        perror("Error opening log file");
        return;
    }

    time_t now = time(NULL);
    char *timestamp = ctime(&now);
    if (timestamp != NULL) {
        timestamp[strlen(timestamp) - 1] = '\0';
    } else {
        timestamp = "unknown time";
    }

    fprintf(log_file, "[%s] %s: ", timestamp, level);
    vfprintf(log_file, format, args);
    fprintf(log_file, "\n");

    fflush(log_file);
    fclose(log_file);
}

/**
 * @brief Logs an informational message.
 *
 * This function logs an informational message to the log file.
 *
 * @param format The format string for the log message.
 * @param ... The arguments for the format string.
 */
void log_info(const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_message("INFO", format, args);
    va_end(args);
}

/**
 * @brief Logs an error message.
 *
 * This function logs an error message to the log file.
 *
 * @param format The format string for the log message.
 * @param ... The arguments for the format string.
 */
void log_error(const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_message("ERROR", format, args);
    va_end(args);
}