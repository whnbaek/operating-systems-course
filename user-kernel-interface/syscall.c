#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <stdio.h>

int main() {
	int syscall_num;
	// Prompt the user to enter a syscall number
    printf("Enter syscall number: ");
    
    // Read syscall number from standard input
    if (scanf("%ld", &syscall_num) != 1) {
        fprintf(stderr, "Invalid input.\n");
        return 1;
    }

    long ret = syscall(syscall_num);  // Syscall number for `fchmodat2`
    if (ret == -1) {
        perror("Syscall failed");
    }
    return 0;
}
