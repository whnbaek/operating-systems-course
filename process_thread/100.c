#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// replace this number with however many threads you have
#define NUM_THREADS 32

void* busy_loop(void* arg) {
    while (1);  // Infinite loop to keep CPU busy
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, busy_loop, NULL) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // Join threads (this will never be reached, but prevents compiler warnings)
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

