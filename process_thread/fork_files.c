#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {
    // No stack or heap variables, using direct values for demonstration

    // Open a shared file descriptor
    int fd = open("shared_file.txt", O_CREAT | O_RDWR | O_TRUNC, 0666);
    if (fd < 0) {
        perror("open");
        exit(1);
    }
    dprintf(fd, "Initial value in file\n");


    pid_t pid = fork();
    if (pid == 0) { // Child process
        dprintf(fd, "Child writing to file\n");
        close(fd);
        exit(0);
    } else { // Parent process
        wait(NULL); // Wait for child to complete
        dprintf(fd, "Parent writing to file\n");
        close(fd);
    }
    return 0;
}

