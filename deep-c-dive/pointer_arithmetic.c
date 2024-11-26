#include <stdio.h>

int main() {
    int numbers[] = {10, 20, 30, 40, 50}; // Array of integers
    int *ptr = numbers;                  // Pointer to the first element of the array

    printf("Pointer Arithmetic Example:\n");
    printf("Address of numbers[0]: %p, Value: %d\n", (void *)ptr, *ptr);

    // Increment the pointer to point to the next element
    ptr++;
    printf("Address after ptr++: %p, Value: %d\n", (void *)ptr, *ptr);

    // Add an offset to the pointer
    ptr += 2;
    printf("Address after ptr += 2: %p, Value: %d\n", (void *)ptr, *ptr);

    // Decrement the pointer
    ptr--;
    printf("Address after ptr--: %p, Value: %d\n", (void *)ptr, *ptr);

    // Subtract an offset
    ptr -= 2;
    printf("Address after ptr -= 2: %p, Value: %d\n", (void *)ptr, *ptr);

    // Calculate the distance between two pointers
    int *startPtr = &numbers[0];
    int *endPtr = &numbers[4];
    printf("\nDistance between startPtr and endPtr: %ld elements\n", endPtr - startPtr);

    return 0;
}

