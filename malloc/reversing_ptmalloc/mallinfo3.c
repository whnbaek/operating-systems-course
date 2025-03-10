#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "printer.c"
int main() {

	print_mallinfo(mallinfo2());
    
    // Print mallinfo before allocation
	printf("Entering Main\n");
	print_mallinfo(mallinfo2());
    
    // Allocate memory
    void *ptr = malloc(10); // 10 bytes
    
    // Print mallinfo after allocation
	printf("After Malloc \n");
	print_mallinfo(mallinfo2());
    
    free(ptr); // Free the memory

	// Print mallinfo after allocation
	printf("After Free \n");
	print_mallinfo(mallinfo2());

    return 0;
}

