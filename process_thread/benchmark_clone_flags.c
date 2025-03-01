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

#define NUM_PROCESSES 1000
#define STACK_SIZE (1024 * 1024)

static int child_func(void *arg) {
    exit(0);
}

static void * clone_func(void *arg) {
    return 0;
}


double benchmark_clone(int flags) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < NUM_PROCESSES; i++) {
        char *stack = malloc(STACK_SIZE);
        if (!stack) {
            perror("malloc");
            exit(1);
        }

        pid_t pid = clone(child_func, stack + STACK_SIZE,  flags, NULL);
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
	int flags = SIGCHLD;

	printf("Benchmarking no sharing memory clone():\n %.6f seconds\n", benchmark_clone(flags));
	flags = flags | CLONE_VM;
    printf("Benchmarking shared memory clone():\n %.6f seconds\n", benchmark_clone(flags | CLONE_VM));
	flags = flags |CLONE_FILES;
	printf("Benchmarking shared files and memory clone():\n %.6f seconds\n", benchmark_clone(flags | CLONE_VM | CLONE_FILES));
	flags = flags | CLONE_FS;
	printf("Benchmarking shared files, filessytem info (like CWD), and memory clone():\n %.6f seconds\n", benchmark_clone(flags | CLONE_VM | CLONE_FILES | CLONE_FS));
	flags = flags | CLONE_SIGHAND;
	printf("Benchmarking shared signal handlers, files, filessytem info (like CWD), and memory clone():\n %.6f seconds\n", benchmark_clone(flags | CLONE_VM | CLONE_FILES | CLONE_FS | CLONE_SIGHAND));
	flags = flags | CLONE_PARENT;
	printf("Benchmarking shared parent, signal handlers, files, filessytem info (like CWD), and memory clone():\n %.6f seconds\n", benchmark_clone(flags));
	flags = flags | CLONE_IO;
	printf("Benchmarking shared IO, parent, signal handlers, files, filessytem info (like CWD), and memory clone():\n %.6f seconds\n", benchmark_clone(flags));
	flags = flags | CLONE_VFORK;
	printf("Benchmarking vfork clone with shared IO, parent, signal handlers, files, filessytem info (like CWD), and memory clone():\n %.6f seconds\n", benchmark_clone(flags));


    return 0;
}

