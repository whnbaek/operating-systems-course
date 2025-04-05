#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include "libmap.h"

#define PAGE_SIZE 4096
int main() {
    parse_maps_and_print_physical();
    void *addr = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE,
                      MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    *((char *)addr) = 'A';  // Ensure the page is actually mapped

    uintptr_t vaddr = (uintptr_t)addr;
    uint64_t pfn = get_pfn(vaddr);

    if (pfn == 0) {
        printf("Page not present\n");
    } else {
        uint64_t paddr = (pfn * PAGE_SIZE) + (vaddr % PAGE_SIZE);
        printf("Virtual Address: %p\n", (void *)vaddr);
        printf("Physical Address: 0x%llx\n", (unsigned long long)paddr);
    }

    munmap(addr, PAGE_SIZE);
    return 0;
}
