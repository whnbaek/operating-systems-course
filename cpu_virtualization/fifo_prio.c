#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#define LOW_PRI  10
#define HIGH_PRI 80
#define WORK (50000000 * 10)
void burn_cpu(const char* tag, int iter) {
    volatile unsigned long junk = 0;
    for (int i = 0; i < WORK; ++i) {
        junk += i;
        if (i % 10000000 == 0) {
            printf("%s: burning iteration %d\n", tag, iter++);
        }
    }
}

void* high_prio_thread(void* arg) {
    printf("HIGH-priority thread started (prio %d), CPU %d\n", HIGH_PRI, sched_getcpu());
    for (int i = 0; i < 3; ++i) {
        burn_cpu("HIGH-priority", i);
    }
    return NULL;
}

void* low_prio_thread(void* arg) {
    printf("LOW-priority thread started (prio %d), CPU %d\n", LOW_PRI, sched_getcpu());

    // Create higher-priority thread from inside this one
    pthread_t t;
    pthread_attr_t attr;
    struct sched_param param;
    cpu_set_t cpuset;

    pthread_attr_init(&attr);
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    param.sched_priority = HIGH_PRI;
    pthread_attr_setschedparam(&attr, &param);
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);

    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);
    pthread_attr_setaffinity_np(&attr, sizeof(cpuset), &cpuset);

    pthread_create(&t, &attr, high_prio_thread, NULL);
    pthread_attr_destroy(&attr);

    for (int i = 0; i < 3; ++i) {
        burn_cpu("LOW-priority", i);
    }

    pthread_join(t, NULL);
    return NULL;
}

int main() {
    pthread_t t0;
    pthread_attr_t attr;
    struct sched_param param;
    cpu_set_t cpuset;

    // Pin main thread to CPU 0
    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);
    sched_setaffinity(0, sizeof(cpuset), &cpuset);

    // Create low-priority thread
    pthread_attr_init(&attr);
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    param.sched_priority = LOW_PRI;
    pthread_attr_setschedparam(&attr, &param);
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setaffinity_np(&attr, sizeof(cpuset), &cpuset);

    pthread_create(&t0, &attr, low_prio_thread, NULL);
    pthread_attr_destroy(&attr);

    pthread_join(t0, NULL);
    return 0;
}

