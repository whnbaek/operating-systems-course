#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include "libmap.h"

#define PAGE_SIZE 4096
int main() {
    void *addr = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE,
                      MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    *((char *)addr) = 'A';  // Ensure the page is actually mapped

    uintptr_t vaddr = (uintptr_t)addr;

	print_map((void *) vaddr);

    munmap(addr, PAGE_SIZE);
    return 0;
}
