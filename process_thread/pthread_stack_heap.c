#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int* heap_var;  // Global heap variable (shared across threads)

void* thread_function(void* arg) {
    int stack_var = 42;  // Local stack variable (separate for each thread)

    printf("Thread: stack_var at %p, value: %d\n", (void*)&stack_var, stack_var);
    printf("Thread: heap_var at %p, value: %d\n", (void*)heap_var, *heap_var);

    *heap_var = 99;  // Modify shared heap variable
    stack_var = 100;  // Modify local stack variable

    printf("Thread (after modification):\n");
    printf("Thread: stack_var at %p, value: %d\n", (void*)&stack_var, stack_var);
    printf("Thread: heap_var at %p, value: %d\n", (void*)heap_var, *heap_var);

    return NULL;
}

int main() {
    heap_var = malloc(sizeof(int));  // Allocate shared heap variable
    *heap_var = 10;  

    int stack_var = 200;  // Main thread's stack variable

    printf("Main: stack_var at %p, value: %d\n", (void*)&stack_var, stack_var);
    printf("Main: heap_var at %p, value: %d\n", (void*)heap_var, *heap_var);

    pthread_t thread;
    pthread_create(&thread, NULL, thread_function, NULL);
    pthread_join(thread, NULL);

    printf("Main (after thread modification):\n");
    printf("Main: stack_var at %p, value: %d\n", (void*)&stack_var, stack_var);
    printf("Main: heap_var at %p, value: %d\n", (void*)heap_var, *heap_var);

    free(heap_var);
    return 0;
}

