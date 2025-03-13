#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 80
#define BUFFER_SIZE 4096

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    struct hostent *host;
    char request[] = "GET / HTTP/1.1\r\nHost: baidu.com\r\nConnection: close\r\n\r\n";
    char buffer[BUFFER_SIZE];

    // Resolve hostname to IP address
    if ((host = gethostbyname("baidu.com")) == NULL) {
        perror("Failed to resolve hostname");
        exit(EXIT_FAILURE);
    }

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    // Set up server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    memcpy(&serv_addr.sin_addr, host->h_addr, host->h_length);

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Send HTTP GET request
    send(sock, request, strlen(request), 0);
    printf("HTTP GET request sent to baidu.com\n");

    // Receive response
    int bytes_received;
    while ((bytes_received = read(sock, buffer, BUFFER_SIZE - 1)) > 0) {
        buffer[bytes_received] = '\0';  // Null-terminate the buffer
        printf("%s", buffer);
    }

    if (bytes_received < 0) {
        perror("Read error");
    }

    // Close the socket
    close(sock);

    return 0;
}

