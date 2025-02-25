#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    // Define arguments (argv)
    char *argv[] = {
        "./printer", // Program name (conventionally the first argument)
        "arg1",      // First argument
        "arg2",      // Second argument
        NULL         // Null-terminated array
    };

    // Define environment variables (envp)
    char *envp[] = {
        "VAR1=value1", // First environment variable
        "VAR2=value2", // Second environment variable
        NULL           // Null-terminated array
    };

    // Execute the program
    if (execve("./printer", argv, envp) == -1) {
        perror("execve failed");
        exit(EXIT_FAILURE);
    }

    // This point is never reached if execve is successful
    return 0;
}

