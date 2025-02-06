#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

int main() {
	int ask = 0;

	while (1) {
		// Ask the user to enter an integer
	    printf("Enter the size you want: ");
    	scanf("%d", &ask);

	    void *p = malloc(ask);
		size_t size = *((size_t *)(((char *) p) -  sizeof(size_t)));
		size &= ~(0x7);
	    printf("Requested: %d bytes, Allocated: %zu bytes, Size Field: %d, Return Pointer: %p\n", ask, malloc_usable_size(p), size, p);
	    free(p);
	}

    return 0;
}
