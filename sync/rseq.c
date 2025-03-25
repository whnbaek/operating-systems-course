#define _GNU_SOURCE
#include <linux/rseq.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sched.h>

// Per-thread rseq structure (aligned as required)
struct rseq rseq_area __attribute__((aligned(32))) = {
    .cpu_id = RSEQ_CPU_ID_UNINITIALIZED,
    .cpu_id_start = 0,
    .rseq_cs = 0,
    .flags = 0,
};

#define RSEQ_SIGNATURE 0x53053053
/* Allocate a large area for the TLS. */
#define RSEQ_THREAD_AREA_ALLOC_SIZE 1024
#include "/home/faculty/abk6349/data/librseq/include/rseq/abi.h"
static
__thread struct rseq_abi __rseq_abi __attribute__((tls_model("initial-exec"), aligned(RSEQ_THREAD_AREA_ALLOC_SIZE))) = {
    .cpu_id = RSEQ_ABI_CPU_ID_UNINITIALIZED,
};

// Function to register rseq with the kernel
int register_rseq() {
    int ret = syscall(SYS_rseq, &__rseq_abi, sizeof(struct rseq), 0, RSEQ_SIGNATURE);
    if (ret == -1) {
        perror("rseq register failed");
		fprintf(stderr, "Error number: %d\n", errno);
        return -1;
    }
    return 0;
}

// Function to migrate the thread to another CPU (simulating disturbance)
void migrate_thread(int target_cpu) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(target_cpu, &cpuset);
    
    if (sched_setaffinity(0, sizeof(cpu_set_t), &cpuset) != 0) {
        perror("sched_setaffinity failed");
        exit(1);
    }
}

// Worker thread function
void *worker_thread(void *arg) {
    if (register_rseq() != 0) {
        return NULL;
    }

    printf("Thread started on CPU: %d\n", rseq_area.cpu_id);

    int start_cpu, end_cpu;
    while (1) {
        start_cpu = rseq_area.cpu_id;  // Read CPU at the start

        // Simulating some critical work
        usleep(500000);  // 0.5 seconds

        end_cpu = rseq_area.cpu_id;  // Read CPU after work

        // Check if the CPU changed during execution (restart happened)
        if (start_cpu == end_cpu) {
            break;  // Successfully completed without migration
        } else {
            printf("RSEQ restarted! CPU changed from %d to %d. Retrying...\n", start_cpu, end_cpu);
        }
    }

    printf("RSEQ completed successfully on CPU: %d\n", end_cpu);
    return NULL;
}

int main() {
    pthread_t thread;

    // Create worker thread
    pthread_create(&thread, NULL, worker_thread, NULL);

    sleep(1);  // Let the thread start execution

    printf("Migrating thread to another CPU...\n");
    migrate_thread(1);  // Move thread to another CPU

    pthread_join(thread, NULL);
    return 0;
}

