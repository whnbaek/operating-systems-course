#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *thread_func_1(void *arg) {
    void *ptr = malloc(16);  // Allocate 16 bytes
    printf("Thread 1 allocated:	 %p\n", ptr);
    free(ptr);  // Should be cached in Thread 1's arena
    printf("Thread 1 freed:	 	 %p\n", ptr);
	sleep(2);
    return NULL;
}

void *thread_func_2(void *arg) {
    sleep(1); // Ensure Thread 1 runs first
    void *ptr = malloc(16);  // Allocate again
    printf("Thread 2 allocated:	 %p\n", ptr);
    free(ptr);
    return NULL;
}

int main() {
    // Step 1:	 Allocate & free in the main thread
    void *main_ptr = malloc(16);
    printf("Main thread allocated:	 %p\n", main_ptr);
    free(main_ptr);
    printf("Main thread freed:	 %p\n", main_ptr);

	//Let's allocate again
	main_ptr = malloc(16);
	printf("Main thread allocated:	 %p\n", main_ptr);
    free(main_ptr);
    printf("Main thread freed:	 %p\n", main_ptr);

    pthread_t t1, t2;

    // Step 2:	 Run Thread 1
    pthread_create(&t1, NULL, thread_func_1, NULL);
   // pthread_join(t1, NULL);  

    // Step 3:	 Run Thread 2
    pthread_create(&t2, NULL, thread_func_2, NULL);
    pthread_join(t2, NULL);  

    return 0;
}

