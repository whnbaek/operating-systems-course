#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Shared resource
int counter = 0;
pthread_spinlock_t lock;  // pthread spinlock

void *thread_func(void *arg) {
    for (int i = 0; i < 5; i++) {
        pthread_spin_lock(&lock);  // Acquire the spinlock

        // Critical Section
        int local = counter;
        usleep(10000); // Simulate work
        counter = local + 1;
        printf("Thread %ld incremented counter to %d\n", (long)arg, counter);

        pthread_spin_unlock(&lock); // Release the spinlock
        usleep(5000); // Simulate work outside critical section
    }
    return NULL;
}

int main() {
    pthread_t threads[5];

    // Initialize the spinlock
    pthread_spin_init(&lock, PTHREAD_PROCESS_PRIVATE);

    // Create threads
    for (long i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, thread_func, (void *)i);
    }

    // Join threads
    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy the spinlock
    pthread_spin_destroy(&lock);

    printf("Final counter value: %d\n", counter);
    return 0;
}

