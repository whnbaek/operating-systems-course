#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    // Define the arguments to pass to the new program
    char *argv[] = {
        "ls",      // The program name (usually the first argument is the program name itself)
        "-l",      // Argument to the program
        "/home",   // Directory to list
        NULL       // Null-terminated list
    };

    // Define a custom environment
    char *envp[] = {
        "MY_CUSTOM_ENV=12345",   // Custom environment variable
        "PATH=/bin:/usr/bin",    // Custom PATH variable (could be extended)
        NULL                     // Null-terminated list
    };

    // Use execvpe to execute the "ls" command with custom environment
    if (execvpe("/bin/ls", argv, envp) == -1) {
        perror("execvpe failed");
        exit(EXIT_FAILURE);
    }

    // This point will never be reached unless execvpe fails
    return 0;
}

