#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <ucontext.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>

#define PAGE_SIZE 4096

// Our hand-crafted rep movsb wrapper
void rep_movsb_copy(void *dst, void *src, size_t len) {
    asm volatile (
        "cld\n\t"          // Clear Direction Flag for forward copy
        "rep movsb\n\t"    // Copy RCX bytes from [RSI] to [RDI]
        :
        : "D"(dst), "S"(src), "c"(len)
        : "memory"
    );
}

void segv_handler(int sig, siginfo_t *info, void *ucontext_raw) {
    ucontext_t *ctx = (ucontext_t *)ucontext_raw;

    void *fault_addr = info->si_addr;
    void *aligned_addr = (void *)((uintptr_t)fault_addr & ~(PAGE_SIZE - 1));

    printf("=== SIGSEGV handler triggered ===\n");
    printf("Fault address : %p\n", fault_addr);
    printf("Aligned addr  : %p\n", aligned_addr);
    printf("RCX (count)   : 0x%llx\n", (unsigned long long)ctx->uc_mcontext.gregs[REG_RCX]);
    printf("RSI (src)     : 0x%llx\n", (unsigned long long)ctx->uc_mcontext.gregs[REG_RSI]);
    printf("RDI (dst)     : 0x%llx\n", (unsigned long long)ctx->uc_mcontext.gregs[REG_RDI]);

    // Try to allow read/write access to the faulting page
    if (mprotect(aligned_addr, PAGE_SIZE, PROT_READ | PROT_WRITE) == -1) {
        perror("mprotect failed in handler");
        _exit(1); // Still exit if we can't fix the issue
    }

    // Return normally so the instruction can be retried
}

int main() {
    // Set up SIGSEGV handler
    struct sigaction sa = {
        .sa_flags = SA_SIGINFO,
        .sa_sigaction = segv_handler,
    };
    sigemptyset(&sa.sa_mask);
    sigaction(SIGSEGV, &sa, NULL);

    // Allocate two pages
    char *region = mmap(NULL, 2 * PAGE_SIZE, PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (region == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    // Protect the second page to cause SIGSEGV on access
    mprotect(region + PAGE_SIZE, PAGE_SIZE, PROT_NONE);

    // Fill first page with some data
    for (int i = 0; i < PAGE_SIZE; ++i)
        region[i] = 'A' + (i % 26);

    // Trigger a forward overlapping copy into protected page
    rep_movsb_copy(region + 2, region, PAGE_SIZE);

    return 0;
}

