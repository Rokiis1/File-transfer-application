#ifndef LOGGER_H
#define LOGGER_H

int create_log_directory();
void log_info(const char *format, ...);
void log_error(const char *format, ...);

#endif