#include <stdio.h>
#include <threads.h>  // C11 thread library

// Function to be executed by each thread
int print_hello(void* arg) {
    int thread_num = *(int*)arg;  // Get the thread number from the argument
    printf("Hello from thread %d!\n", thread_num);
    return 0;  // Return value (can be retrieved using thrd_join)
}

int main() {
    thrd_t threads[5];  // Array to hold thread identifiers
    int thread_args[5]; // Array to pass arguments to threads

    // Create 5 threads
    for (int i = 0; i < 5; i++) {
        thread_args[i] = i + 1;  // Assign thread number to each argument
        if (thrd_create(&threads[i], print_hello, &thread_args[i]) != thrd_success) {
            printf("Error creating thread %d\n", i + 1);
            return 1;
        }
    }

    // Wait for all threads to complete
    for (int i = 0; i < 5; i++) {
        thrd_join(threads[i], NULL);
    }

    printf("All threads have finished.\n");

    return 0;
}

