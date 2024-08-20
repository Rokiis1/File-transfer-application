#ifndef PROTOCOL_H
#define PROTOCOL_H

#define BUFFER_SIZE 1024

typedef enum {
    FILE_REQUEST,
    FILE_CHUNK,
    ACKNOWLEDGEMENT
} MessageType;

typedef struct {
    MessageType type;
    char payload[BUFFER_SIZE];
} Message;

void serialize_message(Message *message, char *buffer);
void deserialize_message(char *buffer, Message *message);

#endif