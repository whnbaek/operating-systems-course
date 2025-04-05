#ifndef LIBMAP_H
#define LIBMAP_H

#include <stdint.h>

// Returns the PFN (Page Frame Number) for a given virtual address.
// Returns 0 if page is not present or on error.
uint64_t get_pfn(uintptr_t vaddr);

// Parses /proc/self/maps and prints all virtual to physical page mappings.
void parse_maps_and_print_physical(void);

// Print the map
void print_map(void * data);

#define PAGE_SIZE 4096
#endif // LIBMAP_H

