#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    while (1) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork failed");
            exit(1);
        }

        if (pid == 0) {  // Child process
            printf("Child created: PID=%d, Parent PID=%d\n", getpid(), getppid());
            _exit(0);  // Child exits immediately to prevent fork bomb
        } else {  // Parent process
            printf("Parent PID=%d created Child PID=%d\n", getpid(), pid);
        }

        sleep(1);  // Sleep before forking again
    }

    return 0;
}

