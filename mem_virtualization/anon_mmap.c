#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>

int main() {
    size_t size = 4096; // Allocate one page (4 KB)

    // Anonymous memory mapping (not backed by a file)
    void *mem = mmap(NULL, size,
                     PROT_READ | PROT_WRITE,     // Read/write access
                     MAP_PRIVATE | MAP_ANONYMOUS,// Private + not backed by a file
                     -1, 0);                     // fd = -1 and offset = 0 for anonymous maps

    if (mem == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // Use the memory like a regular buffer
    strcpy((char *)mem, "Hello from anonymous mmap!");
    printf("%s\n", (char *)mem);

    // Always unmap when done
    if (munmap(mem, size) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }

    return 0;
}

