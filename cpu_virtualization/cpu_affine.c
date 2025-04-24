#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>

void print_affinity(const char* label) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);

    if (sched_getaffinity(0, sizeof(cpuset), &cpuset) == -1) {
        perror("sched_getaffinity");
        exit(1);
    }

    printf("%s CPU Affinity: ", label);
    for (int i = 0; i < CPU_SETSIZE; i++) {
        if (CPU_ISSET(i, &cpuset)) {
            printf("%d ", i);
        }
    }
    printf("\n");
}

int main() {
    // Print initial affinity
    print_affinity("Before");

    // Set affinity to CPU 0 only
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(0, &set);

    if (sched_setaffinity(0, sizeof(set), &set) == -1) {
        perror("sched_setaffinity");
        exit(1);
    }

    // Print new affinity
    print_affinity("After");

    // Run a loop and print actual CPU we're on
    for (int i = 0; i < 5; ++i) {
        printf("Running on CPU: %d\n", sched_getcpu());
        for (volatile int j = 0; j < 100000000; ++j);  // Burn CPU
    }

    return 0;
}

