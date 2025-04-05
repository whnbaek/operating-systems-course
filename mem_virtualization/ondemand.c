#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/mman.h>
#include <ucontext.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>

#define PAGE_SIZE 4096
#define NUM_PAGES 16

char *region = NULL;

void segv_handler(int sig, siginfo_t *info, void *ucontext_raw) {
    ucontext_t *ctx = (ucontext_t *)ucontext_raw;
    void *fault_addr = info->si_addr;
    void *page_start = (void *)((uintptr_t)fault_addr & ~(PAGE_SIZE - 1));

    printf("[SIGSEGV] Fault at %p (align: %p)\n", fault_addr, page_start);

    if (page_start < (void *)region || page_start >= (void *)(region + NUM_PAGES * PAGE_SIZE)) {
        fprintf(stderr, "[ERROR] Fault outside managed region: %p\n", fault_addr);
        _exit(1);
    }

    void *mapped = mmap(page_start, PAGE_SIZE, PROT_READ | PROT_WRITE,
                        MAP_ANONYMOUS | MAP_PRIVATE | MAP_FIXED, -1, 0);
    if (mapped == MAP_FAILED) {
        perror("mmap in handler failed");
        _exit(1);
    }

    printf("[INFO] Mapped page at %p\n", mapped);
}

void setup_sigsegv_handler() {
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = segv_handler;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGSEGV, &sa, NULL) == -1) {
        perror("sigaction failed");
        exit(1);
    }
}

int main() {
    setup_sigsegv_handler();

    // Reserve a conceptual address space — no actual mapping!
    region = (char *)((uintptr_t)mmap(NULL, NUM_PAGES * PAGE_SIZE,
                                      PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
    if (region == MAP_FAILED) {
        perror("initial address reservation failed");
        return 1;
    }

    // Immediately unmap it so we can demand-map later
    if (munmap(region, NUM_PAGES * PAGE_SIZE) == -1) {
        perror("munmap failed");
        return 1;
    }

    printf("Reserved region: %p - %p\n", region, region + NUM_PAGES * PAGE_SIZE);

    // Trigger demand-mapping by writing into pages
    for (int i = 0; i < NUM_PAGES * PAGE_SIZE; i += 128) {
        region[i] = 'A' + (i % 26); // SIGSEGV triggers handler
    }

    printf("All pages touched and demand-mapped.\n");

    return 0;
}

