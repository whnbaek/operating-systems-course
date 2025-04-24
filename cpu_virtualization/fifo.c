#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <string.h>

#define NUM_THREADS 5
#define FIFO_PRIORITY 80

void pin_main_thread_to_cpu0() {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);

    if (sched_setaffinity(0, sizeof(cpu_set_t), &cpuset) != 0) {
        perror("sched_setaffinity (main)");
        exit(1);
    }
}

void* thread_func(void* arg) {
    int id = *(int*)arg;

    for (int i = 0; i < 5; ++i) {
        printf("Thread %d running on CPU %d\n", id, sched_getcpu());
        sleep(1);
        sched_yield();  // Yield to allow fair FIFO rotation
    }

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    pthread_attr_t attr;
    struct sched_param param;
    cpu_set_t cpuset;
    int ids[NUM_THREADS];

	pin_main_thread_to_cpu0();

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_attr_init(&attr);

        // Set SCHED_FIFO policy and same priority
        pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
        param.sched_priority = FIFO_PRIORITY;
        pthread_attr_setschedparam(&attr, &param);
        pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);

        // Set CPU affinity to CPU 0
        CPU_ZERO(&cpuset);
        CPU_SET(0, &cpuset);
        if (pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset) != 0) {
            perror("pthread_attr_setaffinity_np");
        }

        ids[i] = i;
        if (pthread_create(&threads[i], &attr, thread_func, &ids[i]) != 0) {
            perror("pthread_create");
        }

        pthread_attr_destroy(&attr);
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

