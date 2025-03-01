#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#define ARRAY_SIZE (100 * 1024 * 1024)  // 100 MB array

// Function to measure time in microseconds
long get_time_microseconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}

int main() {
	long start_time, end_time;
    // Allocate memory
    int *data = malloc(ARRAY_SIZE);
    if (!data) {
        perror("malloc");
        return 1;
    }

    // Initialize memory (before fork)
    for (size_t i = 0; i < ARRAY_SIZE / sizeof(int); i++) {
        data[i] = 42;
    }
	// First write triggers COW (slow)
        start_time = get_time_microseconds();
        for (size_t i = 0; i < ARRAY_SIZE / sizeof(int); i++) {
            data[i] += 1;
        }
        end_time = get_time_microseconds();
        printf("Parent First write time: %ld microseconds\n", end_time - start_time);

        // Second write (should be faster)
        start_time = get_time_microseconds();
        for (size_t i = 0; i < ARRAY_SIZE / sizeof(int); i++) {
            data[i] += 1;
        }
        end_time = get_time_microseconds();
        printf("Parent Second write time: %ld microseconds\n", end_time - start_time);

    printf("Memory initialized. Forking...\n");

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        free(data);
        return 1;
    }

    if (pid == 0) {  // Child process
        long start_time, end_time;

        // First write triggers COW (slow)
        start_time = get_time_microseconds();
        for (size_t i = 0; i < ARRAY_SIZE / sizeof(int); i++) {
            data[i] += 1;
        }
        end_time = get_time_microseconds();
        printf("First write time: %ld microseconds\n", end_time - start_time);

        // Second write (should be faster)
        start_time = get_time_microseconds();
        for (size_t i = 0; i < ARRAY_SIZE / sizeof(int); i++) {
            data[i] += 1;
        }
        end_time = get_time_microseconds();
        printf("Second write time: %ld microseconds\n", end_time - start_time);

        free(data);
        exit(0);
    } else {  // Parent process
        wait(NULL);
        printf("Parent process finished.\n");
    }

    free(data);
    return 0;
}

