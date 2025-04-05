#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "libmap/libmap.h"

int shared_variable = 42;  // Shared variable between threads

void* thread_function(void* arg) {
    printf("Child thread (before modification):\n");
    printf("Address of shared_variable: %p, Value: %d\n", (void*)&shared_variable, shared_variable);

	print_map(&shared_variable);
    shared_variable = 99;  // Modify the variable

    printf("Child thread (after modification):\n");
    printf("Address of shared_variable: %p, Value: %d\n", (void*)&shared_variable, shared_variable);

    return NULL;
}

int main() {
    printf("Main thread (before creating child thread):\n");
    printf("Address of shared_variable: %p, Value: %d\n", (void*)&shared_variable, shared_variable);

	print_map(&shared_variable);
    pthread_t thread;
    if (pthread_create(&thread, NULL, thread_function, NULL) != 0) {
        perror("pthread_create failed");
        exit(1);
    }

    // Wait for the child thread to complete
    pthread_join(thread, NULL);

    printf("Main thread (after child thread modification):\n");
    printf("Address of shared_variable: %p, Value: %d\n", (void*)&shared_variable, shared_variable);

    return 0;
}

