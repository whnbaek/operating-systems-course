#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

pthread_mutex_t mutex;

void* thread1_function(void* arg) {
    printf("Thread 1: Locking mutex...\n");
    if (pthread_mutex_lock(&mutex) != 0) {
        perror("Thread 1: Mutex lock failed");
        return NULL;
    }
    printf("Thread 1: Mutex locked. Sleeping...\n");
    sleep(3);  // Simulating work (Thread 1 holds the lock)
    printf("Thread 1: Not unlocking mutex!\n");
    return NULL;
}

void* thread2_function(void* arg) {
    sleep(1);  // Ensure thread 1 locks first
    printf("Thread 2: Trying to unlock mutex (Incorrect!)...\n");
    
    int result = pthread_mutex_unlock(&mutex);  // WRONG: Unlocking from a different thread

	if (result == EPERM) {
		printf("Permission error?\n");
	}
	else if (result != 0) {
		printf("Thread 2: Mutex unlock failed\n");
    } else {
        printf("Thread 2: Somehow unlocked the mutex (should not happen!)\n");
    }

    return NULL;
}

int main() {
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);  // Use error-checking mutex
    pthread_mutex_init(&mutex, &attr);

    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread1_function, NULL);
    pthread_create(&t2, NULL, thread2_function, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&mutex);
    return 0;
}

