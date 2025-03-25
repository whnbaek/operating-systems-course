#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    int fd[2];  // File descriptors for the pipe
    pid_t child1, child2;
    
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // First child (Writer 1)
    if ((child1 = fork()) == 0) {
        close(fd[0]); // Close read end
        write(fd[1], "Message from child 1\n", 22);
        close(fd[1]); // Close write end
        exit(EXIT_SUCCESS);
    }

    // Second child (Writer 2)
    if ((child2 = fork()) == 0) {
        close(fd[0]); // Close read end
        write(fd[1], "Message from child 2\n", 22);
        close(fd[1]); // Close write end
        exit(EXIT_SUCCESS);
    }

    // Parent process
    close(fd[1]); // Close write end
    char buffer[100];
    int bytesRead;

    while ((bytesRead = read(fd[0], buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytesRead] = '\0'; // Null-terminate the string
        printf("Parent received: %s", buffer);
    }

    close(fd[0]); // Close read end

    // Wait for children to finish
    wait(NULL);
    wait(NULL);

    return 0;
}

