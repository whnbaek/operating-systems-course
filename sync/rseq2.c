#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syscall.h>
#include <linux/rseq.h>
#include <errno.h>

// Per-thread rseq structure (aligned as required)
struct rseq rseq_area __attribute__((aligned(32))) = {
    .cpu_id = RSEQ_CPU_ID_UNINITIALIZED,
    .cpu_id_start = 0,
    .rseq_cs = 0,
    .flags = 0,
};

#define RSEQ_SIGNATURE 0x53053053

// Function to register rseq with the kernel
int register_rseq() {
    int ret = syscall(SYS_rseq, &rseq_area, sizeof(struct rseq), 0, RSEQ_SIGNATURE);
    if (ret == -1) {
        perror("rseq register failed");
        fprintf(stderr, "Error number: %d\n", errno);
        return -1;
    }
    return 0;
}

// Function to unregister rseq
int unregister_rseq() {
    int ret = syscall(SYS_rseq, &rseq_area, sizeof(struct rseq), RSEQ_FLAG_UNREGISTER, RSEQ_SIGNATURE);
    if (ret == -1) {
        perror("rseq unregister failed");
        fprintf(stderr, "Error number: %d\n", errno);
        return -1;
    }
    return 0;
}

int main() {
    printf("Registering rseq...\n");
    if (register_rseq() == 0) {
        printf("rseq registered successfully.\n");
        printf("CPU ID: %d\n", rseq_area.cpu_id);
        
        // Your rseq operations would go here
        
        printf("Unregistering rseq...\n");
        if (unregister_rseq() == 0) {
            printf("rseq unregistered successfully.\n");
        }
    }
    
    return 0;
}

