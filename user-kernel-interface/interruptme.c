#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t sigint_count = 0;

void handle_sigint(int sig) {
    sigint_count++;
    printf("\nSIGINT caught! Count: %d\n", sigint_count);
    
    if (sigint_count >= 2) {
        printf("Exiting after receiving two SIGINT signals.\n");
        exit(0);
    }
}

int main() {
    // Set up the signal handler for SIGINT
    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sa.sa_flags = 0; // No special flags
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Error setting up signal handler");
        exit(1);
    }

    // Main loop
    printf("Press Ctrl+C to send SIGINT. Program will exit after 2 signals.\n");
    while (1) {
        // Infinite loop to keep the program running
        pause(); // Suspend until a signal is caught
    }

    return 0;
}

