void print_mallinfo(struct mallinfo2 mi) {
    printf("=== Memory Allocation Info ===\n");
    printf("arena:     %zu bytes (Total heap obtained from OS)\n", mi.arena);
    printf("hblks:     %zu (Number of heap blocks mmaped)\n", mi.hblks);
    printf("hblkhd:    %zu bytes (Space in heap blocks mmaped)\n", mi.hblkhd);
    printf("uordblks:  %zu bytes (Total allocated space)\n", mi.uordblks);
    printf("fordblks:  %zu bytes (Total free space in heap)\n", mi.fordblks);
    printf("==============================\n");
}
