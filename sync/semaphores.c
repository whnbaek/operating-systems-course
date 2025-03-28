#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_RESOURCES 3  // Number of available resources
#define NUM_THREADS 10    // Total number of worker threads

sem_t resource_semaphore;  // Counting semaphore for managing resources

void* worker(void* arg) {
    int thread_id = *(int*)arg;
    free(arg);  // Free the dynamically allocated memory

    printf("Thread %d: Waiting for a resource...\n", thread_id);

    // Wait (decrement) the semaphore to acquire a resource
    sem_wait(&resource_semaphore);

    // Critical section (using the shared resource)
    printf("Thread %d: Acquired a resource! Working...\n", thread_id);
    sleep(2);  // Simulate work being done

    // Release (increment) the semaphore after work is done
    printf("Thread %d: Releasing the resource...\n", thread_id);
    sem_post(&resource_semaphore);

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];

    // Initialize the semaphore with MAX_RESOURCES available
    sem_init(&resource_semaphore, 0, MAX_RESOURCES);

    // Create multiple worker threads
    for (int i = 0; i < NUM_THREADS; i++) {
        int* thread_id = malloc(sizeof(int));  // Avoid race condition with loop variable
        *thread_id = i;
        pthread_create(&threads[i], NULL, worker, thread_id);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy the semaphore
    sem_destroy(&resource_semaphore);

    printf("All threads completed. Resources released.\n");
    return 0;
}

