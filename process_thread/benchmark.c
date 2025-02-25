#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <errno.h>
#include <pthread.h>

#define NUM_PROCESSES 1000
#define STACK_SIZE (1024 * 1024)

static int child_func(void *arg) {
    return 0;
}

static void * pthread_func(void *arg) {
    return 0;
}


double benchmark_pthread() {
    struct timespec start, end;
	pthread_t thread_id[NUM_PROCESSES];
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < NUM_PROCESSES; i++) {
	    if (pthread_create(&thread_id[i], NULL, pthread_func, NULL) != 0) {
    	    perror("pthread_create failed");
	        exit(EXIT_FAILURE);
	    }
    }

    for (int i = 0; i < NUM_PROCESSES; i++) {
       	pthread_join(thread_id[i], NULL);
		break;
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

double benchmark_real_fork() {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < NUM_PROCESSES; i++) {
        pid_t pid =  (pid_t)syscall(SYS_fork);;
        if (pid == 0) {
            exit(0);
        } else if (pid < 0) {
            perror("fork");
            exit(1);
        }
    }

    for (int i = 0; i < NUM_PROCESSES; i++) {
        wait(NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}
double benchmark_fork() {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < NUM_PROCESSES; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            exit(0);
        } else if (pid < 0) {
            perror("fork");
            exit(1);
        }
    }

    for (int i = 0; i < NUM_PROCESSES; i++) {
        wait(NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

double benchmark_vfork() {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < NUM_PROCESSES; i++) {
        pid_t pid = vfork();
        if (pid == 0) {
            _exit(0);
        } else if (pid < 0) {
            perror("vfork");
            exit(1);
        }
    }

	for (int i = 0; i < NUM_PROCESSES; i++) {
        wait(NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

double benchmark_clone() {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < NUM_PROCESSES; i++) {
        char *stack = malloc(STACK_SIZE);
        if (!stack) {
            perror("malloc");
            exit(1);
        }

        pid_t pid = clone(child_func, stack + STACK_SIZE, CLONE_VM | SIGCHLD, NULL);
        if (pid < 0) {
            perror("clone");
            exit(1);
        }
    }

    for (int i = 0; i < NUM_PROCESSES; i++) {
        wait(NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

int main() {
	printf("Benchmarking real fork(): %.6f seconds\n", benchmark_real_fork());
    printf("Benchmarking glibc fork(): %.6f seconds\n", benchmark_fork());
    printf("Benchmarking vfork(): %.6f seconds\n", benchmark_vfork());
    printf("Benchmarking clone(): %.6f seconds\n", benchmark_clone());
	printf("Benchmarking pthread(): %.6f seconds\n",benchmark_pthread());
    return 0;
}

