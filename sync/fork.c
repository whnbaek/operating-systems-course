#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();  // Create a new process

    if (pid == -1) {
        // Fork failed
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {
        // This block is executed by the child process
        printf("Hello from the child process! PID: %d\n", getpid());
    } else {
        // This block is executed by the parent process
        printf("Hello from the parent process! Child PID: %d\n", pid);
    }

    return 0;
}

