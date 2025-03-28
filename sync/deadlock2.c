#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mtx;  // Non-recursive mutex

void* cause_deadlock(void* arg) {
    printf("Locking mutex first time...\n");
    pthread_mutex_lock(&mtx);  // First lock succeeds

    printf("Trying to lock mutex again...\n");
    pthread_mutex_lock(&mtx);  // Deadlock occurs here! The thread waits on itself.

    printf("This will never be printed!\n");

    pthread_mutex_unlock(&mtx);  // (Never reached)
    pthread_mutex_unlock(&mtx);  // (Never reached)
    
    return NULL;
}

int main() {
    pthread_t thread;

    // Initialize mutex
    pthread_mutex_init(&mtx, NULL);

    // Create a thread to run cause_deadlock
    pthread_create(&thread, NULL, cause_deadlock, NULL);

    // Wait for the thread (which will never finish due to deadlock)
    pthread_join(thread, NULL);

    // Destroy mutex (never reached)
    pthread_mutex_destroy(&mtx);

    return 0;
}

