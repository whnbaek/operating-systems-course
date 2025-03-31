#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define TARGET 1000000  // The goal each thread will contribute to reaching

volatile unsigned long long goal = 0;  // Shared global variable, volatile to prevent compiler optimizations
volatile unsigned long long per_thread_increment = 0;
pthread_spinlock_t lock;  // Mutex for synchronization

volatile int simulate_work() {
        volatile int work = 0;
        for (int j = 0; j < 100; j++) {
                work += j * j;
        }
}

void *increment_goal(void *arg) {
	int *sub_goal = arg;
    for (int i = 0; i < per_thread_increment; i++) {
        *sub_goal = *sub_goal + 1;  // Critical section
		simulate_work();
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <num_threads>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int num_threads = atoi(argv[1]);
    if (num_threads <= 0) {
        fprintf(stderr, "Number of threads must be positive.\n");
        return EXIT_FAILURE;
    }

    pthread_t threads[num_threads];
	int sub_goal[num_threads];
	memset(sub_goal, 0 ,sizeof(sub_goal));
	per_thread_increment = TARGET / num_threads;

	printf("Each thread will increment %llu times. \n", per_thread_increment);



	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, increment_goal, &sub_goal[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

	for (int i = 0; i < num_threads; i++) {
		goal += sub_goal[i];
	}


	clock_gettime(CLOCK_MONOTONIC, &end);
	double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
	printf("Elapsed time: %.9f seconds\n", elapsed);
    printf("Final value of goal: %lld\n", goal);  

    return EXIT_SUCCESS;
}

