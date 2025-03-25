#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <pthread.h>
#include <unistd.h>

// MCS Lock Node (Per-thread)
typedef struct mcs_node {
    struct mcs_node *next;
    atomic_int locked;
} mcs_node_t;

// MCS Lock Structure
typedef struct {
    atomic_intptr_t tail;  // Pointer to the last node in queue
} mcs_lock_t;

// Initialize the lock
void mcs_lock_init(mcs_lock_t *lock) {
    atomic_store(&lock->tail, (intptr_t)NULL);
}

// Acquire the lock
void mcs_lock_acquire(mcs_lock_t *lock, mcs_node_t *node) {
    node->next = NULL;
    atomic_store(&node->locked, 1);

    // Atomically set the tail to this node and get the previous tail
    mcs_node_t *prev = (mcs_node_t *)atomic_exchange(&lock->tail, (intptr_t)node);
    
    if (prev != NULL) {
        // Link previous node to current node
        prev->next = node;
        
        // Spin until lock is granted
        while (atomic_load(&node->locked)) {
            // Busy-wait
        }
    }
}

// Release the lock
void mcs_lock_release(mcs_lock_t *lock, mcs_node_t *node) {
    if (node->next == NULL) {
        // Try to reset the tail if no one is behind
        if (atomic_compare_exchange_strong(&lock->tail, (intptr_t *)&node, (intptr_t)NULL)) {
            return;
        }
        // Wait until the next pointer is set by another thread
        while (node->next == NULL) {
            // Busy-wait
        }
    }
    // Unlock the next node
    atomic_store(&node->next->locked, 0);
}

// Shared resource
int counter = 0;
mcs_lock_t lock;

void *thread_func(void *arg) {
    mcs_node_t node;

    for (int i = 0; i < 5; i++) {
        mcs_lock_acquire(&lock, &node);

        // Critical Section
        int local = counter;
        usleep(10000); // Simulate work
        counter = local + 1;
        printf("Thread %ld incremented counter to %d\n", (long)arg, counter);

        mcs_lock_release(&lock, &node);
        usleep(5000); // Simulate work outside critical section
    }
    return NULL;
}

int main() {
    pthread_t threads[5];
    mcs_lock_init(&lock);

    // Create threads
    for (long i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, thread_func, (void *)i);
    }

    // Join threads
    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Final counter value: %d\n", counter);
    return 0;
}

