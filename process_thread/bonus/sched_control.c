#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>
#include <errno.h>

#define STACK_SIZE 1024 * 1024  // 1 MB stack size

// The function executed by the LWP (child thread)
int thread_function(void *arg) {
    printf("Child thread running with custom scheduling policy!\n");
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

    // Set the stack top (stack grows downward)
    stackTop = stack + STACK_SIZE;

    // Create the LWP (thread) using clone
    pid = clone(thread_function, stackTop, SIGCHLD, NULL);
    if (pid == -1) {
        perror("clone");
        free(stack);
        return 1;
    }

    // Set the scheduling policy and priority for the child thread
    struct sched_param param;
    param.sched_priority = 10;  // Priority for SCHED_FIFO or SCHED_RR

    if (sched_setscheduler(pid, SCHED_FIFO, &param) == -1) {
        perror("sched_setscheduler");
    } else {
        printf("Scheduling policy set to SCHED_FIFO for child thread.\n");
    }

    // Wait for the child thread to finish
    waitpid(pid, NULL, 0);

    // Free the allocated stack
    free(stack);

    printf("Main process finished.\n");
    return 0;
}

