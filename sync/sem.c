#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_THREADS 3

sem_t semaphore;

void* worker(void* arg) {
    int id = *(int*)arg;
    printf("Thread %d waiting for semaphore...\n", id);
    sem_wait(&semaphore);

    printf("Thread %d entered critical section.\n", id);
    sleep(2); // Simulate work in the critical section
    printf("Thread %d leaving critical section.\n", id);

    sem_post(&semaphore);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    
    // Initialize semaphore with a value of 1 (binary semaphore)
    sem_init(&semaphore, 0, 1);
    
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i + 1;
        pthread_create(&threads[i], NULL, worker, &thread_ids[i]);
    }
    
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    sem_destroy(&semaphore);
    printf("All threads finished execution.\n");
    return 0;
}

