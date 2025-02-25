#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    pid_t pid = vfork();  // Create a new process

    if (pid == -1) {
        // If vfork fails
        perror("vfork failed");
        return 1;
    }

    if (pid == 0) {
        // This block is executed by the child process
        printf("Hello from the child process! PID: %d\n", getpid());
        // Child process calls exec to replace its image
        execlp("/bin/ls", "ls", "-l", (char *)NULL);
        // If execlp fails, print an error message
        perror("execlp failed");
        exit(1);  // Exit child process if exec fails
    } else {
        // This block is executed by the parent process
        printf("Hello from the parent process! Child PID: %d\n", pid);
        // Wait for the child process to finish (optional)
        wait(NULL);
    }

    return 0;
}

