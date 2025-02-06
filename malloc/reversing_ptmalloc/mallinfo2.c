#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

void print_mallinfo(struct mallinfo2 mi) {
    printf("=== Memory Allocation Info ===\n");
    printf("arena:     %zu bytes (Total heap obtained from OS)\n", mi.arena);
    printf("ordblks:   %zu (Number of allocated chunks)\n", mi.ordblks);
    printf("hblks:     %zu (Number of heap blocks)\n", mi.hblks);
    printf("hblkhd:    %zu bytes (Space in heap blocks)\n", mi.hblkhd);
    printf("uordblks:  %zu bytes (Total allocated space)\n", mi.uordblks);
    printf("fordblks:  %zu bytes (Total free space in heap)\n", mi.fordblks);
    printf("keepcost:  %zu bytes (Top-most, releasable space in heap)\n", mi.keepcost);
    printf("==============================\n");
}


int main() {

    // Print mallinfo before allocation
	printf("Entering Main\n");
	print_mallinfo(mallinfo2());
    
    // Allocate memory
    void *ptr = malloc(10); // 1 MB
    
    // Print mallinfo after allocation
	printf("After Malloc \n");
	print_mallinfo(mallinfo2());
    
    free(ptr); // Free the memory

	// Print mallinfo after allocation
	printf("After Free \n");
	print_mallinfo(mallinfo2());

    return 0;
}

