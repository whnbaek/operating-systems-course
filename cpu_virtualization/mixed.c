#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
void setup_cgroup_for_process();
void* thread_func(void* arg) {

	int cpu = 0;
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(cpu, &cpuset);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);

    int id = *(int*)arg;
    struct sched_param param;
    int policy;
    pthread_getschedparam(pthread_self(), &policy, &param);

    const char* policy_str =
        policy == SCHED_FIFO ? "SCHED_FIFO" :
        policy == SCHED_RR   ? "SCHED_RR" :
                               "SCHED_OTHER";


	printf("Thread %ld running on CPU %d\n", pthread_self(), sched_getcpu());

    for (int i = 0; i < 5; ++i) {
        printf("Thread %d [%s, priority %d] running iteration %d\n",
               id, policy_str, param.sched_priority, i);
		volatile int waste = 0;
		for (int j = 0; j < 10000000; ++j) waste += j;
    }
    return NULL;
}

int main() {
    pthread_t threads[3];
    int ids[] = {0, 1, 2};

    struct sched_param param;
    pthread_attr_t attr;

    // Create threads with different scheduling policies
    for (int i = 0; i < 3; ++i) {
		cpu_set_t cpuset;

        pthread_attr_init(&attr);

        CPU_ZERO(&cpuset);
        CPU_SET(0, &cpuset);  // Pin to CPU 0

        if (pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset) != 0) {
            perror("pthread_attr_setaffinity_np");
        }

        pthread_attr_init(&attr);

        if (i == 0) {
            pthread_attr_setschedpolicy(&attr, SCHED_OTHER);
        } else if (i == 1) {
            pthread_attr_setschedpolicy(&attr, SCHED_RR);
            param.sched_priority = 10;
            pthread_attr_setschedparam(&attr, &param);
            pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
        } else {
            pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
            param.sched_priority = 20;
            pthread_attr_setschedparam(&attr, &param);
            pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
        }

        if (pthread_create(&threads[i], &attr, thread_func, &ids[i]) != 0) {
            perror("pthread_create");
        }
        pthread_attr_destroy(&attr);
    }

    for (int i = 0; i < 3; ++i) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

