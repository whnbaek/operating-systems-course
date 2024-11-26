#include <stdio.h>

// Define some example functions
int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

// Function that takes a function pointer as a parameter
void operate(int x, int y, int (*operation)(int, int)) {
    printf("Result: %d\n", operation(x, y));
}

int main() {
    // Declare a function pointer
    int (*funcPtr)(int, int);

    // Assign the address of the add function to the pointer
    funcPtr = &add;

	printf("Function pointer address: %p\n", (void *)funcPtr);


    // Assign the subtract function to the pointer
    funcPtr = &subtract;


    return 0;
}
