#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define TARGET 1000000  // The goal each thread will contribute to reaching

volatile unsigned long long goal = 0;  // Shared global variable, volatile to prevent compiler optimizations

void *increment_goal(void *arg) {
    while (goal < TARGET) {  // No locking, race condition expected
        goal++;
    }
    return NULL;
}

int main(int argc, char *argv[]) {
	increment_goal(NULL);

    return EXIT_SUCCESS;
}

