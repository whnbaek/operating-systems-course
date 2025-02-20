#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <execinfo.h>
#include <unistd.h>
#include <ucontext.h>

#define BACKTRACE_SIZE 100

void print_registers(ucontext_t *context) {
    // Depending on the architecture (e.g., x86_64), we extract register states.
#ifdef __x86_64__
    printf("Register frame at the time of the fault:\n");
    printf("RIP: 0x%llx (Instruction Pointer)\n", (unsigned long long)context->uc_mcontext.gregs[REG_RIP]);
    printf("RSP: 0x%llx (Stack Pointer)\n", (unsigned long long)context->uc_mcontext.gregs[REG_RSP]);
    printf("RBP: 0x%llx (Base Pointer)\n", (unsigned long long)context->uc_mcontext.gregs[REG_RBP]);
    printf("RAX: 0x%llx\n", (unsigned long long)context->uc_mcontext.gregs[REG_RAX]);
    printf("RBX: 0x%llx\n", (unsigned long long)context->uc_mcontext.gregs[REG_RBX]);
    printf("RCX: 0x%llx\n", (unsigned long long)context->uc_mcontext.gregs[REG_RCX]);
    printf("RDX: 0x%llx\n", (unsigned long long)context->uc_mcontext.gregs[REG_RDX]);
    printf("RSI: 0x%llx\n", (unsigned long long)context->uc_mcontext.gregs[REG_RSI]);
    printf("RDI: 0x%llx\n", (unsigned long long)context->uc_mcontext.gregs[REG_RDI]);
    printf("R8 : 0x%llx\n", (unsigned long long)context->uc_mcontext.gregs[REG_R8]);
    printf("R9 : 0x%llx\n", (unsigned long long)context->uc_mcontext.gregs[REG_R9]);
    printf("R10: 0x%llx\n", (unsigned long long)context->uc_mcontext.gregs[REG_R10]);
    printf("R11: 0x%llx\n", (unsigned long long)context->uc_mcontext.gregs[REG_R11]);
    printf("R12: 0x%llx\n", (unsigned long long)context->uc_mcontext.gregs[REG_R12]);
    printf("R13: 0x%llx\n", (unsigned long long)context->uc_mcontext.gregs[REG_R13]);
    printf("R14: 0x%llx\n", (unsigned long long)context->uc_mcontext.gregs[REG_R14]);
    printf("R15: 0x%llx\n", (unsigned long long)context->uc_mcontext.gregs[REG_R15]);
#else
    printf("Register frame not supported for this architecture.\n");
#endif
}

void segfault_handler(int sig, siginfo_t *info, void *context) {
    void *buffer[BACKTRACE_SIZE];
    int frames;

    fprintf(stderr, "\nSegmentation fault (signal %d) detected.\n", sig);
    fprintf(stderr, "Fault address: %p\n", info->si_addr);

    // Get the backtrace
    frames = backtrace(buffer, BACKTRACE_SIZE);
	
	print_registers(context);

    // Print the backtrace
    fprintf(stderr, "\nBacktrace:\n");
    backtrace_symbols_fd(buffer, frames, STDERR_FILENO);

    // Exit the program
    exit(EXIT_FAILURE);
}

void cause_segfault() {
    int *ptr = NULL;
    *ptr = 42; // Dereference a NULL pointer to trigger a segfault
}

int main() {
    // Set up the signal handler for SIGSEGV
    struct sigaction sa;
    sa.sa_sigaction = segfault_handler;
    sa.sa_flags = SA_SIGINFO; // Use sigaction to pass signal info
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGSEGV, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    printf("Program running. Intentionally causing a segmentation fault...\n");

    // Deliberately cause a segmentation fault
    cause_segfault();

    return 0;
}

