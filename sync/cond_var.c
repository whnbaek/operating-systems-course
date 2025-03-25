#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int data_ready = 0; // Shared resource flag

void *producer(void *arg) {
    sleep(2); // Simulate work
    pthread_mutex_lock(&mutex);
    data_ready = 1; // Mark data as ready
    printf("Producer: Data is ready\n");
    pthread_cond_signal(&cond); // Notify the consumer
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *consumer(void *arg) {
    pthread_mutex_lock(&mutex);
    while (data_ready == 0) { // Wait while data is not ready
        printf("Consumer: Waiting for data...\n");
        pthread_cond_wait(&cond, &mutex); // Unlocks mutex and waits
    }
    printf("Consumer: Consumed the data\n");
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    pthread_t prod, cons;

    pthread_create(&cons, NULL, consumer, NULL);
    pthread_create(&prod, NULL, producer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}

