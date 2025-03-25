#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>

pthread_mutex_t mutex;

void *low_priority_thread(void *arg) {
    printf("Low-priority thread: Locking mutex\n");
    pthread_mutex_lock(&mutex);
    printf("Low-priority thread: Holding mutex for 3 seconds\n");
    sleep(3);
    printf("Low-priority thread: Unlocking mutex\n");
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *high_priority_thread(void *arg) {
    sleep(1); // Ensure low-priority thread locks the mutex first
    printf("High-priority thread: Attempting to lock mutex\n");
    pthread_mutex_lock(&mutex);
    printf("High-priority thread: Acquired mutex\n");
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    pthread_t low, high;
    pthread_mutexattr_t attr;
    struct sched_param param;

    // Initialize mutex attributes
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_INHERIT);
    pthread_mutex_init(&mutex, &attr);

    // Set scheduling policy to real-time (SCHED_FIFO)
    param.sched_priority = 10;
    pthread_setschedparam(pthread_self(), SCHED_FIFO, &param);

    // Create low-priority thread
    param.sched_priority = 5;
    pthread_create(&low, NULL, low_priority_thread, NULL);
    pthread_setschedparam(low, SCHED_FIFO, &param);

    // Create high-priority thread
    param.sched_priority = 20;
    pthread_create(&high, NULL, high_priority_thread, NULL);
    pthread_setschedparam(high, SCHED_FIFO, &param);

    // Wait for threads to finish
    pthread_join(low, NULL);
    pthread_join(high, NULL);

    // Destroy mutex
    pthread_mutex_destroy(&mutex);
    pthread_mutexattr_destroy(&attr);

    return 0;
}

