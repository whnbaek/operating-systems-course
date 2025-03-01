#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>

int fd; // Shared file descriptor

// Function to be executed by the child thread
void* child_thread_func(void* arg) {
    dprintf(fd, "Child writing to file\n");
	close(fd);
    return NULL;
}

int main() {
    // Open a shared file descriptor
    fd = open("shared_file.txt", O_CREAT | O_RDWR | O_TRUNC, 0666);
    if (fd < 0) {
        perror("open");
        exit(1);
    }
    dprintf(fd, "Initial value in file\n");

    pthread_t child_thread;
    // Create the child thread
    if (pthread_create(&child_thread, NULL, child_thread_func, NULL) != 0) {
        perror("pthread_create");
        exit(1);
    }

    // Parent thread writing to file
    dprintf(fd, "Parent writing to file\n");

    // Wait for the child thread to finish
    pthread_join(child_thread, NULL);

	// Parent thread writing to file
    dprintf(fd, "Parent writing again to file\n");

    // Close the file descriptor after both threads are done
    close(fd);
    return 0;
}

