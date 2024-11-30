#include <stdio.h>

// Function attribute: mark a function as deprecated
__attribute__((deprecated))
void oldFunction() {
    printf("This function is deprecated.\n");
}

// Function attribute: force inline
__attribute__((always_inline)) inline void inlineFunction() {
    printf("This function is always inlined.\n");
}

// Variable attribute: align a variable in memory
__attribute__((aligned(16))) int alignedVar;

// Function attribute: specify a function doesn't return
__attribute__((noreturn)) void exitWithError() {
    printf("Exiting with error...\n");
    _exit(1); // Exit without returning
}

// Function attribute: unused parameter
void unusedParameterDemo(int x __attribute__((unused))) {
    printf("The parameter 'x' is unused.\n");
}

int main() {
    printf("Demonstrating attributes:\n");

    // Deprecated function call (will generate a warning)
    oldFunction();

    // Always-inline function
    inlineFunction();

    // Aligned variable
    printf("Address of alignedVar: %p\n", (void*)&alignedVar);
    if ((unsigned long)&alignedVar % 16 == 0) {
        printf("alignedVar is correctly aligned to 16 bytes.\n");
    }

    // Unused parameter demonstration
    unusedParameterDemo(42);

    // noreturn function (commented out to allow rest of the program to run)
    // exitWithError();

    return 0;
}

