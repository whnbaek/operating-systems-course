#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include "libmap.h"

#define PAGE_SIZE 4096
#define PAGEMAP_ENTRY 8

uint64_t get_pfn(uintptr_t vaddr) {
    uint64_t value;
    off_t offset = (vaddr / PAGE_SIZE) * PAGEMAP_ENTRY;

    int fd = open("/proc/self/pagemap", O_RDONLY);
    if (fd < 0) {
        perror("open pagemap");
        return 0;
    }

    if (lseek(fd, offset, SEEK_SET) == -1) {
        perror("lseek pagemap");
        close(fd);
        return 0;
    }

    if (read(fd, &value, PAGEMAP_ENTRY) != PAGEMAP_ENTRY) {
        perror("read pagemap");
        close(fd);
        return 0;
    }

    close(fd);

    if (!(value & (1ULL << 63))) {
        // Page not present
        return 0;
    }

    uint64_t pfn = value & ((1ULL << 55) - 1);
    return pfn;
}

void parse_maps_and_print_physical() {
    FILE *maps = fopen("/proc/self/maps", "r");
    if (!maps) {
        perror("fopen maps");
        exit(1);
    }

    char line[512];
    while (fgets(line, sizeof(line), maps)) {
        uintptr_t start, end;
        char perms[5];
        if (sscanf(line, "%lx-%lx %4s", &start, &end, perms) != 3)
            continue;

        // Optional: only print rw private mappings
        if (strchr(perms, 'r') == NULL)
            continue;

        printf("\nMapping: %016lx - %016lx (%s)\n", start, end, perms);

        for (uintptr_t addr = start; addr < end; addr += PAGE_SIZE) {
            uint64_t pfn = get_pfn(addr);
            if (pfn == 0)
                continue;  // Unmapped page

            uint64_t phys = (pfn * PAGE_SIZE) + (addr % PAGE_SIZE);
            printf("  VA: %016lx => PA: %016llx\n",
                   (unsigned long)addr,
                   (unsigned long long)phys);
        }
    }

    fclose(maps);
}

void print_map(void * data) {
	uint64_t pfn;
        uint64_t paddr;
    pfn = get_pfn((uintptr_t)data);
        paddr = (pfn * PAGE_SIZE) + ((unsigned long long)data % PAGE_SIZE);
        printf("Virtual Address: %p\n", (void *)data);
        printf("Physical Address: 0x%llx\n", (unsigned long long)paddr);
}
