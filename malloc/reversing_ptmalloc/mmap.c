#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include <sys/resource.h>

void check_allocation(void *ptr, size_t size) {
    static void *heap_end = NULL;

    if (!heap_end) {
        heap_end = sbrk(0);  // Get current program break
    }

    if (ptr >= heap_end) {
        printf("Allocated %zu bytes using **mmap** (outside heap)\n", size);
    } else {
        printf("Allocated %zu bytes using **heap** (sbrk)\n", size);
    }
}

int main() {
    // Try allocating different sizes
    size_t sizes[] = {8 * 1024, 64 * 1024, 128 * 1024, 256 * 1024, 512 * 1024, 1024 * 1024};
    
    for (int i = 0; i < sizeof(sizes) / sizeof(sizes[0]); i++) {
        void *ptr = malloc(sizes[i]);
        if (ptr) {
            check_allocation(ptr, sizes[i]);
            free(ptr);
        } else {
            printf("Failed to allocate %zu bytes\n", sizes[i]);
        }
    }

    return 0;
}

