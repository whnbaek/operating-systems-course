#include <stdio.h>
#include <pthread.h>

int shared_data = 0; // Shared variable without synchronization

void *thread_func(void *arg) {
    for (int i = 0; i < 1000000; i++) {
        shared_data++; // Race condition: multiple threads modifying this
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, thread_func, NULL);
    pthread_create(&t2, NULL, thread_func, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Final value of shared_data: %d\n", shared_data);
    return 0;
}

