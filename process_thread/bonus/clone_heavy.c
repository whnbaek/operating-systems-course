#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#define STACK_SIZE 1024 * 1024  // Stack size for child process

int shared_var = 42;  // Shared variable

int child_func(void *arg) {
    printf("Child: PID=%d, PPID=%d\n", getpid(), getppid());
    
    // Modify the shared variable
    shared_var += 10;
    printf("Child: Modified shared_var = %d\n", shared_var);

    // Write to a shared file descriptor
    int *fd = (int *)arg;
    dprintf(*fd, "Child: Writing to shared file\n");

    return 0;
}

int main() {
    char *stack = malloc(STACK_SIZE);
    if (!stack) {
        perror("malloc");
        exit(1);
    }
    
    // Open a file and get a file descriptor
    int fd = open("shared_file.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    // Clone with CLONE_VM and CLONE_FILES
    pid_t pid = clone(child_func, stack + STACK_SIZE, SIGCHLD, &fd);
    if (pid == -1) {
        perror("clone");
        exit(1);
    }

    // Wait for the child to finish
    waitpid(pid, NULL, 0);

    // Check the shared variable
    printf("Parent: shared_var after child modified it = %d\n", shared_var);

    // Write to the same file descriptor
    dprintf(fd, "Parent: Writing to shared file\n");

    // Close file and clean up
    close(fd);
    free(stack);

    return 0;
}

