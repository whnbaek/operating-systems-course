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

    // Call the add function using the function pointer
    printf("Calling add using function pointer:\n");
    printf("Result: %d\n", funcPtr(10, 5));


    // Assign the subtract function to the pointer
    funcPtr = &subtract;

	printf("Read Func Pointer %x", *( (char *)funcPtr) );

    // Call the subtract function using the function pointer
    printf("\nCalling subtract using function pointer:\n");
    printf("Result: %d\n", funcPtr(10, 5));

    // Using a function pointer as an argument to another function
    printf("\nUsing function pointers as arguments:\n");
    operate(15, 7, add);
    operate(15, 7, subtract);

    return 0;
}
