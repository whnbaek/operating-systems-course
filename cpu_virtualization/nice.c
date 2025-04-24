#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sched.h>

void cpu_bound_task(const char *label) {
    unsigned long long i = 0;
    while (1) {
        i++;
        if (i % 100000000 == 0) {
            printf("%s still running: %llu\n", label, i);
        }
    }
}

int main() {
	cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(0, &set); // Set CPU 0

    sched_setaffinity(0, sizeof(set), &set); // 0 = current process

    pid_t pid1 = fork();
    if (pid1 == 0) {
        // First child with default nice value (0)
        cpu_bound_task("Normal priority");
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        // Second child with higher nice value (lower priority)
        nice(19);  // Increase nice value
        cpu_bound_task("Low priority");
    }

    // Parent waits a bit then shows priorities
    sleep(1);
    printf("Parent: child 1 pid = %d, child 2 pid = %d\n", pid1, pid2);

    // Optional: run for some time, then kill
    sleep(10);
    kill(pid1, SIGKILL);
    kill(pid2, SIGKILL);
    wait(NULL);
    wait(NULL);
    return 0;
}

