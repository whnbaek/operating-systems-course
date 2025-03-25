#include <stdio.h>
#include <stdint.h>

volatile int lock_var = 0;  // Shared variable to be locked

// Function that uses CAS to atomically change lock_var
int compare_and_swap(volatile int *lock_var, int expected, int new_value) {
    int old_value;
    
    // Inline assembly for the CAS operation using CMPXCHG instruction
    __asm__ volatile (
        "movl %[expected], %%eax   \n"  // Load expected value into EAX
        "movl %[new_value], %%ebx  \n"  // Load new value into EBX
        "lock cmpxchg %%ebx, %[lock_var] \n"  // Compare and swap atomic operation
        "movl %%eax, %[old_value]   \n" // Move the result into old_value
        : [old_value] "=r" (old_value)   // Output: old_value
        : [lock_var] "m" (*lock_var),   // Input: address of lock_var
          [expected] "r" (expected),     // Input: expected value
          [new_value] "r" (new_value)   // Input: new value
        : "%eax", "%ebx", "memory"      // Clobbered registers
    );
    
    return old_value;  // Return the old value of lock_var
}

int main() {
    int expected = 0;
    int new_value = 1;
    
    // Use CAS to try and swap the value of lock_var
    int old_value = compare_and_swap(&lock_var, expected, new_value);
    
    if (old_value == expected) {
        printf("CAS succeeded: lock_var was changed from %d to %d\n", old_value, new_value);
    } else {
        printf("CAS failed: lock_var was %d (expected %d)\n", old_value, expected);
    }
    
    return 0;
}

