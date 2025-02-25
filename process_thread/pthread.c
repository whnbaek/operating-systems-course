#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Function to be executed by the new thread
void* my_thread_function(void* arg) {
    int* num = (int*)arg;  // Cast the argument to the correct type
    printf("Hello from the thread! The number is: %d\n", *num);
    return NULL;
}

int main() {
    pthread_t thread;  // Declare a thread variable
    int number = 42;   // Argument to pass to the thread

    // Create a new thread that runs `my_thread_function`
    int ret = pthread_create(&thread, NULL, my_thread_function, (void*)&number);
    if (ret != 0) {
        perror("pthread_create");
        exit(1);
    }

    // Wait for the thread to finish execution
    ret = pthread_join(thread, NULL);
    if (ret != 0) {
        perror("pthread_join");
        exit(1);
    }

    printf("Main thread finished execution.\n");
    return 0;
}

