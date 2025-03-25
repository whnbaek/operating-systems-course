#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

// Shared data
int shared_data = 0;
#define NUM_READERS 100
#define NUM_WRITERS 1
#define NUM_ITERATIONS 10000

// Simulation of work: perform a computation loop
void simulate_work() {
	int iterations =100;
    volatile double result = 0.0; // 'volatile' prevents optimization away of the loop
    for (int i = 0; i < iterations; ++i) {
        result += i * 0.0001;
    }
}


// Mutex-based synchronization
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

void* mutex_reader(void* arg) {
    for (int i = 0; i < NUM_ITERATIONS; ++i) {
        pthread_mutex_lock(&mtx);
        int value = shared_data; // Read shared data
		simulate_work();
        pthread_mutex_unlock(&mtx);
    }
    return NULL;
}

void* mutex_writer(void* arg) {
    for (int i = 0; i < NUM_ITERATIONS; ++i) {
        pthread_mutex_lock(&mtx);
        shared_data++; // Modify shared data
		simulate_work();
        pthread_mutex_unlock(&mtx);
    }
    return NULL;
}

// RWLock-based synchronization
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

void* rwlock_reader(void* arg) {
    for (int i = 0; i < NUM_ITERATIONS; ++i) {
        pthread_rwlock_rdlock(&rwlock);
        int value = shared_data; // Read shared data
		simulate_work();
        pthread_rwlock_unlock(&rwlock);
    }
    return NULL;
}

void* rwlock_writer(void* arg) {
    for (int i = 0; i < NUM_ITERATIONS; ++i) {
        pthread_rwlock_wrlock(&rwlock);
        shared_data++; // Modify shared data
		simulate_work();
        pthread_rwlock_unlock(&rwlock);
    }
    return NULL;
}

// Utility function to measure execution time
long long measure_time(void (*test_func)(void)) {
    struct timeval start, end;
    gettimeofday(&start, NULL);
    
    test_func();  // Call the test function
    
    gettimeofday(&end, NULL);
    long long elapsed_time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
    return elapsed_time;
}

// Mutex-based test function
void mutex_test() {
    pthread_t threads[NUM_READERS + 1]; // 10 readers + 1 writer


	// Launch writer thread
    pthread_create(&threads[NUM_READERS], NULL, mutex_writer, NULL);
    // Launch reader threads
    for (int i = 0; i < NUM_READERS; ++i) {
        pthread_create(&threads[i], NULL, mutex_reader, NULL);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_READERS + 1; ++i) {
        pthread_join(threads[i], NULL);
    }
}

// RWLock-based test function
void rwlock_test() {
    pthread_t threads[NUM_READERS + 1]; // 10 readers + 1 writer

	// Launch writer thread
    pthread_create(&threads[NUM_READERS], NULL, rwlock_writer, NULL);
    // Launch reader threads
    for (int i = 0; i < NUM_READERS; ++i) {
        pthread_create(&threads[i], NULL, rwlock_reader, NULL);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_READERS +1; ++i) {
        pthread_join(threads[i], NULL);
    }
}

int main() {
    // Test with Mutex
    printf("Running Mutex-based test...\n");
    long long mutex_time = measure_time(mutex_test);
    printf("Mutex-based test finished in %lld ms.\n", mutex_time);

    // Reset shared data
    shared_data = 0;

    // Test with RWLock
    printf("Running RWLock-based test...\n");
    long long rwlock_time = measure_time(rwlock_test);
    printf("RWLock-based test finished in %lld ms.\n", rwlock_time);

    // Compare results
    printf("\nRWLock is %s than Mutex by %lld ms.\n", 
           (mutex_time > rwlock_time) ? "faster" : "slower", 
           abs(mutex_time - rwlock_time));

    return 0;
}

