#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a struct with a zero-length array
struct DataPacket {
    int id;
    char payload[0]; // Zero-length array
};

int main() {
    const char *message = "Hello, zero-length arrays!";
    size_t message_length = strlen(message) + 1; // Include null terminator

    // Allocate memory for the struct and the message
    struct DataPacket *packet = malloc(sizeof(struct DataPacket) + message_length);

    if (!packet) {
        perror("Failed to allocate memory");
        return 1;
    }

    // Assign an ID and copy the message into the payload
    packet->id = 1;
    memcpy(packet->payload, message, message_length);

    // Print the data
    printf("Packet ID: %d\n", packet->id);
    printf("Payload: %s\n", packet->payload);

    // Free allocated memory
    free(packet);

    return 0;
}

