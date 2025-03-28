#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex;

void recursive_function(int count) {
    if (count == 0) return;
    
    pthread_mutex_lock(&mutex);
    printf("Lock acquired at recursion level %d\n", count);
    
    recursive_function(count - 1);

    printf("Unlocking at recursion level %d\n", count);
    pthread_mutex_unlock(&mutex);
}

int main() {
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&mutex, &attr);
    
    recursive_function(3);

    pthread_mutex_destroy(&mutex);
    return 0;
}

