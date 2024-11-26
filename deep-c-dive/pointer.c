#include <stdio.h>

int main() {
    int num = 42;          // Declare an integer variable
    int *ptr = &num;       // Declare a pointer and store the address of 'num'

    // Print the value of 'num' using both the variable and the pointer
    printf("Value of num: %d\n", num);
    printf("Value of num using pointer: %d\n", *ptr);

    // Print the address of 'num' using both '&num' and the pointer
    printf("Address of num: %p\n", (void *)&num);
    printf("Address of num using pointer: %p\n", (void *)ptr);

    // Modify the value of 'num' using the pointer
    *ptr = 99;
    printf("\nAfter modifying value using pointer:\n");
    printf("Value of num: %d\n", num);

    // Pointer to another variable
    int anotherNum = 25;
    ptr = &anotherNum;     // Update the pointer to point to anotherNum
    printf("\nPointer now points to anotherNum:\n");
    printf("Value of anotherNum: %d\n", *ptr);

    // Null pointer demonstration
    ptr = NULL;            // Set pointer to NULL
    printf("\nPointer set to NULL: %p\n", (void *)ptr);

    return 0;
}
