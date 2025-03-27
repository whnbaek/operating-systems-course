#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

#define NUM_PRODUCERS 5

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
int slots_available = 1;  // Only 1 slot available

void* producer(void* arg) {
    int id = *(int*)arg;
    while (1) {
        pthread_mutex_lock(&mtx);
        if (slots_available > 0) {
            slots_available--;
            printf("Producer %d acquired a slot\n", id);
            pthread_mutex_unlock(&mtx);

            usleep(500000);  // Simulate work (500ms)

            pthread_mutex_lock(&mtx);
            slots_available++;
            printf("Producer %d released a slot\n", id);
        } else {
//			printf("TID %d: This sucks, someone already took the spot! Will try my luck some other time.\n", gettid());
		}
        pthread_mutex_unlock(&mtx);
        usleep(10000);  // Polling (BAD!)
    }
    return NULL;
}

int main() {
    pthread_t producers[NUM_PRODUCERS];
    int ids[NUM_PRODUCERS];

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        ids[i] = i + 1;
        pthread_create(&producers[i], NULL, producer, &ids[i]);
    }

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(producers[i], NULL);
    }

    return 0;
}

