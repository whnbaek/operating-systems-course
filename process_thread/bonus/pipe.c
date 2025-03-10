#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>
#include <string.h>

#define STACK_SIZE 1024 * 1024  // 1 MB stack size
#define MESSAGE "Hello from the parent process!"

// The function executed by the LWP (child thread)
int thread_function(void *arg) {
    int *pipe_fd = (int *)arg;  // File descriptors for the pipe
    close(pipe_fd[1]);          // Close the write end of the pipe

    char buffer[128];
    ssize_t bytes_read = read(pipe_fd[0], buffer, sizeof(buffer) - 1);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';  // Null-terminate the string
        printf("Child received: %s\n", buffer);
    } else {
        perror("read");
    }

    close(pipe_fd[0]);  // Close the read end of the pipe
    return 0;
}

int main() {
    char *stack;          // Pointer to the stack for the new thread
    char *stackTop;       // Top of the stack (to pass to clone)
    pid_t pid;
    int pipe_fd[2];       // File descriptors for the pipe

    // Create a pipe
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        return 1;
    }

    // Allocate memory for the thread's stack
    stack = malloc(STACK_SIZE);
    if (stack == NULL) {
        perror("malloc");
        return 1;
    }

    // Set the stack top (stack grows downward, so we set the top to the end of the allocated space)
    stackTop = stack + STACK_SIZE;

    // Create the LWP (thread) using clone
    pid = clone(thread_function, stackTop, SIGCHLD, pipe_fd);
    if (pid == -1) {
        perror("clone");
        free(stack);
        return 1;
    }

    // Parent process writes to the pipe
    close(pipe_fd[0]);  // Close the read end of the pipe
    write(pipe_fd[1], MESSAGE, strlen(MESSAGE));
    close(pipe_fd[1]);  // Close the write end of the pipe

    // Wait for the child thread (LWP) to finish
    waitpid(pid, NULL, 0);

    // Free the allocated stack
    free(stack);

    printf("Main process finished.\n");
    return 0;
}

