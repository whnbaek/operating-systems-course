#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>

#define STACK_SIZE 1024 * 1024  // 1 MB stack size

// The function to be executed by the new thread (LWP)
int thread_function(void *arg) {
    printf("Hello from the LWP (thread)!\n");
    return 0;
}

int main() {
    char *stack;          // Pointer to the stack for the new thread
    char *stackTop;       // Top of the stack (to pass to clone)
    pid_t pid;

    // Allocate memory for the thread's stack
    stack = malloc(STACK_SIZE);
    if (stack == NULL) {
        perror("malloc");
        return 1;
    }

    // Set the stack top (stack grows downward, so we set the top to the end of the allocated space)
    stackTop = stack + STACK_SIZE;

    // Create the LWP (thread) using clone
    pid = clone(thread_function, stackTop, SIGCHLD, NULL);
    if (pid == -1) {
        perror("clone");
        free(stack);
        return 1;
    }

    // Parent process waits for the LWP (thread) to finish
    waitpid(pid, NULL, 0);

    // Free the allocated stack
    free(stack);

    printf("Main process finished.\n");
    return 0;
}

