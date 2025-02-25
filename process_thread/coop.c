#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include <signal.h>
#include <ucontext.h>

#define STACK_SIZE 1024 * 1024  // 1 MB stack size
#define MAX_THREADS 2          // Maximum number of threads

typedef struct thread {
    ucontext_t context;      // Thread context
    char *stack;             // Stack for the thread
    int active;              // Is the thread active?
} thread_t;

thread_t threads[MAX_THREADS];
int current_thread = 0;

// Function to yield control to the next thread
void yield() {
    int prev_thread = current_thread;
    current_thread = (current_thread + 1) % MAX_THREADS;

    // Skip inactive threads
    int iter = MAX_THREADS;
    while (!threads[current_thread].active) {
        current_thread = (current_thread + 1) % MAX_THREADS;
	if (iter==MAX_THREADS) {
		printf("No Active threads, ending");
		exit(0);
	}
    }

    // Swap context to the next thread
    swapcontext(&threads[prev_thread].context, &threads[current_thread].context);
}

// Thread function
void thread_function(void *arg) {
    int id = *(int *)arg;

    for (int i = 0; i < 5; i++) {
        printf("Thread %d: iteration %d\n", id, i);
        yield();  // Yield control to the next thread
    }

    // Mark thread as inactive
    threads[id].active = 0;
    printf("Thread %d finished.\n", id);
    yield();  // Yield to allow other threads to finish
}

__attribute__((noreturn))
void main() {
    // Initialize threads
    for (int i = 0; i < MAX_THREADS; i++) {
        threads[i].stack = malloc(STACK_SIZE);
        if (!threads[i].stack) {
            perror("malloc");
            exit(1);
        }

        threads[i].active = 1;
        getcontext(&threads[i].context);
        threads[i].context.uc_stack.ss_sp = threads[i].stack;
        threads[i].context.uc_stack.ss_size = STACK_SIZE;
        threads[i].context.uc_link = NULL;

        int *arg = malloc(sizeof(int));
        *arg = i;  // Pass thread ID as argument
        makecontext(&threads[i].context, (void (*)(void))thread_function, 1, arg);
    }

    // Start the first thread
    setcontext(&threads[0].context);

    while(1);
}

