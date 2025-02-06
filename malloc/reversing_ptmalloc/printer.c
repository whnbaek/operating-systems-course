// Function to print the mallinfo2 fields with descriptions
void print_mallinfo_full(struct mallinfo2 mi) {
    printf("arena: %zu\n", mi.arena);
    printf("  (Total allocated space in the heap, i.e., total memory allocated from the operating system)\n");

    printf("ordblks: %zu\n", mi.ordblks);
    printf("  (Number of ordinary free blocks, i.e., free memory blocks managed by the allocator that are not mmap'd)\n");

    printf("smblks: %zu\n", mi.smblks);
    printf("  (Number of small free blocks, i.e., blocks that are less than a threshold size managed by the allocator)\n");

    printf("hblks: %zu\n", mi.hblks);
    printf("  (Number of heap blocks obtained via mmap(), i.e., blocks of memory mapped directly from the operating system via mmap)\n");

    printf("hblkhd: %zu\n", mi.hblkhd);
    printf("  (Total size of memory obtained via mmap() from the operating system)\n");

    printf("usmblks: %zu\n", mi.usmblks);
    printf("  (Number of small blocks in use, i.e., small blocks that are currently being used by the program)\n");

    printf("fsmblks: %zu\n", mi.fsmblks);
    printf("  (Number of free small blocks, i.e., small blocks in the heap that are free and available for allocation)\n");

    printf("uordblks: %zu\n", mi.uordblks);
    printf("  (Total allocated memory in use by the program, i.e., memory allocated by malloc that is being used by the program)\n");

    printf("fordblks: %zu\n", mi.fordblks);
    printf("  (Total free space in the heap, i.e., free memory available for new allocations by the allocator)\n");

    printf("keepcost: %zu\n", mi.keepcost);
    printf("  (Top-most, releasable space in the heap, i.e., free memory that can be released back to the operating system if necessary)\n");
}
