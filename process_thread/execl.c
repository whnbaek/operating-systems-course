#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    // Use execl to execute the "ls" command with arguments
    // "ls" is the program name, followed by arguments "-l" and "/home"
    if (execl("/bin/ls", "ls", "-l", "/home", (char *)NULL) == -1) {
        perror("execl failed");
        exit(EXIT_FAILURE);
    }

    // This point will never be reached unless execl fails
    return 0;
}

