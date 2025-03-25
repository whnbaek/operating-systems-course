#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 5
#define NUM_ITERATIONS 1000000

pthread_mutex_t mutex;  // Declare a mutex
int counter = 0;        // Shared resource

// Thread function
void* increment_counter(void* arg) {
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        pthread_mutex_lock(&mutex);  // Lock the mutex
        counter++;                   // Critical section
		for(int i=0; i <100; i++)
				counter++;
		for(int i=0; i <100; i++)
                counter--;
        pthread_mutex_unlock(&mutex); // Unlock the mutex
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];

    // Initialize the mutex
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("Mutex initialization failed");
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

    // Destroy the mutex
    pthread_mutex_destroy(&mutex);

    printf("Final counter value: %d\n", counter);  // Should be NUM_THREADS * NUM_ITERATIONS
    return EXIT_SUCCESS;
}

