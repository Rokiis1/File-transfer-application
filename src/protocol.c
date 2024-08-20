#include <string.h>
#include "protocol.h"

/**
 * @brief Serializes a Message structure into a buffer.
 *
 * This function copies the contents of a Message structure into a buffer for transmission or storage.
 *
 * @param message A pointer to the Message structure to serialize.
 * @param buffer A pointer to the buffer where the serialized data will be stored.
 */
void serialize_message(Message *message, char *buffer) {
    memcpy(buffer, message, sizeof(Message));
}

/**
 * @brief Deserializes a buffer into a Message structure.
 *
 * This function copies the contents of a buffer into a Message structure.
 *
 * @param buffer A pointer to the buffer containing the serialized data.
 * @param message A pointer to the Message structure where the deserialized data will be stored.
 */
void deserialize_message(char *buffer, Message *message) {
    memcpy(message, buffer, sizeof(Message));
}