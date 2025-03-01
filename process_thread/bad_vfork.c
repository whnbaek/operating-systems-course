#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int x = 42;
    pid_t pid = vfork();

    if (pid < 0) {
        perror("vfork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // Child process
        x = 99;  // Modifying parent's memory—UNDEFINED BEHAVIOR!
        printf("Child: x = %d\n", x);
        return 0; // Use _exit() to avoid corrupting the parent's stack
    } else { // Parent process
        printf("Parent: x = %d\n", x); // May print unexpected value!
    }

    return 0;
}

