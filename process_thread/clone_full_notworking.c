#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <linux/sched.h>
#include <string.h>

// Stack size for the child process
#define STACK_SIZE 1024 * 1024

// Structure to simulate TLS
struct tls_data {
    int start;
    int end;
    int result;
};

// Function executed by the child process
int child_function(void *arg) {
    struct tls_data *tls = (struct tls_data *)arg;

    // Perform a sum calculation in the range [start, end]
    tls->result = 0;
    for (int i = tls->start; i <= tls->end; ++i) {
        tls->result += i;
    }

    printf("Child process: PID = %d, Parent PID = %d\n", getpid(), getppid());
    printf("Child calculated sum from %d to %d: %d\n", tls->start, tls->end, tls->result);

    return 0;  // Exit the child process
}

int main() {
    char *stack;              // Pointer to the stack
    char *stack_top;          // Top of the stack (stack grows downward)
    pid_t child_pid, ptid, ctid;

    // Allocate memory for the stack
    stack = malloc(STACK_SIZE);
    if (!stack) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // Set the top of the stack
    stack_top = stack + STACK_SIZE;

    // TLS structure for the child process
    struct tls_data tls;
    tls.start = 1;  // Range start
    tls.end = 100;  // Range end
    tls.result = 0; // Initialize result

    // Use clone to create a new process
    int flags = SIGCHLD | CLONE_PARENT_SETTID | CLONE_CHILD_SETTID | CLONE_SETTLS;
    child_pid = clone(child_function, stack_top, flags, &tls, &ptid, &tls, &ctid);

    if (child_pid == -1) {
        perror("clone");
        free(stack);
        exit(EXIT_FAILURE);
    }

    printf("Parent process: PID = %d, Child PID = %d\n", getpid(), child_pid);

    // Wait for the child process to finish
    if (waitpid(child_pid, NULL, 0) == -1) {
        perror("waitpid");
        free(stack);
        exit(EXIT_FAILURE);
    }

    printf("Parent sees child's thread ID (via CLONE_PARENT_SETTID): %d\n", ptid);
    printf("Parent reads child's calculated result from TLS: %d\n", tls.result);

    // Free the stack memory
    free(stack);

    return 0;
}

