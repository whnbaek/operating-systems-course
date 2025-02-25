#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <errno.h>

int main() {
    pid_t pid;

    printf("Before fork\n");

    pid = (pid_t)syscall(SYS_fork);

    if (pid < 0) {
        // Fork failed
        fprintf(stderr, "Fork failed: %s\n", strerror(errno));
        return 1;
    } else if (pid == 0) {
        // Child process
        printf("I am the child process (PID: %d)\n", getpid());
        printf("My parent's PID is: %d\n", getppid());
    } else {
        // Parent process
        printf("I am the parent process (PID: %d)\n", getpid());
        printf("My child's PID is: %d\n", pid);
    }

    printf("This line is executed by both parent and child\n");

    return 0;
}

