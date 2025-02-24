#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 5
#define NUM_ITERATIONS 1000000

pthread_spinlock_t spinlock; // Declare a spinlock
int counter = 0;             // Shared resource

// Thread function
void* increment_counter(void* arg) {
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        pthread_spin_lock(&spinlock);  // Lock the spinlock (busy-wait)
        counter++;                     // Critical section
        pthread_spin_unlock(&spinlock); // Unlock the spinlock
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];

    // Initialize the spinlock
    if (pthread_spin_init(&spinlock, PTHREAD_PROCESS_PRIVATE) != 0) {
        perror("Spinlock initialization failed");
        return EXIT_FAILURE;
    }

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, increment_counter, NULL) != 0) {
            perror("Thread creation failed");
            return EXIT_FAILURE;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy the spinlock
    pthread_spin_destroy(&spinlock);

    printf("Final counter value: %d\n", counter);  // Should be NUM_THREADS * NUM_ITERATIONS
    return EXIT_SUCCESS;
}

