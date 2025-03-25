#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main() {
    int pipefd[2];  // File descriptors for the pipe (pipefd[0] for reading, pipefd[1] for writing)
    pid_t pid;
    char buffer[100];

    // Create a pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    pid = fork(); // Create a child process

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {  
        // Child process
        close(pipefd[1]); // Close unused write end

        // Read from pipe
        ssize_t num_bytes = read(pipefd[0], buffer, sizeof(buffer) - 1);
        if (num_bytes > 0) {
            buffer[num_bytes] = '\0';  // Null-terminate string
            printf("Child received: %s\n", buffer);
        } else {
            perror("read");
        }

        close(pipefd[0]); // Close read end
    } else {  
        // Parent process
        close(pipefd[0]); // Close unused read end

        const char *message = "Hello from parent!";
        write(pipefd[1], message, strlen(message));

        close(pipefd[1]); // Close write end
    }

    return 0;
}

