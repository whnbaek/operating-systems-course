#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int stack_var = 200; // Main process's stack variable

    // Allocate shared heap memory
    int* heap_var = (int*)malloc(sizeof(int));
    *heap_var = 10;

    printf("Main: stack_var at %p, value: %d\n", (void*)&stack_var, stack_var);
    printf("Main: heap_var at %p, value: %d\n", (void*)heap_var, *heap_var);

    pid_t pid = fork();
    if (pid == 0) { // Child process
        printf("Child: stack_var at %p, value: %d\n", (void*)&stack_var, stack_var);
        printf("Child: heap_var at %p, value: %d\n", (void*)heap_var, *heap_var);

        *heap_var = 99; // Modify shared heap variable
        stack_var = 100; // Modify local stack variable

        printf("Child (after modification):\n");
        printf("Child: stack_var at %p, value: %d\n", (void*)&stack_var, stack_var);
        printf("Child: heap_var at %p, value: %d\n", (void*)heap_var, *heap_var);

        free(heap_var);
        exit(0);
    } else { // Parent process
        wait(NULL); // Wait for child to complete

        printf("Main (after child modification):\n");
        printf("Main: stack_var at %p, value: %d\n", (void*)&stack_var, stack_var);
        printf("Main: heap_var at %p, value: %d\n", (void*)heap_var, *heap_var);

        free(heap_var);
    }
    return 0;
}

