#include <stdio.h>

int main(int argc, char *argv[], char *envp[]) {
    // Print command-line arguments
    printf("Number of arguments: %d\n", argc);
    printf("Arguments:\n");
    for (int i = 0; i < argc; i++) {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    // Print environment variables
    printf("\nEnvironment Variables:\n");
    for (int i = 0; envp[i] != NULL; i++) {
        printf("envp[%d]: %s\n", i, envp[i]);
    }

    return 0;
}

