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
    void *ptr = malloc(10); // 10 - 1024 - 2048 bytes
    
    // Print mallinfo after allocation
	printf("After Malloc \n");
	print_mallinfo(mallinfo2());
    
    free(ptr); // Free the memory

	// Print mallinfo after allocation
	printf("After Free \n");
	print_mallinfo(mallinfo2());

	malloc_trim(0); // The argument is a threshold to tell malloc how much memory we should leave behind

	print_mallinfo(mallinfo2());


    return 0;
}

