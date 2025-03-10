#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }

    if (pid == 0) {
        // Child process: execute a command
        execlp("ls", "ls", NULL);  // Change "ls" to "fakecmd" to simulate failure
        perror("execlp failed");    // If exec fails
        exit(1);                    // Return nonzero on failure
    } else {
        // Parent process: wait for child using waitpid
        int status;
        pid_t wpid = waitpid(pid, &status, 0);

        if (wpid == -1) {
            perror("waitpid failed");
            return 1;
        }

        if (WIFEXITED(status)) {
            int exit_code = WEXITSTATUS(status);
            if (exit_code == 0) {
                printf("Child process succeeded.\n");
            } else {
                printf("Child process failed with exit code %d.\n", exit_code);
            }
        } else {
            printf("Child did not exit normally.\n");
        }
    }

    return 0;
}

