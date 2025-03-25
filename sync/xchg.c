#include <stdio.h>

static inline int atomic_xchg(int *ptr, int new_val) {
    int old_val;
    __asm__ volatile (
        "xchg %0, %1"    // Swap old_val and *ptr atomically
        : "=r" (old_val), "=m" (*ptr)  // Output operands
        : "0" (new_val)   // Input operand (new_val)
        : "memory"        // Clobbers (ensures memory ordering)
    );
    return old_val;  // Return previous value of *ptr
}

int main() {
    int num = 42;
    printf("Before XCHG: num = %d\n", num);
    
    int prev = atomic_xchg(&num, 99);
    
    printf("After XCHG: num = %d (previous value was %d)\n", num, prev);
    return 0;
}

