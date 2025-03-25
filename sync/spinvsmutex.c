#define _GNU_SOURCE  // Required for syscall(SYS_gettid)
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>

pthread_spinlock_t spinlock;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Function to get thread ID
pid_t get_thread_id() {
    return syscall(SYS_gettid);
}

// Spinlock waiting thread (actively spins)
void* spinlock_waiting_thread(void* arg) {
    printf("[Spinlock Thread] PID: %d, TID: %ld - Waiting...\n", getpid(), get_thread_id());

    clock_t start = clock();
    pthread_spin_lock(&spinlock);  // Will spin here
    printf("[Spinlock Thread] Acquired lock after %.2f seconds\n",
           (double)(clock() - start) / CLOCKS_PER_SEC);

    pthread_spin_unlock(&spinlock);
    return NULL;
}

// Mutex waiting thread (blocks and sleeps)
void* mutex_waiting_thread(void* arg) {
    printf("[Mutex Thread] PID: %d, TID: %ld - Waiting...\n", getpid(), get_thread_id());

    clock_t start = clock();
    pthread_mutex_lock(&mutex);  // Will block and sleep
    printf("[Mutex Thread] Acquired lock after %.2f seconds\n",
           (double)(clock() - start) / CLOCKS_PER_SEC);

    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    pthread_t spin_tid, mutex_tid;

    printf("[Main Thread] PID: %d, TID: %ld - Initializing locks...\n", getpid(), get_thread_id());

    // Initialize spinlock and lock it
    pthread_spin_init(&spinlock, 0);
    pthread_spin_lock(&spinlock);

    // Lock mutex before starting thread
    pthread_mutex_lock(&mutex);

    // Create the waiting threads
    pthread_create(&spin_tid, NULL, spinlock_waiting_thread, NULL);
    pthread_create(&mutex_tid, NULL, mutex_waiting_thread, NULL);

    sleep(30);  // Give threads time to wait

    printf("[Main Thread] Releasing locks now...\n");

    pthread_spin_unlock(&spinlock);  // Unlock spinlock (spin thread continues)
    pthread_mutex_unlock(&mutex);    // Unlock mutex (mutex thread wakes up)

    // Wait for threads to complete
    pthread_join(spin_tid, NULL);
    pthread_join(mutex_tid, NULL);

    pthread_spin_destroy(&spinlock);

    return 0;
}

