#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *thread_func(void *arg) {
    // Allocate and free memory in quick succession to increase contention
    for (int i = 0; i < 1000; i++) {
        void *ptr = malloc(16);  // Allocate 16 bytes
		sleep(1); //Sleep so there is some room between caching
        free(ptr);  // Free the allocated memory immediately
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <num_threads>\n", argv[0]);
        return 1;
    }

    // Parse the number of threads from the command-line argument
    int num_threads = atoi(argv[1]);
    if (num_threads <= 0) {
        fprintf(stderr, "Please provide a valid positive number of threads.\n");
        return 1;
    }

    pthread_t threads[num_threads];

    // Spawn multiple threads
    for (long i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, thread_func, (void *)i);
    }

    // Wait for all threads to complete
    for (long i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

	while(1);
	return 0;
}

